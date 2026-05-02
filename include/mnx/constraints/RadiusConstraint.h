#pragma once

#include "mnx/constraints/Constraint.h"

namespace mnx {

class RadiusConstraint final : public Constraint {
public:
    RadiusConstraint(ConstraintId id, GeometryRef target, double radius, bool driving = true);

    [[nodiscard]] double radius() const {
        return radius_;
    }

    void setRadius(double radius);

    [[nodiscard]] Status validate(const Sketch& sketch) const override;
    [[nodiscard]] std::unique_ptr<Constraint> clone() const override;

private:
    double radius_ = 0.0;
};

} // namespace mnx
