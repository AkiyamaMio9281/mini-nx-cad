#pragma once

#include "mnx/assembly/AssemblyDocument.h"
#include "mnx/modeling/PartDocument.h"

#include <string>

namespace mnx::persistence {

class ProjectSerializer {
public:
    [[nodiscard]] std::string serialize(const PartDocument& document) const;
    [[nodiscard]] std::string serializeAssembly(const AssemblyDocument& assembly) const;
};

} // namespace mnx::persistence
