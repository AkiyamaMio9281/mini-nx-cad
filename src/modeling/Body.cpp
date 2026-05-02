#include "mnx/modeling/Body.h"

#include <utility>

namespace mnx {

Body::Body(BodyId id, FeatureId sourceFeatureId, DisplayMesh mesh, double volume, topology::Body topologyBody)
    : id_(id),
      sourceFeatureId_(sourceFeatureId),
      displayMesh_(std::move(mesh)),
      topology_(std::move(topologyBody)),
      volume_(volume) {}

BodyId Body::id() const {
    return id_;
}

FeatureId Body::sourceFeatureId() const {
    return sourceFeatureId_;
}

const DisplayMesh& Body::displayMesh() const {
    return displayMesh_;
}

const topology::Body& Body::topology() const {
    return topology_;
}

AABB3 Body::bounds() const {
    return displayMesh_.bounds();
}

double Body::volume() const {
    return volume_;
}

} // namespace mnx
