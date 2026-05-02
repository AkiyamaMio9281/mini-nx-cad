#include "mnx/geometry/algorithms/Projections2D.h"

namespace mnx {

Point2 projectPoint(const Point2& point, const Line2D& line) {
    return line.projection(point);
}

Point2 projectPoint(const Point2& point, const Segment2D& segment) {
    return segment.projection(point);
}

Point2 projectPoint(const Point2& point, const Circle2D& circle) {
    const Vec2 radial = point - circle.center();
    if(isZero(radial.squaredNorm())) {
        return circle.evaluate(0.0);
    }
    return circle.center() + radial.normalized() * circle.radius();
}

} // namespace mnx
