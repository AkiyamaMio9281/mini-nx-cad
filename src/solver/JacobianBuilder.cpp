#include "mnx/solver/JacobianBuilder.h"

namespace mnx {

Matrix JacobianBuilder::build(
    std::span<const ConstraintEquation> equations,
    std::span<const double> values,
    double step
) const {
    Matrix jacobian(equations.size(), std::vector<double>(values.size(), 0.0));
    std::vector<double> plus(values.begin(), values.end());
    std::vector<double> minus(values.begin(), values.end());

    for(std::size_t variableIndex = 0; variableIndex < values.size(); ++variableIndex) {
        plus[variableIndex] = values[variableIndex] + step;
        minus[variableIndex] = values[variableIndex] - step;

        for(std::size_t equationIndex = 0; equationIndex < equations.size(); ++equationIndex) {
            const double forward = equations[equationIndex].evaluate(plus);
            const double backward = equations[equationIndex].evaluate(minus);
            jacobian[equationIndex][variableIndex] = (forward - backward) / (2.0 * step);
        }

        plus[variableIndex] = values[variableIndex];
        minus[variableIndex] = values[variableIndex];
    }

    return jacobian;
}

} // namespace mnx
