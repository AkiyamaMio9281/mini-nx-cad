#include "mnx/modeling/RevolveBuilder.h"

#include "mnx/core/math/Tolerance.h"

#include <cmath>
#include <numbers>
#include <utility>

namespace mnx {

Result<RevolveBuildResult> RevolveBuilder::buildRevolve(
    FeatureId featureId,
    const Sketch& sketch,
    const RevolveAxis2D& axis,
    double angleRadians
) const {
    if(!featureId.isValid()) {
        return Result<RevolveBuildResult>::failure(Status::invalidArgument("Revolve feature id is invalid"));
    }
    if(!axis.isValid()) {
        return Result<RevolveBuildResult>::failure(Status::invalidArgument("Revolve axis direction must be non-zero"));
    }
    if(angleRadians <= kDefaultTolerance || angleRadians > 2.0 * std::numbers::pi + kDefaultTolerance) {
        return Result<RevolveBuildResult>::failure(Status::invalidArgument(
            "Revolve angle must be greater than zero and no more than a full turn"
        ));
    }

    Result<Profile> profileResult = ProfileUtils::extractClosedProfile(sketch);
    if(!profileResult) {
        return Result<RevolveBuildResult>::failure(profileResult.status());
    }

    Profile profile = profileResult.value();
    Status axisStatus = validateProfileAgainstAxis(profile, axis);
    if(!axisStatus) {
        return Result<RevolveBuildResult>::failure(axisStatus);
    }

    DisplayMesh mesh = meshBuilder_.buildRevolveMesh(profile, axis, angleRadians);
    if(mesh.vertexCount() == 0 || mesh.triangleCount() == 0) {
        return Result<RevolveBuildResult>::failure(Status::internalError("Revolve mesh generation produced no geometry"));
    }

    Body body = bodyBuilder_.buildRevolveBody(featureId, mesh);
    RevolveBuildResult result {std::move(profile), std::move(mesh), std::move(body)};
    return result;
}

Status RevolveBuilder::validateProfileAgainstAxis(const Profile& profile, const RevolveAxis2D& axis) const {
    const std::vector<Point2> boundary = profile.tessellatedBoundary();
    if(boundary.empty()) {
        return Status::failedPrecondition("Revolve profile has no boundary");
    }

    int side = 0;
    for(const Point2& point : boundary) {
        const double signedDistance = axis.signedDistance(point);
        if(std::abs(signedDistance) <= kDefaultTolerance) {
            return Status::failedPrecondition("Revolve axis touches the profile boundary");
        }

        const int pointSide = signedDistance > 0.0 ? 1 : -1;
        if(side == 0) {
            side = pointSide;
        } else if(side != pointSide) {
            return Status::failedPrecondition("Revolve axis crosses the profile; crossing profiles are unsupported");
        }
    }
    return Status::ok();
}

} // namespace mnx
