#include "mnx/modeling/DisplayMesh.h"

namespace mnx {

std::size_t DisplayMesh::addVertex(const Point3& point) {
    vertices_.push_back(point);
    return vertices_.size() - 1;
}

void DisplayMesh::addTriangle(std::size_t first, std::size_t second, std::size_t third) {
    triangles_.push_back({{first, second, third}});
}

void DisplayMesh::clear() {
    vertices_.clear();
    triangles_.clear();
}

std::span<const Point3> DisplayMesh::vertices() const {
    return vertices_;
}

std::span<const MeshTriangle> DisplayMesh::triangles() const {
    return triangles_;
}

std::size_t DisplayMesh::vertexCount() const {
    return vertices_.size();
}

std::size_t DisplayMesh::triangleCount() const {
    return triangles_.size();
}

AABB3 DisplayMesh::bounds() const {
    AABB3 box;
    for(const Point3& vertex : vertices_) {
        box.expand(vertex.vector());
    }
    return box;
}

} // namespace mnx
