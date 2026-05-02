#pragma once

#include "mnx/constraints/Constraint.h"
#include "mnx/solver/ConstraintEquation.h"
#include "mnx/solver/SolverDiagnostics.h"
#include "mnx/solver/VariableRegistry.h"

#include <span>

namespace mnx {

class EquationBuilder {
public:
    [[nodiscard]] std::vector<ConstraintEquation> build(
        const Sketch& sketch,
        const VariableRegistry& registry,
        std::span<const Constraint*> constraints,
        SolverDiagnostics& diagnostics
    ) const;
};

} // namespace mnx
