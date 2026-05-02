#pragma once

#include <algorithm>
#include <cmath>

namespace mnx {

inline constexpr double kDefaultTolerance = 1.0e-9;
inline constexpr double kLooseTolerance = 1.0e-6;
inline constexpr double kAngularTolerance = 1.0e-10;

[[nodiscard]] inline bool almostEqual(
    double lhs,
    double rhs,
    double absoluteTolerance = kDefaultTolerance,
    double relativeTolerance = kDefaultTolerance
) {
    const double difference = std::abs(lhs - rhs);
    if(difference <= absoluteTolerance) {
        return true;
    }

    const double scale = std::max({1.0, std::abs(lhs), std::abs(rhs)});
    return difference <= relativeTolerance * scale;
}

[[nodiscard]] inline bool isZero(double value, double tolerance = kDefaultTolerance) {
    return std::abs(value) <= tolerance;
}

} // namespace mnx
