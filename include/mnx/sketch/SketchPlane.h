#pragma once

#include "mnx/geometry/Plane.h"
#include "mnx/geometry/Point2.h"
#include "mnx/geometry/Point3.h"

namespace mnx {

class SketchPlane {
public:
    SketchPlane();
    explicit SketchPlane(const Plane& plane);
    SketchPlane(const Point3& origin, const Vec3& uDirection, const Vec3& vDirection);

    [[nodiscard]] const Point3& origin() const {
        return origin_;
    }

    [[nodiscard]] const Vec3& uDirection() const {
        return uDirection_;
    }

    [[nodiscard]] const Vec3& vDirection() const {
        return vDirection_;
    }

    [[nodiscard]] const Vec3& normal() const {
        return normal_;
    }

    [[nodiscard]] Point3 toWorld(const Point2& point) const;
    [[nodiscard]] Point2 toSketch(const Point3& point) const;

private:
    Point3 origin_;
    Vec3 uDirection_ {1.0, 0.0, 0.0};
    Vec3 vDirection_ {0.0, 1.0, 0.0};
    Vec3 normal_ {0.0, 0.0, 1.0};
};

} // namespace mnx
