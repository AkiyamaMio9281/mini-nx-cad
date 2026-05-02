#include "mnx/solver/SolverDiagnostics.h"

#include <algorithm>
#include <utility>

namespace mnx {

void SolverDiagnostics::addWarning(SolverWarning warning, std::string message) {
    warnings_.push_back(warning);
    messages_.push_back(std::move(message));
}

bool SolverDiagnostics::hasWarning(SolverWarning warning) const {
    return std::find(warnings_.begin(), warnings_.end(), warning) != warnings_.end();
}

} // namespace mnx
