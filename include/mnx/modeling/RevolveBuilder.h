#pragma once

#include "mnx/core/ids/FeatureId.h"
#include "mnx/core/utils/Result.h"
#include "mnx/modeling/Body.h"
#include "mnx/modeling/BodyBuilder.h"
#include "mnx/modeling/MeshBuilder.h"
#include "mnx/modeling/ProfileUtils.h"
#include "mnx/modeling/RevolveAxis2D.h"
#include "mnx/sketch/Sketch.h"

namespace mnx {

struct RevolveBuildResult {
    Profile profile;
    DisplayMesh mesh;
    Body body;
};

class RevolveBuilder {
public:
    [[nodiscard]] Result<RevolveBuildResult> buildRevolve(
        FeatureId featureId,
        const Sketch& sketch,
        const RevolveAxis2D& axis,
        double angleRadians
    ) const;

private:
    [[nodiscard]] Status validateProfileAgainstAxis(const Profile& profile, const RevolveAxis2D& axis) const;

    MeshBuilder meshBuilder_;
    BodyBuilder bodyBuilder_;
};

} // namespace mnx
