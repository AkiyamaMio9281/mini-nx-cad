#pragma once

#include "mnx/geometry/Point2.h"

namespace mnx {

class Line2D {
public:
    Line2D(const Point2& point, const Vec2& direction);

    [[nodiscard]] const Point2& point() const {
        return point_;
    }

    [[nodiscard]] const Vec2& direction() const {
        return direction_;
    }

    [[nodiscard]] Point2 evaluate(double parameter) const;
    [[nodiscard]] Point2 projection(const Point2& point) const;
    [[nodiscard]] double distanceTo(const Point2& point) const;

private:
    Point2 point_;
    Vec2 direction_ {1.0, 0.0};
};

} // namespace mnx
