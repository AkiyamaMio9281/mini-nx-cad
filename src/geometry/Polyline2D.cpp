#include "mnx/geometry/Polyline2D.h"

#include <algorithm>
#include <stdexcept>

namespace mnx {

Polyline2D::Polyline2D(std::vector<Point2> points) : points_(std::move(points)) {}

bool Polyline2D::isClosed(double tolerance) const {
    return points_.size() > 1 && points_.front().isApprox(points_.back(), tolerance);
}

double Polyline2D::length() const {
    double total = 0.0;
    for(std::size_t index = 1; index < points_.size(); ++index) {
        total += (points_[index] - points_[index - 1]).norm();
    }
    return total;
}

AABB2 Polyline2D::bounds() const {
    AABB2 box;
    for(const Point2& point : points_) {
        box.expand(point.vector());
    }
    return box;
}

Point2 Polyline2D::evaluate(double t) const {
    if(points_.empty()) {
        throw std::runtime_error("Cannot evaluate an empty Polyline2D");
    }
    if(points_.size() == 1) {
        return points_.front();
    }

    const double clampedT = std::clamp(t, 0.0, 1.0);
    const double targetLength = length() * clampedT;
    double accumulated = 0.0;

    for(std::size_t index = 1; index < points_.size(); ++index) {
        const Vec2 edge = points_[index] - points_[index - 1];
        const double edgeLength = edge.norm();
        if(isZero(edgeLength)) {
            continue;
        }
        if(accumulated + edgeLength >= targetLength) {
            const double localT = (targetLength - accumulated) / edgeLength;
            return points_[index - 1] + edge * localT;
        }
        accumulated += edgeLength;
    }

    return points_.back();
}

} // namespace mnx
