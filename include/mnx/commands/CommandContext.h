#pragma once

#include "mnx/constraints/Constraint.h"
#include "mnx/constraints/Dimension.h"

#include <memory>
#include <vector>

namespace mnx {

struct CommandContext {
    std::vector<std::unique_ptr<Constraint>> constraints;
    std::vector<Dimension> dimensions;
};

} // namespace mnx
