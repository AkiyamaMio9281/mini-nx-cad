#pragma once

#include "mnx/core/ids/BodyId.h"
#include "mnx/core/math/AABB3.h"
#include "mnx/rendering/RenderItem.h"

#include <optional>
#include <span>
#include <vector>

namespace mnx::rendering {

class RenderScene {
public:
    [[nodiscard]] RenderItem& addItem(RenderItem item);
    void clear();

    [[nodiscard]] RenderItem* findItem(BodyId bodyId);
    [[nodiscard]] const RenderItem* findItem(BodyId bodyId) const;
    [[nodiscard]] std::span<RenderItem> items();
    [[nodiscard]] std::span<const RenderItem> items() const;
    [[nodiscard]] AABB3 bounds() const;

    void clearSelection();
    [[nodiscard]] bool selectBody(BodyId bodyId);
    [[nodiscard]] std::optional<BodyId> selectedBodyId() const;

private:
    std::vector<RenderItem> items_;
};

} // namespace mnx::rendering
