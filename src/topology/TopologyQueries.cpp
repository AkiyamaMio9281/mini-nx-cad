#include "mnx/topology/TopologyQueries.h"

#include <algorithm>

namespace mnx::topology {

std::vector<std::size_t> TopologyQueries::edgesOfFace(const Body& body, std::size_t faceIndex) {
    std::vector<std::size_t> result;
    const Face* face = body.face(faceIndex);
    if(face == nullptr) {
        return result;
    }

    for(std::size_t loopIndex : face->loopIndices()) {
        const Loop* loop = body.loop(loopIndex);
        if(loop == nullptr) {
            continue;
        }
        for(std::size_t coEdgeIndex : loop->coEdgeIndices()) {
            const CoEdge* coEdge = body.coEdge(coEdgeIndex);
            if(coEdge != nullptr) {
                result.push_back(coEdge->edgeIndex());
            }
        }
    }
    return result;
}

std::vector<std::size_t> TopologyQueries::facesOfBody(const Body& body) {
    std::vector<std::size_t> result;
    for(std::size_t index = 0; index < body.faces().size(); ++index) {
        result.push_back(index);
    }
    return result;
}

std::vector<std::size_t> TopologyQueries::verticesOfEdge(const Body& body, std::size_t edgeIndex) {
    const Edge* edge = body.edge(edgeIndex);
    if(edge == nullptr) {
        return {};
    }
    return {edge->startVertexIndex(), edge->endVertexIndex()};
}

std::vector<std::size_t> TopologyQueries::adjacentFacesOfEdge(const Body& body, std::size_t edgeIndex) {
    std::vector<std::size_t> result;
    for(std::size_t faceIndex = 0; faceIndex < body.faces().size(); ++faceIndex) {
        const std::vector<std::size_t> faceEdges = edgesOfFace(body, faceIndex);
        if(std::find(faceEdges.begin(), faceEdges.end(), edgeIndex) != faceEdges.end()) {
            result.push_back(faceIndex);
        }
    }
    return result;
}

} // namespace mnx::topology
