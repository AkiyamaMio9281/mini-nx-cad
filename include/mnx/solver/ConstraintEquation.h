#pragma once

#include <functional>
#include <span>
#include <string>
#include <vector>

namespace mnx {

class ConstraintEquation {
public:
    using ResidualFunction = std::function<double(std::span<const double>)>;

    ConstraintEquation(std::string label, ResidualFunction residualFunction);

    [[nodiscard]] const std::string& label() const {
        return label_;
    }

    [[nodiscard]] double evaluate(std::span<const double> values) const {
        return residualFunction_(values);
    }

private:
    std::string label_;
    ResidualFunction residualFunction_;
};

[[nodiscard]] std::vector<double> evaluateResiduals(
    std::span<const ConstraintEquation> equations,
    std::span<const double> values
);

} // namespace mnx
