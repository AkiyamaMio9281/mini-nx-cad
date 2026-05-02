#pragma once

#include "mnx/core/math/Vec2.h"
#include "mnx/geometry/Point2.h"

namespace mnx {

class RevolveAxis2D {
public:
    RevolveAxis2D() = default;
    RevolveAxis2D(const Point2& origin, const Vec2& direction);

    [[nodiscard]] bool isValid() const;
    [[nodiscard]] const Point2& origin() const;
    [[nodiscard]] const Vec2& direction() const;
    [[nodiscard]] double signedDistance(const Point2& point) const;
    [[nodiscard]] double axialCoordinate(const Point2& point) const;

private:
    Point2 origin_;
    Vec2 direction_;
    bool valid_ = false;
};

} // namespace mnx
