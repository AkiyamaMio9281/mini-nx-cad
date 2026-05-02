#include "mnx/geometry/algorithms/Distances2D.h"

#include <cmath>

namespace mnx {

double distance(const Point2& lhs, const Point2& rhs) {
    return (lhs - rhs).norm();
}

double distance(const Point2& point, const Line2D& line) {
    return line.distanceTo(point);
}

double distance(const Point2& point, const Segment2D& segment) {
    return distance(point, segment.projection(point));
}

double distance(const Point2& point, const Circle2D& circle) {
    return std::abs(distance(point, circle.center()) - circle.radius());
}

} // namespace mnx
