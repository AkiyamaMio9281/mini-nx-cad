#include "mnx/persistence/PersistenceUtils.h"

#include "mnx/persistence/ProjectDeserializer.h"
#include "mnx/persistence/ProjectSerializer.h"

#include <fstream>
#include <sstream>

namespace mnx::persistence {

Status PersistenceUtils::saveProject(const PartDocument& document, const std::filesystem::path& path) {
    std::ofstream file(path);
    if(!file) {
        return Status::failedPrecondition("Could not open project file for writing");
    }
    file << ProjectSerializer().serialize(document);
    return file ? Status::ok() : Status::internalError("Failed while writing project file");
}

Result<PartDocument> PersistenceUtils::loadProject(const std::filesystem::path& path) {
    std::ifstream file(path);
    if(!file) {
        return Result<PartDocument>::failure(Status::notFound("Could not open project file for reading"));
    }
    std::ostringstream stream;
    stream << file.rdbuf();
    return ProjectDeserializer().deserialize(stream.str());
}

Status PersistenceUtils::saveAssembly(const AssemblyDocument& assembly, const std::filesystem::path& path) {
    std::ofstream file(path);
    if(!file) {
        return Status::failedPrecondition("Could not open assembly file for writing");
    }
    file << ProjectSerializer().serializeAssembly(assembly);
    return Status::ok();
}

Result<AssemblyDocument> PersistenceUtils::loadAssembly(const std::filesystem::path& path) {
    std::ifstream file(path);
    if(!file) {
        return Result<AssemblyDocument>::failure(Status::notFound("Could not open assembly file for reading"));
    }
    std::ostringstream stream;
    stream << file.rdbuf();
    return ProjectDeserializer().deserializeAssembly(stream.str());
}

} // namespace mnx::persistence
