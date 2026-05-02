#pragma once

#include <cstddef>
#include <span>
#include <vector>

namespace mnx::topology {

class Loop {
public:
    Loop() = default;
    explicit Loop(std::vector<std::size_t> coEdgeIndices);

    [[nodiscard]] std::span<const std::size_t> coEdgeIndices() const;
    [[nodiscard]] std::size_t coEdgeCount() const;

private:
    std::vector<std::size_t> coEdgeIndices_;
};

} // namespace mnx::topology
