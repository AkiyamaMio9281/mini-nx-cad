#pragma once

#include "mnx/rendering/Mesh.h"

namespace mnx::rendering {

class MeshBuffer {
public:
    void upload(const Mesh& mesh);
    [[nodiscard]] bool hasMesh() const;
    [[nodiscard]] std::size_t vertexCount() const;
    [[nodiscard]] std::size_t triangleCount() const;

private:
    std::size_t vertexCount_ = 0;
    std::size_t triangleCount_ = 0;
    bool hasMesh_ = false;
};

} // namespace mnx::rendering
