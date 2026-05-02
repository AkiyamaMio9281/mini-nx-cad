#include "mnx/core/math/Transform2D.h"

#include <stdexcept>

namespace mnx {

Transform2D Transform2D::inverse(double tolerance) const {
    const double a = matrix_(0, 0);
    const double b = matrix_(0, 1);
    const double c = matrix_(0, 2);
    const double d = matrix_(1, 0);
    const double e = matrix_(1, 1);
    const double f = matrix_(1, 2);

    const double determinant = a * e - b * d;
    if(isZero(determinant, tolerance)) {
        throw std::runtime_error("Transform2D is not invertible");
    }

    const double inverseDeterminant = 1.0 / determinant;
    return Transform2D(Mat3({
        e * inverseDeterminant,
        -b * inverseDeterminant,
        (b * f - e * c) * inverseDeterminant,
        -d * inverseDeterminant,
        a * inverseDeterminant,
        (d * c - a * f) * inverseDeterminant,
        0.0,
        0.0,
        1.0,
    }));
}

} // namespace mnx
