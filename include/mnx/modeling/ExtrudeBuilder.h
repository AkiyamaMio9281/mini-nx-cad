#pragma once

#include "mnx/core/ids/FeatureId.h"
#include "mnx/core/utils/Result.h"
#include "mnx/modeling/Body.h"
#include "mnx/modeling/BodyBuilder.h"
#include "mnx/modeling/MeshBuilder.h"
#include "mnx/modeling/ProfileUtils.h"
#include "mnx/sketch/Sketch.h"

namespace mnx {

struct ExtrudeBuildResult {
    Profile profile;
    DisplayMesh mesh;
    Body body;
};

class ExtrudeBuilder {
public:
    [[nodiscard]] Result<ExtrudeBuildResult> buildBlindExtrude(
        FeatureId featureId,
        const Sketch& sketch,
        double distance
    ) const;

private:
    MeshBuilder meshBuilder_;
    BodyBuilder bodyBuilder_;
};

} // namespace mnx
