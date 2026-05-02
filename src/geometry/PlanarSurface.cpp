#include "mnx/geometry/PlanarSurface.h"

#include <stdexcept>

namespace mnx {

PlanarSurface::PlanarSurface(const Plane& plane)
    : origin_(plane.point()), uDirection_(plane.basisU()), vDirection_(plane.basisV()), normal_(plane.normal()) {}

PlanarSurface::PlanarSurface(const Point3& origin, const Vec3& uDirection, const Vec3& vDirection)
    : origin_(origin), uDirection_(uDirection), vDirection_(vDirection) {
    if(isZero(uDirection.squaredNorm()) || isZero(vDirection.squaredNorm())) {
        throw std::invalid_argument("PlanarSurface directions must be non-zero");
    }
    normal_ = uDirection.cross(vDirection).normalized();
    if(isZero(normal_.squaredNorm())) {
        throw std::invalid_argument("PlanarSurface directions must not be parallel");
    }
}

Point3 PlanarSurface::evaluate(double u, double v) const {
    return origin_ + uDirection_ * u + vDirection_ * v;
}

Vec3 PlanarSurface::normal(double, double) const {
    return normal_;
}

AABB3 PlanarSurface::bounds() const {
    return AABB3::empty();
}

} // namespace mnx
