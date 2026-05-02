#pragma once

#include "mnx/core/math/Tolerance.h"
#include "mnx/core/math/Vec2.h"

#include <array>
#include <cstddef>

namespace mnx {

class Mat3 {
public:
    using Storage = std::array<double, 9>;

    constexpr Mat3() = default;
    explicit constexpr Mat3(Storage values) : values_(values) {}

    [[nodiscard]] static constexpr Mat3 identity() {
        return Mat3({1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0});
    }

    [[nodiscard]] static constexpr Mat3 zero() {
        return Mat3(Storage {});
    }

    [[nodiscard]] static Mat3 translation(const Vec2& offset) {
        return Mat3({1.0, 0.0, offset.x(), 0.0, 1.0, offset.y(), 0.0, 0.0, 1.0});
    }

    [[nodiscard]] static Mat3 scaling(double sx, double sy) {
        return Mat3({sx, 0.0, 0.0, 0.0, sy, 0.0, 0.0, 0.0, 1.0});
    }

    [[nodiscard]] static Mat3 rotation(double radians);

    [[nodiscard]] constexpr double operator()(std::size_t row, std::size_t column) const {
        return values_[row * 3 + column];
    }

    constexpr double& operator()(std::size_t row, std::size_t column) {
        return values_[row * 3 + column];
    }

    [[nodiscard]] constexpr const Storage& values() const {
        return values_;
    }

    [[nodiscard]] Mat3 operator*(const Mat3& rhs) const;
    [[nodiscard]] Vec2 transformPoint(const Vec2& point) const;
    [[nodiscard]] Vec2 transformVector(const Vec2& vector) const;
    [[nodiscard]] bool isApprox(const Mat3& rhs, double tolerance = kDefaultTolerance) const;

private:
    Storage values_ {};
};

} // namespace mnx
