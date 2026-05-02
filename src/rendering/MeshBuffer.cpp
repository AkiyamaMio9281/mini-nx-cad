#include "mnx/rendering/MeshBuffer.h"

namespace mnx::rendering {

void MeshBuffer::upload(const Mesh& mesh) {
    vertexCount_ = mesh.vertexCount();
    triangleCount_ = mesh.triangleCount();
    hasMesh_ = true;
}

bool MeshBuffer::hasMesh() const {
    return hasMesh_;
}

std::size_t MeshBuffer::vertexCount() const {
    return vertexCount_;
}

std::size_t MeshBuffer::triangleCount() const {
    return triangleCount_;
}

} // namespace mnx::rendering
