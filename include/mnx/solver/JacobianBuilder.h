#pragma once

#include "mnx/solver/ConstraintEquation.h"

#include <span>
#include <vector>

namespace mnx {

using Matrix = std::vector<std::vector<double>>;

class JacobianBuilder {
public:
    [[nodiscard]] Matrix build(
        std::span<const ConstraintEquation> equations,
        std::span<const double> values,
        double step = 1.0e-6
    ) const;
};

} // namespace mnx
