#pragma once

#include "mnx/assembly/AssemblyDocument.h"
#include "mnx/core/utils/Result.h"
#include "mnx/modeling/PartDocument.h"

#include <filesystem>

namespace mnx::persistence {

class PersistenceUtils {
public:
    [[nodiscard]] static Status saveProject(const PartDocument& document, const std::filesystem::path& path);
    [[nodiscard]] static Result<PartDocument> loadProject(const std::filesystem::path& path);
    [[nodiscard]] static Status saveAssembly(const AssemblyDocument& assembly, const std::filesystem::path& path);
    [[nodiscard]] static Result<AssemblyDocument> loadAssembly(const std::filesystem::path& path);
};

} // namespace mnx::persistence
