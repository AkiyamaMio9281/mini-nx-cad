#include "mnx/rendering/Mesh.h"

namespace mnx::rendering {

Mesh Mesh::fromDisplayMesh(const mnx::DisplayMesh& displayMesh) {
    Mesh mesh;
    for(const Point3& point : displayMesh.vertices()) {
        (void)mesh.addVertex(point);
    }
    for(const MeshTriangle& triangle : displayMesh.triangles()) {
        mesh.addTriangle(triangle.vertexIndices[0], triangle.vertexIndices[1], triangle.vertexIndices[2]);
    }
    return mesh;
}

std::size_t Mesh::addVertex(const Point3& point) {
    vertices_.push_back({point});
    return vertices_.size() - 1;
}

void Mesh::addTriangle(std::size_t first, std::size_t second, std::size_t third) {
    triangles_.push_back({{first, second, third}});
}

std::span<const MeshVertex> Mesh::vertices() const {
    return vertices_;
}

std::span<const MeshTriangle> Mesh::triangles() const {
    return triangles_;
}

std::size_t Mesh::vertexCount() const {
    return vertices_.size();
}

std::size_t Mesh::triangleCount() const {
    return triangles_.size();
}

AABB3 Mesh::bounds() const {
    AABB3 box;
    for(const MeshVertex& vertex : vertices_) {
        box.expand(vertex.position.vector());
    }
    return box;
}

} // namespace mnx::rendering
