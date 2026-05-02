#include "mnx/rendering/RenderItem.h"

#include <utility>

namespace mnx::rendering {

RenderItem::RenderItem(BodyId bodyId, std::string name, Mesh mesh)
    : bodyId_(bodyId), name_(std::move(name)), mesh_(std::move(mesh)) {}

BodyId RenderItem::bodyId() const {
    return bodyId_;
}

const std::string& RenderItem::name() const {
    return name_;
}

const Mesh& RenderItem::mesh() const {
    return mesh_;
}

Mesh& RenderItem::mesh() {
    return mesh_;
}

const Transform3D& RenderItem::transform() const {
    return transform_;
}

void RenderItem::setTransform(const Transform3D& transform) {
    transform_ = transform;
}

bool RenderItem::isVisible() const {
    return visible_;
}

void RenderItem::setVisible(bool visible) {
    visible_ = visible;
}

bool RenderItem::isSelected() const {
    return selected_;
}

void RenderItem::setSelected(bool selected) {
    selected_ = selected;
}

bool RenderItem::isHighlighted() const {
    return highlighted_;
}

void RenderItem::setHighlighted(bool highlighted) {
    highlighted_ = highlighted;
}

AABB3 RenderItem::bounds() const {
    AABB3 box;
    for(const MeshVertex& vertex : mesh_.vertices()) {
        box.expand(transform_.applyToPoint(vertex.position.vector()));
    }
    return box;
}

} // namespace mnx::rendering
