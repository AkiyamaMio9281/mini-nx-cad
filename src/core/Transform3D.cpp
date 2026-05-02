#include "mnx/core/math/Transform3D.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <stdexcept>

namespace mnx {

Transform3D Transform3D::inverse(double tolerance) const {
    std::array<std::array<double, 8>, 4> augmented {};
    for(std::size_t row = 0; row < 4; ++row) {
        for(std::size_t column = 0; column < 4; ++column) {
            augmented[row][column] = matrix_(row, column);
            augmented[row][column + 4] = row == column ? 1.0 : 0.0;
        }
    }

    for(std::size_t pivotColumn = 0; pivotColumn < 4; ++pivotColumn) {
        std::size_t pivotRow = pivotColumn;
        for(std::size_t row = pivotColumn + 1; row < 4; ++row) {
            if(std::abs(augmented[row][pivotColumn]) > std::abs(augmented[pivotRow][pivotColumn])) {
                pivotRow = row;
            }
        }

        if(isZero(augmented[pivotRow][pivotColumn], tolerance)) {
            throw std::runtime_error("Transform3D is not invertible");
        }

        if(pivotRow != pivotColumn) {
            std::swap(augmented[pivotRow], augmented[pivotColumn]);
        }

        const double pivot = augmented[pivotColumn][pivotColumn];
        for(double& value : augmented[pivotColumn]) {
            value /= pivot;
        }

        for(std::size_t row = 0; row < 4; ++row) {
            if(row == pivotColumn) {
                continue;
            }

            const double factor = augmented[row][pivotColumn];
            for(std::size_t column = 0; column < 8; ++column) {
                augmented[row][column] -= factor * augmented[pivotColumn][column];
            }
        }
    }

    Mat4 inverse = Mat4::zero();
    for(std::size_t row = 0; row < 4; ++row) {
        for(std::size_t column = 0; column < 4; ++column) {
            inverse(row, column) = augmented[row][column + 4];
        }
    }

    return Transform3D(inverse);
}

} // namespace mnx
