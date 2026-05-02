#pragma once

#include "mnx/core/ids/BodyId.h"
#include "mnx/core/ids/FeatureId.h"
#include "mnx/core/math/AABB3.h"
#include "mnx/modeling/DisplayMesh.h"
#include "mnx/topology/Body.h"

namespace mnx {

class Body {
public:
    Body() = default;
    Body(BodyId id, FeatureId sourceFeatureId, DisplayMesh mesh, double volume, topology::Body topologyBody = {});

    [[nodiscard]] BodyId id() const;
    [[nodiscard]] FeatureId sourceFeatureId() const;
    [[nodiscard]] const DisplayMesh& displayMesh() const;
    [[nodiscard]] const topology::Body& topology() const;
    [[nodiscard]] AABB3 bounds() const;
    [[nodiscard]] double volume() const;

private:
    BodyId id_;
    FeatureId sourceFeatureId_;
    DisplayMesh displayMesh_;
    topology::Body topology_;
    double volume_ = 0.0;
};

} // namespace mnx
