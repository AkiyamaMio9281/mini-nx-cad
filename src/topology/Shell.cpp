#include "mnx/topology/Shell.h"

#include <utility>

namespace mnx::topology {

Shell::Shell(std::vector<std::size_t> faceIndices) : faceIndices_(std::move(faceIndices)) {}

std::span<const std::size_t> Shell::faceIndices() const {
    return faceIndices_;
}

std::size_t Shell::faceCount() const {
    return faceIndices_.size();
}

} // namespace mnx::topology
