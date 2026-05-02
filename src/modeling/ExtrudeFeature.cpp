#include "mnx/modeling/ExtrudeFeature.h"

#include "mnx/modeling/ExtrudeBuilder.h"
#include "mnx/modeling/FeatureContext.h"

#include <cmath>
#include <utility>

namespace mnx {

ExtrudeFeature::ExtrudeFeature(FeatureId id, std::string name, FeatureId profileSketchFeatureId, double distance)
    : Feature(id, FeatureType::Extrude, std::move(name)),
      profileSketchFeatureId_(profileSketchFeatureId),
      distance_(distance) {
    (void)addDependency(profileSketchFeatureId_);
}

FeatureId ExtrudeFeature::profileSketchFeatureId() const {
    return profileSketchFeatureId_;
}

void ExtrudeFeature::setProfileSketchFeatureId(FeatureId id) {
    profileSketchFeatureId_ = id;
    clearDependencies();
    (void)addDependency(id);
}

double ExtrudeFeature::distance() const {
    return distance_;
}

void ExtrudeFeature::setDistance(double distance) {
    distance_ = distance;
}

const std::optional<Body>& ExtrudeFeature::body() const {
    return body_;
}

const std::optional<DisplayMesh>& ExtrudeFeature::displayMesh() const {
    return displayMesh_;
}

FeatureResult ExtrudeFeature::execute(FeatureContext& context) {
    context.recordExecutedFeature(id());
    body_.reset();
    displayMesh_.reset();

    if(!profileSketchFeatureId_.isValid()) {
        FeatureResult result = FeatureResult::failure(Status::invalidArgument("Extrude profile sketch feature id is invalid"));
        result.addDiagnostic("Extrude requires a valid SketchFeature dependency");
        return result;
    }

    const Sketch* sketch = context.findSketch(profileSketchFeatureId_);
    if(sketch == nullptr) {
        FeatureResult result = FeatureResult::failure(Status::failedPrecondition(
            "Extrude profile sketch dependency has not been rebuilt"
        ));
        result.addDiagnostic("Extrude requires a prior SketchFeature dependency");
        return result;
    }

    const ExtrudeBuilder builder;
    Result<ExtrudeBuildResult> buildResult = builder.buildBlindExtrude(id(), *sketch, distance_);
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
    result.addDiagnostic("Extrude generated display mesh and placeholder body");
    return result;
}

} // namespace mnx
