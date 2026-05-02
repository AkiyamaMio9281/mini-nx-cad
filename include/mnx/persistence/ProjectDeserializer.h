#pragma once

#include "mnx/assembly/AssemblyDocument.h"
#include "mnx/core/utils/Result.h"
#include "mnx/modeling/PartDocument.h"

#include <string>

namespace mnx::persistence {

class ProjectDeserializer {
public:
    [[nodiscard]] Result<PartDocument> deserialize(const std::string& jsonText) const;
    [[nodiscard]] Result<AssemblyDocument> deserializeAssembly(const std::string& jsonText) const;
};

} // namespace mnx::persistence
