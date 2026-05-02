#include "mnx/assembly/TransformNode.h"

#include <utility>

namespace mnx {

TransformNode::TransformNode(Transform3D localTransform, std::optional<ComponentId> parentId)
    : localTransform_(std::move(localTransform)), parentId_(parentId) {}

const Transform3D& TransformNode::localTransform() const {
    return localTransform_;
}

void TransformNode::setLocalTransform(const Transform3D& transform) {
    localTransform_ = transform;
}

std::optional<ComponentId> TransformNode::parentId() const {
    return parentId_;
}

void TransformNode::setParentId(std::optional<ComponentId> parentId) {
    parentId_ = parentId;
}

bool TransformNode::hasParent() const {
    return parentId_.has_value();
}

} // namespace mnx
