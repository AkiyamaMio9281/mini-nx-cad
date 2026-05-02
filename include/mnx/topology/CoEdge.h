#pragma once

#include <cstddef>

namespace mnx::topology {

class CoEdge {
public:
    CoEdge() = default;
    CoEdge(std::size_t edgeIndex, bool reversed);

    [[nodiscard]] std::size_t edgeIndex() const;
    [[nodiscard]] bool isReversed() const;

private:
    std::size_t edgeIndex_ = 0;
    bool reversed_ = false;
};

} // namespace mnx::topology
