#include "mnx/solver/SolveResult.h"

#include <utility>

namespace mnx {

SolveResult::SolveResult(bool converged, int iterationCount, double finalResidual, SolverDiagnostics diagnostics)
    : converged_(converged),
      iterationCount_(iterationCount),
      finalResidual_(finalResidual),
      diagnostics_(std::move(diagnostics)) {}

} // namespace mnx
