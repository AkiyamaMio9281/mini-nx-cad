#pragma once

#include "mnx/core/math/AABB3.h"
#include "mnx/geometry/Point3.h"
#include "mnx/modeling/DisplayMesh.h"

#include <array>
#include <span>
#include <vector>

namespace mnx::rendering {

struct MeshVertex {
    Point3 position;
};

class Mesh {
public:
    [[nodiscard]] static Mesh fromDisplayMesh(const mnx::DisplayMesh& displayMesh);

    [[nodiscard]] std::size_t addVertex(const Point3& point);
    void addTriangle(std::size_t first, std::size_t second, std::size_t third);

    [[nodiscard]] std::span<const MeshVertex> vertices() const;
    [[nodiscard]] std::span<const MeshTriangle> triangles() const;
    [[nodiscard]] std::size_t vertexCount() const;
    [[nodiscard]] std::size_t triangleCount() const;
    [[nodiscard]] AABB3 bounds() const;

private:
    std::vector<MeshVertex> vertices_;
    std::vector<MeshTriangle> triangles_;
};

} // namespace mnx::rendering
