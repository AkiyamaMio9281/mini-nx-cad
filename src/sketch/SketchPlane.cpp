#include "mnx/sketch/SketchPlane.h"

#include <stdexcept>

namespace mnx {

SketchPlane::SketchPlane() = default;

SketchPlane::SketchPlane(const Plane& plane)
    : origin_(plane.point()), uDirection_(plane.basisU()), vDirection_(plane.basisV()), normal_(plane.normal()) {}

SketchPlane::SketchPlane(const Point3& origin, const Vec3& uDirection, const Vec3& vDirection)
    : origin_(origin), uDirection_(uDirection.normalized()), vDirection_(vDirection.normalized()) {
    if(isZero(uDirection.squaredNorm()) || isZero(vDirection.squaredNorm())) {
        throw std::invalid_argument("SketchPlane directions must be non-zero");
    }
    normal_ = uDirection_.cross(vDirection_).normalized();
    if(isZero(normal_.squaredNorm())) {
        throw std::invalid_argument("SketchPlane directions must not be parallel");
    }
    vDirection_ = normal_.cross(uDirection_).normalized();
}

Point3 SketchPlane::toWorld(const Point2& point) const {
    return origin_ + uDirection_ * point.x() + vDirection_ * point.y();
}

Point2 SketchPlane::toSketch(const Point3& point) const {
    const Vec3 delta = point - origin_;
    return Point2(delta.dot(uDirection_), delta.dot(vDirection_));
}

} // namespace mnx
