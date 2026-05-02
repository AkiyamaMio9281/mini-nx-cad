#include "mnx/modeling/ProfileUtils.h"

#include "mnx/core/math/Tolerance.h"
#include "mnx/sketch/SketchCircle.h"
#include "mnx/sketch/SketchLine.h"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <utility>

namespace mnx {

namespace {

[[nodiscard]] bool samePoint(const Point2& lhs, const Point2& rhs) {
    return lhs.isApprox(rhs, kDefaultTolerance);
}

[[nodiscard]] bool appendConnectedLine(std::vector<const SketchLine*>& remaining, std::vector<Point2>& vertices) {
    const Point2 current = vertices.back();
    for(auto iterator = remaining.begin(); iterator != remaining.end(); ++iterator) {
        const SketchLine* line = *iterator;
        if(samePoint(line->start(), current)) {
            vertices.push_back(line->end());
            remaining.erase(iterator);
            return true;
        }
        if(samePoint(line->end(), current)) {
            vertices.push_back(line->start());
            remaining.erase(iterator);
            return true;
        }
    }
    return false;
}

[[nodiscard]] double crossAt(const Point2& previous, const Point2& current, const Point2& next) {
    const Vec2 first = current - previous;
    const Vec2 second = next - current;
    return first.cross(second);
}

} // namespace

Profile Profile::polyline(std::vector<Point2> vertices) {
    Profile profile;
    profile.kind_ = ProfileKind::Polyline;
    if(!vertices.empty() && samePoint(vertices.front(), vertices.back())) {
        vertices.pop_back();
    }
    profile.vertices_ = std::move(vertices);
    return profile;
}

Profile Profile::circle(const Point2& center, double radius, int segmentCount) {
    Profile profile;
    profile.kind_ = ProfileKind::Circle;
    profile.center_ = center;
    profile.radius_ = radius;
    profile.segmentCount_ = std::max(segmentCount, 8);
    return profile;
}

ProfileKind Profile::kind() const {
    return kind_;
}

const std::vector<Point2>& Profile::vertices() const {
    return vertices_;
}

const Point2& Profile::center() const {
    return center_;
}

double Profile::radius() const {
    return radius_;
}

int Profile::segmentCount() const {
    return segmentCount_;
}

double Profile::area() const {
    if(kind_ == ProfileKind::Circle) {
        return std::numbers::pi * radius_ * radius_;
    }
    return std::abs(ProfileUtils::signedArea(vertices_));
}

std::vector<Point2> Profile::tessellatedBoundary() const {
    if(kind_ == ProfileKind::Polyline) {
        return vertices_;
    }

    std::vector<Point2> points;
    points.reserve(static_cast<std::size_t>(segmentCount_));
    for(int index = 0; index < segmentCount_; ++index) {
        const double angle = 2.0 * std::numbers::pi * static_cast<double>(index) / static_cast<double>(segmentCount_);
        points.emplace_back(center_.x() + std::cos(angle) * radius_, center_.y() + std::sin(angle) * radius_);
    }
    return points;
}

Result<Profile> ProfileUtils::extractClosedProfile(const Sketch& sketch) {
    std::vector<const SketchLine*> lines;
    const SketchCircle* circle = nullptr;

    for(const SketchEntity* entity : sketch.entities()) {
        if(entity->type() == SketchEntityType::Line) {
            lines.push_back(static_cast<const SketchLine*>(entity));
        } else if(entity->type() == SketchEntityType::Circle) {
            if(circle != nullptr || !lines.empty()) {
                return Result<Profile>::failure(Status::failedPrecondition(
                    "Only a single circle or one closed line loop profile is supported"
                ));
            }
            circle = static_cast<const SketchCircle*>(entity);
        } else {
            return Result<Profile>::failure(Status::failedPrecondition(
                "Unsupported sketch entity in profile; only lines and one circle are supported"
            ));
        }
    }

    if(circle != nullptr) {
        if(circle->radius() <= kDefaultTolerance) {
            return Result<Profile>::failure(Status::failedPrecondition("Circle profile radius must be positive"));
        }
        return Profile::circle(circle->center(), circle->radius());
    }

    if(lines.size() < 3) {
        return Result<Profile>::failure(Status::failedPrecondition("Line profile must contain at least three edges"));
    }

    std::vector<const SketchLine*> remaining = lines;
    std::vector<Point2> orderedVertices {remaining.front()->start(), remaining.front()->end()};
    remaining.erase(remaining.begin());

    while(!remaining.empty()) {
        if(!appendConnectedLine(remaining, orderedVertices)) {
            return Result<Profile>::failure(Status::failedPrecondition("Line profile contains disconnected edges"));
        }
    }

    if(!isClosedPolyline(orderedVertices)) {
        return Result<Profile>::failure(Status::failedPrecondition("Line profile is open"));
    }

    Profile profile = Profile::polyline(std::move(orderedVertices));
    if(profile.vertices().size() < 3 || profile.area() <= kDefaultTolerance) {
        return Result<Profile>::failure(Status::failedPrecondition("Line profile has zero or degenerate area"));
    }
    if(!isConvexPolyline(profile.vertices())) {
        return Result<Profile>::failure(Status::failedPrecondition(
            "Concave or self-intersecting line profiles are not supported by the current extrude mesh builder"
        ));
    }
    return profile;
}

bool ProfileUtils::isClosedPolyline(const std::vector<Point2>& vertices) {
    return vertices.size() >= 4 && samePoint(vertices.front(), vertices.back());
}

bool ProfileUtils::isConvexPolyline(const std::vector<Point2>& vertices) {
    if(vertices.size() < 3) {
        return false;
    }

    int sign = 0;
    for(std::size_t index = 0; index < vertices.size(); ++index) {
        const Point2& previous = vertices[(index + vertices.size() - 1) % vertices.size()];
        const Point2& current = vertices[index];
        const Point2& next = vertices[(index + 1) % vertices.size()];
        const double cross = crossAt(previous, current, next);
        if(std::abs(cross) <= kDefaultTolerance) {
            continue;
        }

        const int currentSign = cross > 0.0 ? 1 : -1;
        if(sign == 0) {
            sign = currentSign;
        } else if(sign != currentSign) {
            return false;
        }
    }
    return sign != 0;
}

double ProfileUtils::signedArea(const std::vector<Point2>& vertices) {
    if(vertices.size() < 3) {
        return 0.0;
    }

    double area = 0.0;
    for(std::size_t index = 0; index < vertices.size(); ++index) {
        const Point2& current = vertices[index];
        const Point2& next = vertices[(index + 1) % vertices.size()];
        area += current.x() * next.y() - next.x() * current.y();
    }
    return area * 0.5;
}

} // namespace mnx
