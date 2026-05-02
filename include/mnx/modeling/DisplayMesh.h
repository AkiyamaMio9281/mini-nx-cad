#pragma once

#include "mnx/core/math/AABB3.h"
#include "mnx/geometry/Point3.h"

#include <array>
#include <span>
#include <vector>

namespace mnx {

struct MeshTriangle {
    std::array<std::size_t, 3> vertexIndices {};
};

class DisplayMesh {
public:
    [[nodiscard]] std::size_t addVertex(const Point3& point);
    void addTriangle(std::size_t first, std::size_t second, std::size_t third);
    void clear();

    [[nodiscard]] std::span<const Point3> vertices() const;
    [[nodiscard]] std::span<const MeshTriangle> triangles() const;
    [[nodiscard]] std::size_t vertexCount() const;
    [[nodiscard]] std::size_t triangleCount() const;
    [[nodiscard]] AABB3 bounds() const;

private:
    std::vector<Point3> vertices_;
    std::vector<MeshTriangle> triangles_;
};

} // namespace mnx
