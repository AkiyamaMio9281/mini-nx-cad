#pragma once

#include "mnx/topology/Body.h"

#include <cstddef>
#include <vector>

namespace mnx::topology {

class TopologyQueries {
public:
    [[nodiscard]] static std::vector<std::size_t> edgesOfFace(const Body& body, std::size_t faceIndex);
    [[nodiscard]] static std::vector<std::size_t> facesOfBody(const Body& body);
    [[nodiscard]] static std::vector<std::size_t> verticesOfEdge(const Body& body, std::size_t edgeIndex);
    [[nodiscard]] static std::vector<std::size_t> adjacentFacesOfEdge(const Body& body, std::size_t edgeIndex);
};

} // namespace mnx::topology
