#pragma once

#include "mnx/constraints/Constraint.h"

namespace mnx {

class HorizontalConstraint final : public Constraint {
public:
    HorizontalConstraint(ConstraintId id, GeometryRef target, bool driving = true);

    [[nodiscard]] const GeometryRef& target() const {
        return refs()[0];
    }

    [[nodiscard]] Status validate(const Sketch& sketch) const override;
    [[nodiscard]] std::unique_ptr<Constraint> clone() const override;
};

} // namespace mnx
