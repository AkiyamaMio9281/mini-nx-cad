#pragma once

#include "mnx/core/math/AABB2.h"
#include "mnx/geometry/Point2.h"

namespace mnx {

class Arc2D {
public:
    Arc2D() = default;
    Arc2D(const Point2& center, double radius, double startAngle, double endAngle, bool counterClockwise = true);

    [[nodiscard]] const Point2& center() const {
        return center_;
    }

    [[nodiscard]] double radius() const {
        return radius_;
    }

    [[nodiscard]] double startAngle() const {
        return startAngle_;
    }

    [[nodiscard]] double endAngle() const {
        return endAngle_;
    }

    [[nodiscard]] double sweepAngle() const {
        return sweepAngle_;
    }

    [[nodiscard]] Point2 startPoint() const;
    [[nodiscard]] Point2 endPoint() const;
    [[nodiscard]] Point2 evaluate(double t) const;
    [[nodiscard]] Vec2 tangent(double t) const;
    [[nodiscard]] bool containsAngle(double angle, double tolerance = kDefaultTolerance) const;
    [[nodiscard]] AABB2 bounds() const;

private:
    Point2 center_;
    double radius_ = 0.0;
    double startAngle_ = 0.0;
    double endAngle_ = 0.0;
    double sweepAngle_ = 0.0;
};

} // namespace mnx
