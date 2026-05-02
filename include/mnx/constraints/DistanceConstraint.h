#pragma once

#include "mnx/constraints/Constraint.h"

namespace mnx {

class DistanceConstraint final : public Constraint {
public:
    DistanceConstraint(
        ConstraintId id,
        GeometryRef first,
        GeometryRef second,
        double distance,
        bool driving = true
    );

    [[nodiscard]] double distance() const {
        return distance_;
    }

    void setDistance(double distance);

    [[nodiscard]] Status validate(const Sketch& sketch) const override;
    [[nodiscard]] std::unique_ptr<Constraint> clone() const override;

private:
    double distance_ = 0.0;
};

} // namespace mnx
