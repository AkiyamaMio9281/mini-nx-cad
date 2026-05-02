#include "mnx/topology/TopologyValidator.h"

#include <unordered_set>

namespace mnx::topology {

Status TopologyValidator::validate(const Body& body) const {
    if(body.vertices().empty()) {
        return Status::failedPrecondition("Topology body has no vertices");
    }
    if(body.edges().empty()) {
        return Status::failedPrecondition("Topology body has no edges");
    }
    if(body.faces().empty()) {
        return Status::failedPrecondition("Topology body has no faces");
    }
    if(body.shells().empty()) {
        return Status::failedPrecondition("Topology body has no shells");
    }

    for(const Edge& edge : body.edges()) {
        if(edge.startVertexIndex() >= body.vertices().size() || edge.endVertexIndex() >= body.vertices().size()) {
            return Status::failedPrecondition("Topology edge references a missing vertex");
        }
        if(edge.startVertexIndex() == edge.endVertexIndex()) {
            return Status::failedPrecondition("Topology edge has identical start and end vertices");
        }
    }

    for(const CoEdge& coEdge : body.coEdges()) {
        if(coEdge.edgeIndex() >= body.edges().size()) {
            return Status::failedPrecondition("Topology coedge references a missing edge");
        }
    }

    for(const Loop& loop : body.loops()) {
        if(loop.coEdgeCount() < 3) {
            return Status::failedPrecondition("Topology loop must contain at least three coedges");
        }
        for(std::size_t coEdgeIndex : loop.coEdgeIndices()) {
            if(coEdgeIndex >= body.coEdges().size()) {
                return Status::failedPrecondition("Topology loop references a missing coedge");
            }
        }
    }

    for(const Face& face : body.faces()) {
        if(face.loopIndices().empty()) {
            return Status::failedPrecondition("Topology face has no loops");
        }
        for(std::size_t loopIndex : face.loopIndices()) {
            if(loopIndex >= body.loops().size()) {
                return Status::failedPrecondition("Topology face references a missing loop");
            }
        }
    }

    std::unordered_set<std::size_t> referencedFaces;
    for(const Shell& shell : body.shells()) {
        if(shell.faceCount() == 0) {
            return Status::failedPrecondition("Topology shell has no faces");
        }
        for(std::size_t faceIndex : shell.faceIndices()) {
            if(faceIndex >= body.faces().size()) {
                return Status::failedPrecondition("Topology shell references a missing face");
            }
            referencedFaces.insert(faceIndex);
        }
    }
    if(referencedFaces.size() != body.faces().size()) {
        return Status::failedPrecondition("Topology body has faces that are not referenced by a shell");
    }

    return Status::ok();
}

} // namespace mnx::topology
