#pragma once

#include "mnx/core/math/Tolerance.h"
#include "mnx/core/math/Vec3.h"

#include <array>
#include <cstddef>

namespace mnx {

class Mat4 {
public:
    using Storage = std::array<double, 16>;

    constexpr Mat4() = default;
    explicit constexpr Mat4(Storage values) : values_(values) {}

    [[nodiscard]] static constexpr Mat4 identity() {
        return Mat4(
            {1.0, 0.0, 0.0, 0.0,
             0.0, 1.0, 0.0, 0.0,
             0.0, 0.0, 1.0, 0.0,
             0.0, 0.0, 0.0, 1.0}
        );
    }

    [[nodiscard]] static constexpr Mat4 zero() {
        return Mat4(Storage {});
    }

    [[nodiscard]] static Mat4 translation(const Vec3& offset) {
        return Mat4(
            {1.0, 0.0, 0.0, offset.x(),
             0.0, 1.0, 0.0, offset.y(),
             0.0, 0.0, 1.0, offset.z(),
             0.0, 0.0, 0.0, 1.0}
        );
    }

    [[nodiscard]] static Mat4 scaling(double sx, double sy, double sz) {
        return Mat4(
            {sx, 0.0, 0.0, 0.0,
             0.0, sy, 0.0, 0.0,
             0.0, 0.0, sz, 0.0,
             0.0, 0.0, 0.0, 1.0}
        );
    }

    [[nodiscard]] static Mat4 rotationX(double radians);
    [[nodiscard]] static Mat4 rotationY(double radians);
    [[nodiscard]] static Mat4 rotationZ(double radians);

    [[nodiscard]] constexpr double operator()(std::size_t row, std::size_t column) const {
        return values_[row * 4 + column];
    }

    constexpr double& operator()(std::size_t row, std::size_t column) {
        return values_[row * 4 + column];
    }

    [[nodiscard]] constexpr const Storage& values() const {
        return values_;
    }

    [[nodiscard]] Mat4 operator*(const Mat4& rhs) const;
    [[nodiscard]] Vec3 transformPoint(const Vec3& point) const;
    [[nodiscard]] Vec3 transformVector(const Vec3& vector) const;
    [[nodiscard]] bool isApprox(const Mat4& rhs, double tolerance = kDefaultTolerance) const;

private:
    Storage values_ {};
};

} // namespace mnx
