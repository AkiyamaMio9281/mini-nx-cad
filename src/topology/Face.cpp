#include "mnx/topology/Face.h"

#include <utility>

namespace mnx::topology {

Face::Face(std::vector<std::size_t> loopIndices) : loopIndices_(std::move(loopIndices)) {}

std::span<const std::size_t> Face::loopIndices() const {
    return loopIndices_;
}

std::size_t Face::outerLoopIndex() const {
    return loopIndices_.empty() ? 0 : loopIndices_.front();
}

} // namespace mnx::topology
