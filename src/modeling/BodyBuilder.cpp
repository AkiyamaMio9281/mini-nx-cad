#include "mnx/modeling/BodyBuilder.h"

#include <algorithm>
#include <cmath>
#include <map>
#include <utility>
#include <vector>

namespace mnx {

Body BodyBuilder::buildExtrudeBody(FeatureId sourceFeatureId, const Profile& profile, DisplayMesh mesh, double distance)
    const {
    const BodyId bodyId(sourceFeatureId.value());
    const double volume = profile.area() * std::abs(distance);
    topology::Body topologyBody = buildTopologyFromMesh(mesh);
    return Body(bodyId, sourceFeatureId, std::move(mesh), volume, std::move(topologyBody));
}

Body BodyBuilder::buildRevolveBody(FeatureId sourceFeatureId, DisplayMesh mesh) const {
    topology::Body topologyBody = buildTopologyFromMesh(mesh);
    return Body(BodyId(sourceFeatureId.value()), sourceFeatureId, std::move(mesh), 0.0, std::move(topologyBody));
}

topology::Body BodyBuilder::buildTopologyFromMesh(const DisplayMesh& mesh) const {
    topology::Body body;
    for(const Point3& vertex : mesh.vertices()) {
        (void)body.addVertex(topology::Vertex(vertex));
    }

    std::map<std::pair<std::size_t, std::size_t>, std::size_t> edgeMap;
    std::vector<std::size_t> faceIndices;
    for(const MeshTriangle& triangle : mesh.triangles()) {
        std::vector<std::size_t> coEdgeIndices;
        for(std::size_t local = 0; local < 3; ++local) {
            const std::size_t start = triangle.vertexIndices[local];
            const std::size_t end = triangle.vertexIndices[(local + 1) % 3];
            const auto key = std::minmax(start, end);
            auto [iterator, inserted] = edgeMap.emplace(key, 0);
            if(inserted) {
                iterator->second = body.addEdge(topology::Edge(key.first, key.second));
            }

            const bool reversed = start != key.first;
            coEdgeIndices.push_back(body.addCoEdge(topology::CoEdge(iterator->second, reversed)));
        }

        const std::size_t loopIndex = body.addLoop(topology::Loop(std::move(coEdgeIndices)));
        faceIndices.push_back(body.addFace(topology::Face({loopIndex})));
    }

    if(!faceIndices.empty()) {
        (void)body.addShell(topology::Shell(std::move(faceIndices)));
    }
    return body;
}

} // namespace mnx
