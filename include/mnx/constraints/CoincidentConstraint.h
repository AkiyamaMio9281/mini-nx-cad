#pragma once

#include "mnx/constraints/Constraint.h"

namespace mnx {

class CoincidentConstraint final : public Constraint {
public:
    CoincidentConstraint(ConstraintId id, GeometryRef first, GeometryRef second, bool driving = true);

    [[nodiscard]] const GeometryRef& first() const {
        return refs()[0];
    }

    [[nodiscard]] const GeometryRef& second() const {
        return refs()[1];
    }

    [[nodiscard]] Status validate(const Sketch& sketch) const override;
    [[nodiscard]] std::unique_ptr<Constraint> clone() const override;
};

} // namespace mnx
