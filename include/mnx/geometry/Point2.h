#pragma once

#include "mnx/core/math/Tolerance.h"
#include "mnx/core/math/Vec2.h"

namespace mnx {

class Point2 {
public:
    constexpr Point2() = default;
    constexpr Point2(double x, double y) : coordinates_(x, y) {}
    explicit constexpr Point2(const Vec2& coordinates) : coordinates_(coordinates) {}

    [[nodiscard]] constexpr double x() const {
        return coordinates_.x();
    }

    [[nodiscard]] constexpr double y() const {
        return coordinates_.y();
    }

    [[nodiscard]] constexpr const Vec2& vector() const {
        return coordinates_;
    }

    [[nodiscard]] bool isApprox(const Point2& rhs, double tolerance = kDefaultTolerance) const {
        return coordinates_.isApprox(rhs.coordinates_, tolerance);
    }

private:
    Vec2 coordinates_;
};

[[nodiscard]] constexpr Vec2 operator-(const Point2& lhs, const Point2& rhs) {
    return lhs.vector() - rhs.vector();
}

[[nodiscard]] constexpr Point2 operator+(const Point2& point, const Vec2& vector) {
    return Point2(point.vector() + vector);
}

[[nodiscard]] constexpr Point2 operator+(const Vec2& vector, const Point2& point) {
    return point + vector;
}

[[nodiscard]] constexpr Point2 operator-(const Point2& point, const Vec2& vector) {
    return Point2(point.vector() - vector);
}

} // namespace mnx
