#pragma once

#include "mnx/geometry/Plane.h"
#include "mnx/geometry/Surface.h"

namespace mnx {

class PlanarSurface final : public Surface {
public:
    explicit PlanarSurface(const Plane& plane);
    PlanarSurface(const Point3& origin, const Vec3& uDirection, const Vec3& vDirection);

    [[nodiscard]] const Point3& origin() const {
        return origin_;
    }

    [[nodiscard]] const Vec3& uDirection() const {
        return uDirection_;
    }

    [[nodiscard]] const Vec3& vDirection() const {
        return vDirection_;
    }

    [[nodiscard]] Point3 evaluate(double u, double v) const override;
    [[nodiscard]] Vec3 normal(double u, double v) const override;
    [[nodiscard]] AABB3 bounds() const override;

private:
    Point3 origin_;
    Vec3 uDirection_ {1.0, 0.0, 0.0};
    Vec3 vDirection_ {0.0, 1.0, 0.0};
    Vec3 normal_ {0.0, 0.0, 1.0};
};

} // namespace mnx
