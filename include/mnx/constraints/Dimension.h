#pragma once

#include "mnx/core/ids/ConstraintId.h"
#include "mnx/geometry/Point2.h"

#include <string>

namespace mnx {

enum class DimensionDisplayKind {
    Distance,
    Angle,
    Radius,
};

class Dimension {
public:
    Dimension(
        ConstraintId constraintId,
        DimensionDisplayKind displayKind,
        Point2 labelPosition,
        std::string label = {}
    );

    [[nodiscard]] ConstraintId constraintId() const {
        return constraintId_;
    }

    [[nodiscard]] DimensionDisplayKind displayKind() const {
        return displayKind_;
    }

    [[nodiscard]] const Point2& labelPosition() const {
        return labelPosition_;
    }

    void setLabelPosition(const Point2& labelPosition) {
        labelPosition_ = labelPosition;
    }

    [[nodiscard]] const std::string& label() const {
        return label_;
    }

    void setLabel(std::string label);

private:
    ConstraintId constraintId_;
    DimensionDisplayKind displayKind_;
    Point2 labelPosition_;
    std::string label_;
};

} // namespace mnx
