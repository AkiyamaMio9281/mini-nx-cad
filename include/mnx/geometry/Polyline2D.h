#pragma once

#include "mnx/core/math/AABB2.h"
#include "mnx/geometry/Point2.h"

#include <span>
#include <vector>
#include <utility>

namespace mnx {

class Polyline2D {
public:
    Polyline2D() = default;
    explicit Polyline2D(std::vector<Point2> points);

    [[nodiscard]] std::span<const Point2> points() const {
        return points_;
    }

    [[nodiscard]] bool empty() const {
        return points_.empty();
    }

    [[nodiscard]] bool isClosed(double tolerance = kDefaultTolerance) const;
    [[nodiscard]] double length() const;
    [[nodiscard]] AABB2 bounds() const;
    [[nodiscard]] Point2 evaluate(double t) const;

private:
    std::vector<Point2> points_;
};

} // namespace mnx
