#include "mnx/modeling/SketchFeature.h"

#include "mnx/modeling/FeatureContext.h"

#include <utility>

namespace mnx {

SketchFeature::SketchFeature(FeatureId id, std::string name, Sketch sketch)
    : Feature(id, FeatureType::Sketch, std::move(name)), sketch_(std::move(sketch)) {}

Sketch& SketchFeature::sketch() {
    return sketch_;
}

const Sketch& SketchFeature::sketch() const {
    return sketch_;
}

FeatureResult SketchFeature::execute(FeatureContext& context) {
    context.recordExecutedFeature(id());
    context.recordSketch(id(), sketch_);
    FeatureResult result = FeatureResult::success();
    if(sketch_.entityCount() == 0) {
        result.addDiagnostic("Sketch feature contains no sketch entities");
    }
    return result;
}

} // namespace mnx
