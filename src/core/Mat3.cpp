#include "mnx/core/math/Mat3.h"

#include <cmath>

namespace mnx {

Mat3 Mat3::rotation(double radians) {
    const double cosine = std::cos(radians);
    const double sine = std::sin(radians);
    return Mat3({cosine, -sine, 0.0, sine, cosine, 0.0, 0.0, 0.0, 1.0});
}

Mat3 Mat3::operator*(const Mat3& rhs) const {
    Mat3 result = Mat3::zero();
    for(std::size_t row = 0; row < 3; ++row) {
        for(std::size_t column = 0; column < 3; ++column) {
            for(std::size_t inner = 0; inner < 3; ++inner) {
                result(row, column) += (*this)(row, inner) * rhs(inner, column);
            }
        }
    }
    return result;
}

Vec2 Mat3::transformPoint(const Vec2& point) const {
    const double x = (*this)(0, 0) * point.x() + (*this)(0, 1) * point.y() + (*this)(0, 2);
    const double y = (*this)(1, 0) * point.x() + (*this)(1, 1) * point.y() + (*this)(1, 2);
    const double w = (*this)(2, 0) * point.x() + (*this)(2, 1) * point.y() + (*this)(2, 2);
    if(isZero(w)) {
        return {x, y};
    }
    return {x / w, y / w};
}

Vec2 Mat3::transformVector(const Vec2& vector) const {
    return {
        (*this)(0, 0) * vector.x() + (*this)(0, 1) * vector.y(),
        (*this)(1, 0) * vector.x() + (*this)(1, 1) * vector.y(),
    };
}

bool Mat3::isApprox(const Mat3& rhs, double tolerance) const {
    for(std::size_t index = 0; index < values_.size(); ++index) {
        if(!almostEqual(values_[index], rhs.values_[index], tolerance)) {
            return false;
        }
    }
    return true;
}

} // namespace mnx
