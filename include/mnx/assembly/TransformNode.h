#pragma once

#include "mnx/core/ids/ComponentId.h"
#include "mnx/core/math/Transform3D.h"

#include <optional>

namespace mnx {

class TransformNode {
public:
    TransformNode() = default;
    explicit TransformNode(Transform3D localTransform, std::optional<ComponentId> parentId = std::nullopt);

    [[nodiscard]] const Transform3D& localTransform() const;
    void setLocalTransform(const Transform3D& transform);

    [[nodiscard]] std::optional<ComponentId> parentId() const;
    void setParentId(std::optional<ComponentId> parentId);
    [[nodiscard]] bool hasParent() const;

private:
    Transform3D localTransform_;
    std::optional<ComponentId> parentId_;
};

} // namespace mnx
