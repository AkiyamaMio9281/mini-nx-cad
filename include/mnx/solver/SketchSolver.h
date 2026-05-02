#pragma once

#include "mnx/constraints/Constraint.h"
#include "mnx/sketch/Sketch.h"
#include "mnx/solver/EquationBuilder.h"
#include "mnx/solver/JacobianBuilder.h"
#include "mnx/solver/SolveResult.h"

#include <span>

namespace mnx {

struct SketchSolverOptions {
    int maxIterations = 80;
    double residualTolerance = 1.0e-7;
    double stepTolerance = 1.0e-9;
    double damping = 1.0e-8;
};

class SketchSolver {
public:
    explicit SketchSolver(SketchSolverOptions options = {});

    [[nodiscard]] SolveResult solve(Sketch& sketch, std::span<const Constraint*> constraints) const;

private:
    [[nodiscard]] std::vector<double> solveNormalEquations(
        const Matrix& jacobian,
        std::span<const double> residuals,
        double damping,
        SolverDiagnostics& diagnostics
    ) const;

    [[nodiscard]] static double residualNorm(std::span<const double> residuals);

    SketchSolverOptions options_;
    EquationBuilder equationBuilder_;
    JacobianBuilder jacobianBuilder_;
};

} // namespace mnx
