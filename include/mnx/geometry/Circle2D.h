#pragma once

#include "mnx/core/math/AABB2.h"
#include "mnx/geometry/Point2.h"

namespace mnx {

class Circle2D {
public:
    Circle2D() = default;
    Circle2D(const Point2& center, double radius);

    [[nodiscard]] const Point2& center() const {
        return center_;
    }

    [[nodiscard]] double radius() const {
        return radius_;
    }

    [[nodiscard]] Point2 evaluate(double angleRadians) const;
    [[nodiscard]] AABB2 bounds() const;

private:
    Point2 center_;
    double radius_ = 0.0;
};

} // namespace mnx
