#pragma once

#include "mnx/assembly/AssemblyDocument.h"

#include <string>
#include <vector>

namespace mnx {

struct AssemblySolveResult {
    Status status;
    std::vector<ConstraintId> appliedMateIds;
    std::vector<std::string> diagnostics;

    [[nodiscard]] bool isOk() const {
        return status.isOk();
    }
};

class AssemblySolver {
public:
    [[nodiscard]] AssemblySolveResult solve(AssemblyDocument& assembly) const;

private:
    [[nodiscard]] Status applyMate(AssemblyDocument& assembly, const MateConstraint& mate) const;
};

} // namespace mnx
