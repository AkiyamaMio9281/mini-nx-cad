#pragma once

#include "mnx/core/math/AABB2.h"
#include "mnx/geometry/Point2.h"

namespace mnx {

class Segment2D {
public:
    constexpr Segment2D() = default;
    constexpr Segment2D(const Point2& start, const Point2& end) : start_(start), end_(end) {}

    [[nodiscard]] constexpr const Point2& start() const {
        return start_;
    }

    [[nodiscard]] constexpr const Point2& end() const {
        return end_;
    }

    [[nodiscard]] Vec2 vector() const {
        return end_ - start_;
    }

    [[nodiscard]] double length() const;
    [[nodiscard]] Point2 midpoint() const;
    [[nodiscard]] AABB2 bounds() const;
    [[nodiscard]] Point2 projection(const Point2& point) const;

private:
    Point2 start_;
    Point2 end_;
};

} // namespace mnx
