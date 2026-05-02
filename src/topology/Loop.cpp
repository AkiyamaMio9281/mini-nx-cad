#include "mnx/topology/Loop.h"

#include <utility>

namespace mnx::topology {

Loop::Loop(std::vector<std::size_t> coEdgeIndices) : coEdgeIndices_(std::move(coEdgeIndices)) {}

std::span<const std::size_t> Loop::coEdgeIndices() const {
    return coEdgeIndices_;
}

std::size_t Loop::coEdgeCount() const {
    return coEdgeIndices_.size();
}

} // namespace mnx::topology
