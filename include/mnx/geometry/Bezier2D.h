#pragma once

#include "mnx/geometry/Curve2D.h"

#include <array>

namespace mnx {

class Bezier2D final : public Curve2D {
public:
    using ControlPoints = std::array<Point2, 4>;

    explicit Bezier2D(const ControlPoints& controlPoints);

    [[nodiscard]] const ControlPoints& controlPoints() const {
        return controlPoints_;
    }

    [[nodiscard]] Point2 evaluate(double t) const override;
    [[nodiscard]] Vec2 tangent(double t) const override;
    [[nodiscard]] AABB2 bounds() const override;

private:
    ControlPoints controlPoints_;
};

} // namespace mnx
