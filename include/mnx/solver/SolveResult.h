#pragma once

#include "mnx/solver/SolverDiagnostics.h"

namespace mnx {

class SolveResult {
public:
    SolveResult() = default;
    SolveResult(bool converged, int iterationCount, double finalResidual, SolverDiagnostics diagnostics);

    [[nodiscard]] bool converged() const {
        return converged_;
    }

    [[nodiscard]] int iterationCount() const {
        return iterationCount_;
    }

    [[nodiscard]] double finalResidual() const {
        return finalResidual_;
    }

    [[nodiscard]] const SolverDiagnostics& diagnostics() const {
        return diagnostics_;
    }

private:
    bool converged_ = false;
    int iterationCount_ = 0;
    double finalResidual_ = 0.0;
    SolverDiagnostics diagnostics_;
};

} // namespace mnx
