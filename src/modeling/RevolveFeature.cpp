#include "mnx/modeling/RevolveFeature.h"

#include "mnx/modeling/FeatureContext.h"
#include "mnx/modeling/RevolveBuilder.h"

#include <utility>

namespace mnx {

RevolveFeature::RevolveFeature(
    FeatureId id,
    std::string name,
    FeatureId profileSketchFeatureId,
    RevolveAxis2D axis,
    double angleRadians
)
    : Feature(id, FeatureType::Revolve, std::move(name)),
      profileSketchFeatureId_(profileSketchFeatureId),
      axis_(axis),
      angleRadians_(angleRadians) {
    (void)addDependency(profileSketchFeatureId_);
}

FeatureId RevolveFeature::profileSketchFeatureId() const {
    return profileSketchFeatureId_;
}

void RevolveFeature::setProfileSketchFeatureId(FeatureId id) {
    profileSketchFeatureId_ = id;
    clearDependencies();
    (void)addDependency(id);
}

const RevolveAxis2D& RevolveFeature::axis() const {
    return axis_;
}

void RevolveFeature::setAxis(RevolveAxis2D axis) {
    axis_ = axis;
}

double RevolveFeature::angleRadians() const {
    return angleRadians_;
}

void RevolveFeature::setAngleRadians(double angleRadians) {
    angleRadians_ = angleRadians;
}

const std::optional<Body>& RevolveFeature::body() const {
    return body_;
}

const std::optional<DisplayMesh>& RevolveFeature::displayMesh() const {
    return displayMesh_;
}

FeatureResult RevolveFeature::execute(FeatureContext& context) {
    context.recordExecutedFeature(id());
    body_.reset();
    displayMesh_.reset();

    if(!profileSketchFeatureId_.isValid()) {
        FeatureResult result =
            FeatureResult::failure(Status::invalidArgument("Revolve profile sketch feature id is invalid"));
        result.addDiagnostic("Revolve requires a valid SketchFeature dependency");
        return result;
    }

    const Sketch* sketch = context.findSketch(profileSketchFeatureId_);
    if(sketch == nullptr) {
        FeatureResult result = FeatureResult::failure(Status::failedPrecondition(
            "Revolve profile sketch dependency has not been rebuilt"
        ));
        result.addDiagnostic("Revolve requires a prior SketchFeature dependency");
        return result;
    }

    const RevolveBuilder builder{};
    Result<RevolveBuildResult> buildResult = builder.buildRevolve(id(), *sketch, axis_, angleRadians_);
    if(!buildResult) {
        FeatureResult result = FeatureResult::failure(buildResult.status());
        result.addDiagnostic(buildResult.status().message());
        return result;
    }

    displayMesh_ = buildResult.value().mesh;
    body_ = buildResult.value().body;
    context.recordBody(*body_);

    FeatureResult result = FeatureResult::success();
    result.addBody(*body_);
    result.addDiagnostic("Revolve generated display mesh and placeholder body");
    return result;
}

} // namespace mnx
