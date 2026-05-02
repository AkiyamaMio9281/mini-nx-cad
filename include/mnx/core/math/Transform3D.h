#pragma once

#include "mnx/core/math/Mat4.h"
#include "mnx/core/math/Vec3.h"

namespace mnx {

class Transform3D {
public:
    constexpr Transform3D() : matrix_(Mat4::identity()) {}
    explicit constexpr Transform3D(const Mat4& matrix) : matrix_(matrix) {}

    [[nodiscard]] static constexpr Transform3D identity() {
        return Transform3D();
    }

    [[nodiscard]] static Transform3D translation(const Vec3& offset) {
        return Transform3D(Mat4::translation(offset));
    }

    [[nodiscard]] static Transform3D scaling(double sx, double sy, double sz) {
        return Transform3D(Mat4::scaling(sx, sy, sz));
    }

    [[nodiscard]] static Transform3D rotationX(double radians) {
        return Transform3D(Mat4::rotationX(radians));
    }

    [[nodiscard]] static Transform3D rotationY(double radians) {
        return Transform3D(Mat4::rotationY(radians));
    }

    [[nodiscard]] static Transform3D rotationZ(double radians) {
        return Transform3D(Mat4::rotationZ(radians));
    }

    [[nodiscard]] const Mat4& matrix() const {
        return matrix_;
    }

    [[nodiscard]] Transform3D operator*(const Transform3D& rhs) const {
        return Transform3D(matrix_ * rhs.matrix_);
    }

    [[nodiscard]] Vec3 applyToPoint(const Vec3& point) const {
        return matrix_.transformPoint(point);
    }

    [[nodiscard]] Vec3 applyToVector(const Vec3& vector) const {
        return matrix_.transformVector(vector);
    }

    [[nodiscard]] Transform3D inverse(double tolerance = kDefaultTolerance) const;

private:
    Mat4 matrix_;
};

} // namespace mnx
