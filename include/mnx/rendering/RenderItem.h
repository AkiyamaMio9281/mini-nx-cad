#pragma once

#include "mnx/core/ids/BodyId.h"
#include "mnx/core/math/AABB3.h"
#include "mnx/core/math/Transform3D.h"
#include "mnx/rendering/Mesh.h"

#include <string>

namespace mnx::rendering {

class RenderItem {
public:
    RenderItem() = default;
    RenderItem(BodyId bodyId, std::string name, Mesh mesh);

    [[nodiscard]] BodyId bodyId() const;
    [[nodiscard]] const std::string& name() const;
    [[nodiscard]] const Mesh& mesh() const;
    [[nodiscard]] Mesh& mesh();

    [[nodiscard]] const Transform3D& transform() const;
    void setTransform(const Transform3D& transform);

    [[nodiscard]] bool isVisible() const;
    void setVisible(bool visible);
    [[nodiscard]] bool isSelected() const;
    void setSelected(bool selected);
    [[nodiscard]] bool isHighlighted() const;
    void setHighlighted(bool highlighted);
    [[nodiscard]] AABB3 bounds() const;

private:
    BodyId bodyId_;
    std::string name_;
    Mesh mesh_;
    Transform3D transform_;
    bool visible_ = true;
    bool selected_ = false;
    bool highlighted_ = false;
};

} // namespace mnx::rendering
