#pragma once

#include "mnx/modeling/Body.h"
#include "mnx/modeling/DisplayMesh.h"
#include "mnx/modeling/Feature.h"
#include "mnx/modeling/RevolveAxis2D.h"

#include <optional>

namespace mnx {

class RevolveFeature final : public Feature {
public:
    RevolveFeature(
        FeatureId id,
        std::string name,
        FeatureId profileSketchFeatureId,
        RevolveAxis2D axis,
        double angleRadians
    );

    [[nodiscard]] FeatureId profileSketchFeatureId() const;
    void setProfileSketchFeatureId(FeatureId id);

    [[nodiscard]] const RevolveAxis2D& axis() const;
    void setAxis(RevolveAxis2D axis);

    [[nodiscard]] double angleRadians() const;
    void setAngleRadians(double angleRadians);

    [[nodiscard]] const std::optional<Body>& body() const;
    [[nodiscard]] const std::optional<DisplayMesh>& displayMesh() const;

    [[nodiscard]] FeatureResult execute(FeatureContext& context) override;

private:
    FeatureId profileSketchFeatureId_;
    RevolveAxis2D axis_;
    double angleRadians_ = 0.0;
    std::optional<Body> body_;
    std::optional<DisplayMesh> displayMesh_;
};

} // namespace mnx
