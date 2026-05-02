#pragma once

#include "mnx/core/math/Tolerance.h"
#include "mnx/core/math/Vec3.h"

namespace mnx {

class Point3 {
public:
    constexpr Point3() = default;
    constexpr Point3(double x, double y, double z) : coordinates_(x, y, z) {}
    explicit constexpr Point3(const Vec3& coordinates) : coordinates_(coordinates) {}

    [[nodiscard]] constexpr double x() const {
        return coordinates_.x();
    }

    [[nodiscard]] constexpr double y() const {
        return coordinates_.y();
    }

    [[nodiscard]] constexpr double z() const {
        return coordinates_.z();
    }

    [[nodiscard]] constexpr const Vec3& vector() const {
        return coordinates_;
    }

    [[nodiscard]] bool isApprox(const Point3& rhs, double tolerance = kDefaultTolerance) const {
        return coordinates_.isApprox(rhs.coordinates_, tolerance);
    }

private:
    Vec3 coordinates_;
};

[[nodiscard]] constexpr Vec3 operator-(const Point3& lhs, const Point3& rhs) {
    return lhs.vector() - rhs.vector();
}

[[nodiscard]] constexpr Point3 operator+(const Point3& point, const Vec3& vector) {
    return Point3(point.vector() + vector);
}

[[nodiscard]] constexpr Point3 operator+(const Vec3& vector, const Point3& point) {
    return point + vector;
}

[[nodiscard]] constexpr Point3 operator-(const Point3& point, const Vec3& vector) {
    return Point3(point.vector() - vector);
}

} // namespace mnx
