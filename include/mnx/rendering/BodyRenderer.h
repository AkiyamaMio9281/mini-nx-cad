#pragma once

#include "mnx/modeling/Body.h"
#include "mnx/rendering/RenderItem.h"
#include "mnx/rendering/RenderScene.h"

namespace mnx::rendering {

class BodyRenderer {
public:
    [[nodiscard]] RenderItem makeRenderItem(const mnx::Body& body, std::string name = {}) const;
    void populateScene(RenderScene& scene, const mnx::Body& body, std::string name = {}) const;
};

} // namespace mnx::rendering
