#include "mnx/constraints/Dimension.h"

#include <utility>

namespace mnx {

Dimension::Dimension(
    ConstraintId constraintId,
    DimensionDisplayKind displayKind,
    Point2 labelPosition,
    std::string label
) : constraintId_(constraintId),
    displayKind_(displayKind),
    labelPosition_(labelPosition),
    label_(std::move(label)) {}

void Dimension::setLabel(std::string label) {
    label_ = std::move(label);
}

} // namespace mnx
