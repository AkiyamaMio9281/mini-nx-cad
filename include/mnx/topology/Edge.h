#pragma once

#include <cstddef>

namespace mnx::topology {

class Edge {
public:
    Edge() = default;
    Edge(std::size_t startVertexIndex, std::size_t endVertexIndex);

    [[nodiscard]] std::size_t startVertexIndex() const;
    [[nodiscard]] std::size_t endVertexIndex() const;

private:
    std::size_t startVertexIndex_ = 0;
    std::size_t endVertexIndex_ = 0;
};

} // namespace mnx::topology
