#pragma once

#include "mnx/core/ids/FeatureId.h"
#include "mnx/modeling/Body.h"
#include "mnx/modeling/ProfileUtils.h"

namespace mnx {

class BodyBuilder {
public:
    [[nodiscard]] Body buildExtrudeBody(FeatureId sourceFeatureId, const Profile& profile, DisplayMesh mesh, double distance)
        const;
    [[nodiscard]] Body buildRevolveBody(FeatureId sourceFeatureId, DisplayMesh mesh) const;

private:
    [[nodiscard]] topology::Body buildTopologyFromMesh(const DisplayMesh& mesh) const;
};

} // namespace mnx
