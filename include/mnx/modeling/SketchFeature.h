#pragma once

#include "mnx/modeling/Feature.h"
#include "mnx/sketch/Sketch.h"

namespace mnx {

class SketchFeature final : public Feature {
public:
    SketchFeature(FeatureId id, std::string name, Sketch sketch = Sketch());

    [[nodiscard]] Sketch& sketch();
    [[nodiscard]] const Sketch& sketch() const;

    [[nodiscard]] FeatureResult execute(FeatureContext& context) override;

private:
    Sketch sketch_;
};

} // namespace mnx
