#pragma once

#include "mnx/core/ids/BodyId.h"
#include "mnx/core/ids/FeatureId.h"
#include "mnx/modeling/Body.h"
#include "mnx/sketch/Sketch.h"

#include <string>
#include <utility>
#include <vector>

namespace mnx {

class FeatureContext {
public:
    void clear();

    void recordExecutedFeature(FeatureId id);
    [[nodiscard]] const std::vector<FeatureId>& executedFeatureIds() const;

    void recordBody(BodyId id);
    [[nodiscard]] const std::vector<BodyId>& bodyIds() const;
    void recordBody(Body body);
    [[nodiscard]] const std::vector<Body>& bodies() const;

    void recordSketch(FeatureId id, const Sketch& sketch);
    [[nodiscard]] const Sketch* findSketch(FeatureId id) const;

    void addDiagnostic(std::string diagnostic);
    [[nodiscard]] const std::vector<std::string>& diagnostics() const;

private:
    std::vector<FeatureId> executedFeatureIds_;
    std::vector<BodyId> bodyIds_;
    std::vector<Body> bodies_;
    std::vector<std::pair<FeatureId, const Sketch*>> sketches_;
    std::vector<std::string> diagnostics_;
};

} // namespace mnx
