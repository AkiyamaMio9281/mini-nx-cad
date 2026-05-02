#pragma once

#include "mnx/core/math/AABB3.h"
#include "mnx/geometry/Point3.h"

namespace mnx {

class Surface {
public:
    virtual ~Surface() = default;

    [[nodiscard]] virtual Point3 evaluate(double u, double v) const = 0;
    [[nodiscard]] virtual Vec3 normal(double u, double v) const = 0;
    [[nodiscard]] virtual AABB3 bounds() const = 0;
};

} // namespace mnx
