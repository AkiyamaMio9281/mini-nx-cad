#pragma once

#include "mnx/core/utils/Status.h"
#include "mnx/topology/Body.h"

namespace mnx::topology {

class TopologyValidator {
public:
    [[nodiscard]] Status validate(const Body& body) const;
};

} // namespace mnx::topology
