#pragma once

#include "mnx/modeling/Body.h"
#include "mnx/modeling/DisplayMesh.h"
#include "mnx/modeling/Feature.h"

#include <optional>

namespace mnx {

class ExtrudeFeature final : public Feature {
public:
    ExtrudeFeature(FeatureId id, std::string name, FeatureId profileSketchFeatureId, double distance);

    [[nodiscard]] FeatureId profileSketchFeatureId() const;
    void setProfileSketchFeatureId(FeatureId id);

    [[nodiscard]] double distance() const;
    void setDistance(double distance);

    [[nodiscard]] const std::optional<Body>& body() const;
    [[nodiscard]] const std::optional<DisplayMesh>& displayMesh() const;

    [[nodiscard]] FeatureResult execute(FeatureContext& context) override;

private:
    FeatureId profileSketchFeatureId_;
    double distance_ = 0.0;
    std::optional<Body> body_;
    std::optional<DisplayMesh> displayMesh_;
};

} // namespace mnx
