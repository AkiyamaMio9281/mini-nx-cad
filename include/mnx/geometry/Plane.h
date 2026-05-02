#pragma once

#include "mnx/geometry/Point3.h"

namespace mnx {

class Plane {
public:
    Plane();
    Plane(const Point3& point, const Vec3& normal);

    [[nodiscard]] const Point3& point() const {
        return point_;
    }

    [[nodiscard]] const Vec3& normal() const {
        return normal_;
    }

    [[nodiscard]] double signedDistance(const Point3& point) const;
    [[nodiscard]] Point3 projection(const Point3& point) const;
    [[nodiscard]] Vec3 basisU() const;
    [[nodiscard]] Vec3 basisV() const;

private:
    Point3 point_;
    Vec3 normal_ {0.0, 0.0, 1.0};
};

} // namespace mnx
