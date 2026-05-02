#pragma once

#include <cstddef>
#include <span>
#include <vector>

namespace mnx::topology {

class Shell {
public:
    Shell() = default;
    explicit Shell(std::vector<std::size_t> faceIndices);

    [[nodiscard]] std::span<const std::size_t> faceIndices() const;
    [[nodiscard]] std::size_t faceCount() const;

private:
    std::vector<std::size_t> faceIndices_;
};

} // namespace mnx::topology
