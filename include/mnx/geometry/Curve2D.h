#pragma once

#include "mnx/core/math/AABB2.h"
#include "mnx/geometry/Point2.h"

namespace mnx {

class Curve2D {
public:
    virtual ~Curve2D() = default;

    [[nodiscard]] virtual Point2 evaluate(double t) const = 0;
    [[nodiscard]] virtual Vec2 tangent(double t) const = 0;
    [[nodiscard]] virtual AABB2 bounds() const = 0;
};

} // namespace mnx
