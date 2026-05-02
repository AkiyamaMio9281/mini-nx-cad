#pragma once

#include "mnx/core/math/Tolerance.h"

#include <cmath>

namespace mnx {

class Vec2 {
public:
    constexpr Vec2() = default;
    constexpr Vec2(double x, double y) : x_(x), y_(y) {}

    [[nodiscard]] constexpr double x() const {
        return x_;
    }

    [[nodiscard]] constexpr double y() const {
        return y_;
    }

    constexpr void setX(double x) {
        x_ = x;
    }

    constexpr void setY(double y) {
        y_ = y;
    }

    [[nodiscard]] constexpr Vec2 operator+() const {
        return *this;
    }

    [[nodiscard]] constexpr Vec2 operator-() const {
        return {-x_, -y_};
    }

    [[nodiscard]] constexpr Vec2 operator+(const Vec2& rhs) const {
        return {x_ + rhs.x_, y_ + rhs.y_};
    }

    [[nodiscard]] constexpr Vec2 operator-(const Vec2& rhs) const {
        return {x_ - rhs.x_, y_ - rhs.y_};
    }

    [[nodiscard]] constexpr Vec2 operator*(double scalar) const {
        return {x_ * scalar, y_ * scalar};
    }

    [[nodiscard]] constexpr Vec2 operator/(double scalar) const {
        return {x_ / scalar, y_ / scalar};
    }

    constexpr Vec2& operator+=(const Vec2& rhs) {
        x_ += rhs.x_;
        y_ += rhs.y_;
        return *this;
    }

    constexpr Vec2& operator-=(const Vec2& rhs) {
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        return *this;
    }

    constexpr Vec2& operator*=(double scalar) {
        x_ *= scalar;
        y_ *= scalar;
        return *this;
    }

    constexpr Vec2& operator/=(double scalar) {
        x_ /= scalar;
        y_ /= scalar;
        return *this;
    }

    [[nodiscard]] constexpr double dot(const Vec2& rhs) const {
        return x_ * rhs.x_ + y_ * rhs.y_;
    }

    [[nodiscard]] constexpr double cross(const Vec2& rhs) const {
        return x_ * rhs.y_ - y_ * rhs.x_;
    }

    [[nodiscard]] constexpr double squaredNorm() const {
        return dot(*this);
    }

    [[nodiscard]] double norm() const {
        return std::sqrt(squaredNorm());
    }

    [[nodiscard]] Vec2 normalized(double tolerance = kDefaultTolerance) const {
        const double length = norm();
        if(isZero(length, tolerance)) {
            return {};
        }
        return *this / length;
    }

    [[nodiscard]] bool isApprox(const Vec2& rhs, double tolerance = kDefaultTolerance) const {
        return almostEqual(x_, rhs.x_, tolerance) && almostEqual(y_, rhs.y_, tolerance);
    }

private:
    double x_ = 0.0;
    double y_ = 0.0;
};

[[nodiscard]] constexpr Vec2 operator*(double scalar, const Vec2& vector) {
    return vector * scalar;
}

} // namespace mnx
