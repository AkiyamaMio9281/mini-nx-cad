#include "mnx/topology/Edge.h"

namespace mnx::topology {

Edge::Edge(std::size_t startVertexIndex, std::size_t endVertexIndex)
    : startVertexIndex_(startVertexIndex), endVertexIndex_(endVertexIndex) {}

std::size_t Edge::startVertexIndex() const {
    return startVertexIndex_;
}

std::size_t Edge::endVertexIndex() const {
    return endVertexIndex_;
}

} // namespace mnx::topology
