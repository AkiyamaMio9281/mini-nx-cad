#include "mnx/geometry/algorithms/Intersections2D.h"

#include <algorithm>
#include <cmath>

namespace {

bool pointOnSegment(const mnx::Point2& point, const mnx::Segment2D& segment, double tolerance) {
    const mnx::Vec2 fromStart = point - segment.start();
    const mnx::Vec2 segmentVector = segment.end() - segment.start();
    return mnx::isZero(fromStart.cross(segmentVector), tolerance)
        && fromStart.dot(segmentVector) >= -tolerance
        && fromStart.dot(segmentVector) <= segmentVector.squaredNorm() + tolerance;
}

} // namespace

namespace mnx {

Intersection2DResult intersect(const Line2D& lhs, const Line2D& rhs) {
    const double denominator = lhs.direction().cross(rhs.direction());
    const Vec2 delta = rhs.point() - lhs.point();

    if(isZero(denominator)) {
        if(isZero(delta.cross(lhs.direction()))) {
            return {IntersectionType::Coincident, {}};
        }
        return {IntersectionType::Parallel, {}};
    }

    const double t = delta.cross(rhs.direction()) / denominator;
    return {IntersectionType::Point, {lhs.evaluate(t)}};
}

Intersection2DResult intersect(const Segment2D& lhs, const Segment2D& rhs) {
    const Vec2 p = lhs.start().vector();
    const Vec2 r = lhs.end() - lhs.start();
    const Vec2 q = rhs.start().vector();
    const Vec2 s = rhs.end() - rhs.start();
    const double rCrossS = r.cross(s);
    const Vec2 qMinusP = q - p;

    if(isZero(rCrossS)) {
        if(!isZero(qMinusP.cross(r))) {
            return {IntersectionType::Parallel, {}};
        }

        if(isZero(r.squaredNorm()) || isZero(s.squaredNorm())) {
            if(lhs.start().isApprox(rhs.start())) {
                return {IntersectionType::Point, {lhs.start()}};
            }
            return {IntersectionType::None, {}};
        }

        const double t0 = qMinusP.dot(r) / r.squaredNorm();
        const double t1 = t0 + s.dot(r) / r.squaredNorm();
        const double start = std::max(0.0, std::min(t0, t1));
        const double end = std::min(1.0, std::max(t0, t1));
        if(start > end + kDefaultTolerance) {
            return {IntersectionType::None, {}};
        }
        if(almostEqual(start, end)) {
            return {IntersectionType::Point, {lhs.start() + r * start}};
        }
        return {IntersectionType::Overlap, {lhs.start() + r * start, lhs.start() + r * end}};
    }

    const double t = qMinusP.cross(s) / rCrossS;
    const double u = qMinusP.cross(r) / rCrossS;
    if(t < -kDefaultTolerance || t > 1.0 + kDefaultTolerance || u < -kDefaultTolerance
       || u > 1.0 + kDefaultTolerance) {
        return {IntersectionType::None, {}};
    }

    return {IntersectionType::Point, {lhs.start() + r * t}};
}

Intersection2DResult intersect(const Line2D& line, const Circle2D& circle) {
    const Point2 foot = line.projection(circle.center());
    const double distanceToCenter = (foot - circle.center()).norm();

    if(distanceToCenter > circle.radius() + kDefaultTolerance) {
        return {IntersectionType::None, {}};
    }
    if(almostEqual(distanceToCenter, circle.radius())) {
        return {IntersectionType::Tangent, {foot}};
    }

    const double offset = std::sqrt(circle.radius() * circle.radius() - distanceToCenter * distanceToCenter);
    return {
        IntersectionType::Point,
        {foot - line.direction() * offset, foot + line.direction() * offset},
    };
}

Intersection2DResult intersect(const Circle2D& lhs, const Circle2D& rhs) {
    const Vec2 centerDelta = rhs.center() - lhs.center();
    const double centerDistance = centerDelta.norm();

    if(isZero(centerDistance) && almostEqual(lhs.radius(), rhs.radius())) {
        return {IntersectionType::Coincident, {}};
    }
    if(centerDistance > lhs.radius() + rhs.radius() + kDefaultTolerance) {
        return {IntersectionType::None, {}};
    }
    if(centerDistance < std::abs(lhs.radius() - rhs.radius()) - kDefaultTolerance) {
        return {IntersectionType::None, {}};
    }
    if(isZero(centerDistance)) {
        return {IntersectionType::None, {}};
    }

    const double a =
        (lhs.radius() * lhs.radius() - rhs.radius() * rhs.radius() + centerDistance * centerDistance)
        / (2.0 * centerDistance);
    const double heightSquared = lhs.radius() * lhs.radius() - a * a;
    const Vec2 direction = centerDelta / centerDistance;
    const Point2 base = lhs.center() + direction * a;

    if(isZero(heightSquared) || heightSquared < 0.0) {
        return {IntersectionType::Tangent, {base}};
    }

    const double height = std::sqrt(heightSquared);
    const Vec2 perpendicular(-direction.y(), direction.x());
    return {
        IntersectionType::Point,
        {base + perpendicular * height, base - perpendicular * height},
    };
}

Intersection2DResult intersect(const Segment2D& segment, const Circle2D& circle) {
    const Line2D line(segment.start(), segment.end() - segment.start());
    Intersection2DResult result = intersect(line, circle);
    if(result.points.empty()) {
        return result;
    }

    std::vector<Point2> filtered;
    for(const Point2& point : result.points) {
        if(pointOnSegment(point, segment, kDefaultTolerance)) {
            filtered.push_back(point);
        }
    }

    if(filtered.empty()) {
        return {IntersectionType::None, {}};
    }
    if(filtered.size() == 1) {
        return {result.type == IntersectionType::Tangent ? IntersectionType::Tangent : IntersectionType::Point, filtered};
    }
    return {IntersectionType::Point, filtered};
}

} // namespace mnx
