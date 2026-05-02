#pragma once

#include "mnx/core/math/Tolerance.h"

#include <cmath>

namespace mnx {

class Vec3 {
public:
    constexpr Vec3() = default;
    constexpr Vec3(double x, double y, double z) : x_(x), y_(y), z_(z) {}

    [[nodiscard]] constexpr double x() const {
        return x_;
    }

    [[nodiscard]] constexpr double y() const {
        return y_;
    }

    [[nodiscard]] constexpr double z() const {
        return z_;
    }

    constexpr void setX(double x) {
        x_ = x;
    }

    constexpr void setY(double y) {
        y_ = y;
    }

    constexpr void setZ(double z) {
        z_ = z;
    }

    [[nodiscard]] constexpr Vec3 operator+() const {
        return *this;
    }

    [[nodiscard]] constexpr Vec3 operator-() const {
        return {-x_, -y_, -z_};
    }

    [[nodiscard]] constexpr Vec3 operator+(const Vec3& rhs) const {
        return {x_ + rhs.x_, y_ + rhs.y_, z_ + rhs.z_};
    }

    [[nodiscard]] constexpr Vec3 operator-(const Vec3& rhs) const {
        return {x_ - rhs.x_, y_ - rhs.y_, z_ - rhs.z_};
    }

    [[nodiscard]] constexpr Vec3 operator*(double scalar) const {
        return {x_ * scalar, y_ * scalar, z_ * scalar};
    }

    [[nodiscard]] constexpr Vec3 operator/(double scalar) const {
        return {x_ / scalar, y_ / scalar, z_ / scalar};
    }

    constexpr Vec3& operator+=(const Vec3& rhs) {
        x_ += rhs.x_;
        y_ += rhs.y_;
        z_ += rhs.z_;
        return *this;
    }

    constexpr Vec3& operator-=(const Vec3& rhs) {
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        z_ -= rhs.z_;
        return *this;
    }

    constexpr Vec3& operator*=(double scalar) {
        x_ *= scalar;
        y_ *= scalar;
        z_ *= scalar;
        return *this;
    }

    constexpr Vec3& operator/=(double scalar) {
        x_ /= scalar;
        y_ /= scalar;
        z_ /= scalar;
        return *this;
    }

    [[nodiscard]] constexpr double dot(const Vec3& rhs) const {
        return x_ * rhs.x_ + y_ * rhs.y_ + z_ * rhs.z_;
    }

    [[nodiscard]] constexpr Vec3 cross(const Vec3& rhs) const {
        return {
            y_ * rhs.z_ - z_ * rhs.y_,
            z_ * rhs.x_ - x_ * rhs.z_,
            x_ * rhs.y_ - y_ * rhs.x_,
        };
    }

    [[nodiscard]] constexpr double squaredNorm() const {
        return dot(*this);
    }

    [[nodiscard]] double norm() const {
        return std::sqrt(squaredNorm());
    }

    [[nodiscard]] Vec3 normalized(double tolerance = kDefaultTolerance) const {
        const double length = norm();
        if(isZero(length, tolerance)) {
            return {};
        }
        return *this / length;
    }

    [[nodiscard]] bool isApprox(const Vec3& rhs, double tolerance = kDefaultTolerance) const {
        return almostEqual(x_, rhs.x_, tolerance) && almostEqual(y_, rhs.y_, tolerance)
            && almostEqual(z_, rhs.z_, tolerance);
    }

private:
    double x_ = 0.0;
    double y_ = 0.0;
    double z_ = 0.0;
};

[[nodiscard]] constexpr Vec3 operator*(double scalar, const Vec3& vector) {
    return vector * scalar;
}

} // namespace mnx
