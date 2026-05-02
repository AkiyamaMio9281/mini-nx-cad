#pragma once

#include "mnx/assembly/AssemblyDocument.h"
#include "mnx/core/math/AABB3.h"

#include <vector>

namespace mnx {

class AssemblyQueries {
public:
    [[nodiscard]] static std::vector<ComponentId> rootComponents(const AssemblyDocument& assembly);
    [[nodiscard]] static std::vector<ComponentId> childComponents(const AssemblyDocument& assembly, ComponentId parentId);
    [[nodiscard]] static AABB3 bounds(const AssemblyDocument& assembly);
};

} // namespace mnx
