#pragma once

#include <cstddef>
#include <span>
#include <vector>

namespace mnx::topology {

class Face {
public:
    Face() = default;
    explicit Face(std::vector<std::size_t> loopIndices);

    [[nodiscard]] std::span<const std::size_t> loopIndices() const;
    [[nodiscard]] std::size_t outerLoopIndex() const;

private:
    std::vector<std::size_t> loopIndices_;
};

} // namespace mnx::topology
