#include "mnx/rendering/RenderScene.h"

#include <algorithm>

namespace mnx::rendering {

RenderItem& RenderScene::addItem(RenderItem item) {
    items_.push_back(std::move(item));
    return items_.back();
}

void RenderScene::clear() {
    items_.clear();
}

RenderItem* RenderScene::findItem(BodyId bodyId) {
    const auto iterator = std::find_if(items_.begin(), items_.end(), [bodyId](const RenderItem& item) {
        return item.bodyId() == bodyId;
    });
    return iterator == items_.end() ? nullptr : &*iterator;
}

const RenderItem* RenderScene::findItem(BodyId bodyId) const {
    const auto iterator = std::find_if(items_.begin(), items_.end(), [bodyId](const RenderItem& item) {
        return item.bodyId() == bodyId;
    });
    return iterator == items_.end() ? nullptr : &*iterator;
}

std::span<RenderItem> RenderScene::items() {
    return items_;
}

std::span<const RenderItem> RenderScene::items() const {
    return items_;
}

AABB3 RenderScene::bounds() const {
    AABB3 box;
    for(const RenderItem& item : items_) {
        if(item.isVisible()) {
            box.expand(item.bounds());
        }
    }
    return box;
}

void RenderScene::clearSelection() {
    for(RenderItem& item : items_) {
        item.setSelected(false);
    }
}

bool RenderScene::selectBody(BodyId bodyId) {
    clearSelection();
    RenderItem* item = findItem(bodyId);
    if(item == nullptr) {
        return false;
    }
    item->setSelected(true);
    return true;
}

std::optional<BodyId> RenderScene::selectedBodyId() const {
    for(const RenderItem& item : items_) {
        if(item.isSelected()) {
            return item.bodyId();
        }
    }
    return std::nullopt;
}

} // namespace mnx::rendering
