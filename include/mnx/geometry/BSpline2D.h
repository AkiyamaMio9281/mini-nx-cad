#pragma once

#include "mnx/geometry/Curve2D.h"

#include <span>
#include <vector>

namespace mnx {

class BSpline2D final : public Curve2D {
public:
    BSpline2D(std::vector<Point2> controlPoints, int degree);

    [[nodiscard]] std::span<const Point2> controlPoints() const {
        return controlPoints_;
    }

    [[nodiscard]] int degree() const {
        return degree_;
    }

    [[nodiscard]] Point2 evaluate(double t) const override;
    [[nodiscard]] Vec2 tangent(double t) const override;
    [[nodiscard]] AABB2 bounds() const override;

private:
    std::vector<Point2> controlPoints_;
    int degree_ = 0;
};

} // namespace mnx
