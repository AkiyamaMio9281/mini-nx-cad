#include "mnx/core/math/Mat4.h"

#include <cmath>

namespace mnx {

Mat4 Mat4::rotationX(double radians) {
    const double cosine = std::cos(radians);
    const double sine = std::sin(radians);
    return Mat4(
        {1.0, 0.0, 0.0, 0.0,
         0.0, cosine, -sine, 0.0,
         0.0, sine, cosine, 0.0,
         0.0, 0.0, 0.0, 1.0}
    );
}

Mat4 Mat4::rotationY(double radians) {
    const double cosine = std::cos(radians);
    const double sine = std::sin(radians);
    return Mat4(
        {cosine, 0.0, sine, 0.0,
         0.0, 1.0, 0.0, 0.0,
         -sine, 0.0, cosine, 0.0,
         0.0, 0.0, 0.0, 1.0}
    );
}

Mat4 Mat4::rotationZ(double radians) {
    const double cosine = std::cos(radians);
    const double sine = std::sin(radians);
    return Mat4(
        {cosine, -sine, 0.0, 0.0,
         sine, cosine, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 0.0, 1.0}
    );
}

Mat4 Mat4::operator*(const Mat4& rhs) const {
    Mat4 result = Mat4::zero();
    for(std::size_t row = 0; row < 4; ++row) {
        for(std::size_t column = 0; column < 4; ++column) {
            for(std::size_t inner = 0; inner < 4; ++inner) {
                result(row, column) += (*this)(row, inner) * rhs(inner, column);
            }
        }
    }
    return result;
}

Vec3 Mat4::transformPoint(const Vec3& point) const {
    const double x = (*this)(0, 0) * point.x() + (*this)(0, 1) * point.y()
        + (*this)(0, 2) * point.z() + (*this)(0, 3);
    const double y = (*this)(1, 0) * point.x() + (*this)(1, 1) * point.y()
        + (*this)(1, 2) * point.z() + (*this)(1, 3);
    const double z = (*this)(2, 0) * point.x() + (*this)(2, 1) * point.y()
        + (*this)(2, 2) * point.z() + (*this)(2, 3);
    const double w = (*this)(3, 0) * point.x() + (*this)(3, 1) * point.y()
        + (*this)(3, 2) * point.z() + (*this)(3, 3);
    if(isZero(w)) {
        return {x, y, z};
    }
    return {x / w, y / w, z / w};
}

Vec3 Mat4::transformVector(const Vec3& vector) const {
    return {
        (*this)(0, 0) * vector.x() + (*this)(0, 1) * vector.y() + (*this)(0, 2) * vector.z(),
        (*this)(1, 0) * vector.x() + (*this)(1, 1) * vector.y() + (*this)(1, 2) * vector.z(),
        (*this)(2, 0) * vector.x() + (*this)(2, 1) * vector.y() + (*this)(2, 2) * vector.z(),
    };
}

bool Mat4::isApprox(const Mat4& rhs, double tolerance) const {
    for(std::size_t index = 0; index < values_.size(); ++index) {
        if(!almostEqual(values_[index], rhs.values_[index], tolerance)) {
            return false;
        }
    }
    return true;
}

} // namespace mnx
