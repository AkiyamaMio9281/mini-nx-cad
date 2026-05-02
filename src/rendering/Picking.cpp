#include "mnx/rendering/Picking.h"

#include "mnx/core/math/Tolerance.h"

#include <algorithm>
#include <limits>

namespace mnx::rendering {

namespace {

[[nodiscard]] bool updateSlab(double origin, double direction, double min, double max, double& tMin, double& tMax) {
    if(isZero(direction)) {
        return origin >= min && origin <= max;
    }
    double t1 = (min - origin) / direction;
    double t2 = (max - origin) / direction;
    if(t1 > t2) {
        std::swap(t1, t2);
    }
    tMin = std::max(tMin, t1);
    tMax = std::min(tMax, t2);
    return tMin <= tMax;
}

} // namespace

std::optional<double> Picking::intersectAABB(const Ray& ray, const AABB3& box) {
    if(!box.isValid()) {
        return std::nullopt;
    }

    double tMin = 0.0;
    double tMax = std::numeric_limits<double>::infinity();
    if(!updateSlab(ray.origin.x(), ray.direction.x(), box.min().x(), box.max().x(), tMin, tMax)) {
        return std::nullopt;
    }
    if(!updateSlab(ray.origin.y(), ray.direction.y(), box.min().y(), box.max().y(), tMin, tMax)) {
        return std::nullopt;
    }
    if(!updateSlab(ray.origin.z(), ray.direction.z(), box.min().z(), box.max().z(), tMin, tMax)) {
        return std::nullopt;
    }
    return tMin;
}

std::optional<PickResult> Picking::pickBody(const RenderScene& scene, const Ray& ray) {
    std::optional<PickResult> best;
    for(const RenderItem& item : scene.items()) {
        if(!item.isVisible()) {
            continue;
        }
        const std::optional<double> distance = intersectAABB(ray, item.bounds());
        if(distance.has_value() && (!best.has_value() || *distance < best->distance)) {
            best = PickResult {item.bodyId(), *distance};
        }
    }
    return best;
}

} // namespace mnx::rendering
