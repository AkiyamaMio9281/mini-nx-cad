#pragma once

#include "mnx/constraints/Constraint.h"

namespace mnx {

class ParallelConstraint final : public Constraint {
public:
    ParallelConstraint(ConstraintId id, GeometryRef first, GeometryRef second, bool driving = true);

    [[nodiscard]] Status validate(const Sketch& sketch) const override;
    [[nodiscard]] std::unique_ptr<Constraint> clone() const override;
};

} // namespace mnx
