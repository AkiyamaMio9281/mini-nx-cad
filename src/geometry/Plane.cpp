#include "mnx/geometry/Plane.h"

#include <stdexcept>

namespace mnx {

Plane::Plane() = default;

Plane::Plane(const Point3& point, const Vec3& normal) : point_(point) {
    if(isZero(normal.squaredNorm())) {
        throw std::invalid_argument("Plane normal must be non-zero");
    }
    normal_ = normal.normalized();
}

double Plane::signedDistance(const Point3& point) const {
    return (point - point_).dot(normal_);
}

Point3 Plane::projection(const Point3& point) const {
    return point - normal_ * signedDistance(point);
}

Vec3 Plane::basisU() const {
    const Vec3 reference = std::abs(normal_.z()) < 0.9 ? Vec3(0.0, 0.0, 1.0) : Vec3(1.0, 0.0, 0.0);
    return reference.cross(normal_).normalized();
}

Vec3 Plane::basisV() const {
    return normal_.cross(basisU()).normalized();
}

} // namespace mnx
