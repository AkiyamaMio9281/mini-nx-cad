#include "mnx/topology/CoEdge.h"

namespace mnx::topology {

CoEdge::CoEdge(std::size_t edgeIndex, bool reversed) : edgeIndex_(edgeIndex), reversed_(reversed) {}

std::size_t CoEdge::edgeIndex() const {
    return edgeIndex_;
}

bool CoEdge::isReversed() const {
    return reversed_;
}

} // namespace mnx::topology
