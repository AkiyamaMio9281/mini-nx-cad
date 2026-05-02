#include "mnx/modeling/ExtrudeBuilder.h"

#include "mnx/core/math/Tolerance.h"

#include <cmath>
#include <utility>

namespace mnx {

Result<ExtrudeBuildResult> ExtrudeBuilder::buildBlindExtrude(
    FeatureId featureId,
    const Sketch& sketch,
    double distance
) const {
    if(!featureId.isValid()) {
        return Result<ExtrudeBuildResult>::failure(Status::invalidArgument("Extrude feature id is invalid"));
    }
    if(distance <= kDefaultTolerance) {
        return Result<ExtrudeBuildResult>::failure(Status::invalidArgument("Extrude distance must be positive"));
    }

    Result<Profile> profileResult = ProfileUtils::extractClosedProfile(sketch);
    if(!profileResult) {
        return Result<ExtrudeBuildResult>::failure(profileResult.status());
    }

    Profile profile = profileResult.value();
    DisplayMesh mesh = meshBuilder_.buildExtrudeMesh(profile, distance);
    if(mesh.vertexCount() == 0 || mesh.triangleCount() == 0) {
        return Result<ExtrudeBuildResult>::failure(Status::internalError("Extrude mesh generation produced no geometry"));
    }

    Body body = bodyBuilder_.buildExtrudeBody(featureId, profile, mesh, distance);
    ExtrudeBuildResult result {std::move(profile), std::move(mesh), std::move(body)};
    return result;
}

} // namespace mnx
