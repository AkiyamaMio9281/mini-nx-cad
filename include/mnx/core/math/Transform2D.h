#pragma once

#include "mnx/core/math/Mat3.h"
#include "mnx/core/math/Vec2.h"

namespace mnx {

class Transform2D {
public:
    constexpr Transform2D() : matrix_(Mat3::identity()) {}
    explicit constexpr Transform2D(const Mat3& matrix) : matrix_(matrix) {}

    [[nodiscard]] static constexpr Transform2D identity() {
        return Transform2D();
    }

    [[nodiscard]] static Transform2D translation(const Vec2& offset) {
        return Transform2D(Mat3::translation(offset));
    }

    [[nodiscard]] static Transform2D rotation(double radians) {
        return Transform2D(Mat3::rotation(radians));
    }

    [[nodiscard]] static Transform2D scaling(double sx, double sy) {
        return Transform2D(Mat3::scaling(sx, sy));
    }

    [[nodiscard]] const Mat3& matrix() const {
        return matrix_;
    }

    [[nodiscard]] Transform2D operator*(const Transform2D& rhs) const {
        return Transform2D(matrix_ * rhs.matrix_);
    }

    [[nodiscard]] Vec2 applyToPoint(const Vec2& point) const {
        return matrix_.transformPoint(point);
    }

    [[nodiscard]] Vec2 applyToVector(const Vec2& vector) const {
        return matrix_.transformVector(vector);
    }

    [[nodiscard]] Transform2D inverse(double tolerance = kDefaultTolerance) const;

private:
    Mat3 matrix_;
};

} // namespace mnx
