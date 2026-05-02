#pragma once

#include "mnx/constraints/Constraint.h"

namespace mnx {

class AngleConstraint final : public Constraint {
public:
    AngleConstraint(
        ConstraintId id,
        GeometryRef first,
        GeometryRef second,
        double angleRadians,
        bool driving = true
    );

    [[nodiscard]] double angleRadians() const {
        return angleRadians_;
    }

    void setAngleRadians(double angleRadians) {
        angleRadians_ = angleRadians;
    }

    [[nodiscard]] Status validate(const Sketch& sketch) const override;
    [[nodiscard]] std::unique_ptr<Constraint> clone() const override;

private:
    double angleRadians_ = 0.0;
};

} // namespace mnx
