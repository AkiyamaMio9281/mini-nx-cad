#include "mnx/geometry/algorithms/Bounds2D.h"

namespace mnx {

AABB2 boundsOf(std::span<const Point2> points) {
    AABB2 box;
    for(const Point2& point : points) {
        box.expand(point.vector());
    }
    return box;
}

AABB2 boundsOf(const Segment2D& segment) {
    return segment.bounds();
}

AABB2 boundsOf(const Circle2D& circle) {
    return circle.bounds();
}

AABB2 boundsOf(const Arc2D& arc) {
    return arc.bounds();
}

AABB2 boundsOf(const Polyline2D& polyline) {
    return polyline.bounds();
}

} // namespace mnx
