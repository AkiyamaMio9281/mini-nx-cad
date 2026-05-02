#pragma once

#include "mnx/core/ids/BodyId.h"
#include "mnx/core/utils/Result.h"
#include "mnx/rendering/Camera.h"
#include "mnx/rendering/RenderScene.h"

#include <optional>

namespace mnx::rendering {

struct PickResult {
    BodyId bodyId;
    double distance = 0.0;
};

class Picking {
public:
    [[nodiscard]] static std::optional<double> intersectAABB(const Ray& ray, const AABB3& box);
    [[nodiscard]] static std::optional<PickResult> pickBody(const RenderScene& scene, const Ray& ray);
};

} // namespace mnx::rendering
