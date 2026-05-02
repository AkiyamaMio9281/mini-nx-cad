#include "mnx/geometry/Segment2D.h"

#include <algorithm>

namespace mnx {

double Segment2D::length() const {
    return vector().norm();
}

Point2 Segment2D::midpoint() const {
    return Point2((start_.vector() + end_.vector()) * 0.5);
}

AABB2 Segment2D::bounds() const {
    AABB2 box;
    box.expand(start_.vector());
    box.expand(end_.vector());
    return box;
}

Point2 Segment2D::projection(const Point2& point) const {
    const Vec2 segmentVector = vector();
    const double squaredLength = segmentVector.squaredNorm();
    if(isZero(squaredLength)) {
        return start_;
    }

    const double t = std::clamp((point - start_).dot(segmentVector) / squaredLength, 0.0, 1.0);
    return start_ + segmentVector * t;
}

} // namespace mnx
