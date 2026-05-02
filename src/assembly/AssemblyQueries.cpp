#include "mnx/assembly/AssemblyQueries.h"

#include "mnx/modeling/ExtrudeFeature.h"
#include "mnx/modeling/RevolveFeature.h"

namespace mnx {

namespace {

void expandTransformed(AABB3& result, const AABB3& localBounds, const Transform3D& transform) {
    if(!localBounds.isValid()) {
        return;
    }
    const Vec3 min = localBounds.min();
    const Vec3 max = localBounds.max();
    for(double x : {min.x(), max.x()}) {
        for(double y : {min.y(), max.y()}) {
            for(double z : {min.z(), max.z()}) {
                result.expand(transform.applyToPoint(Vec3(x, y, z)));
            }
        }
    }
}

} // namespace

std::vector<ComponentId> AssemblyQueries::rootComponents(const AssemblyDocument& assembly) {
    std::vector<ComponentId> result;
    for(const ComponentInstance& component : assembly.components()) {
        if(!component.transformNode().hasParent()) {
            result.push_back(component.id());
        }
    }
    return result;
}

std::vector<ComponentId> AssemblyQueries::childComponents(const AssemblyDocument& assembly, ComponentId parentId) {
    std::vector<ComponentId> result;
    for(const ComponentInstance& component : assembly.components()) {
        if(component.transformNode().parentId() == parentId) {
            result.push_back(component.id());
        }
    }
    return result;
}

AABB3 AssemblyQueries::bounds(const AssemblyDocument& assembly) {
    AABB3 result;
    for(const ComponentInstance& component : assembly.components()) {
        std::shared_ptr<const PartDocument> part = component.partDocument();
        if(!part) {
            continue;
        }
        Result<Transform3D> transform = assembly.worldTransform(component.id());
        if(!transform) {
            continue;
        }
        for(const Feature* feature : part->featureGraph().features()) {
            if(feature == nullptr) {
                continue;
            }
            if(const auto* extrude = dynamic_cast<const ExtrudeFeature*>(feature);
               extrude != nullptr && extrude->body().has_value()) {
                expandTransformed(result, extrude->body()->bounds(), transform.value());
            } else if(const auto* revolve = dynamic_cast<const RevolveFeature*>(feature);
                      revolve != nullptr && revolve->body().has_value()) {
                expandTransformed(result, revolve->body()->bounds(), transform.value());
            }
        }
    }
    return result;
}

} // namespace mnx
