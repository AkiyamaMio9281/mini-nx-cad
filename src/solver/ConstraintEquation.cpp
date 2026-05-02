#include "mnx/solver/ConstraintEquation.h"

#include <utility>

namespace mnx {

ConstraintEquation::ConstraintEquation(std::string label, ResidualFunction residualFunction)
    : label_(std::move(label)), residualFunction_(std::move(residualFunction)) {}

std::vector<double> evaluateResiduals(
    std::span<const ConstraintEquation> equations,
    std::span<const double> values
) {
    std::vector<double> residuals;
    residuals.reserve(equations.size());
    for(const ConstraintEquation& equation : equations) {
        residuals.push_back(equation.evaluate(values));
    }
    return residuals;
}

} // namespace mnx
