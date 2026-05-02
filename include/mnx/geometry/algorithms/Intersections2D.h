#pragma once

#include "mnx/geometry/Circle2D.h"
#include "mnx/geometry/Line2D.h"
#include "mnx/geometry/Segment2D.h"

#include <vector>

namespace mnx {

enum class IntersectionType {
    None,
    Point,
    Tangent,
    Overlap,
    Parallel,
    Coincident,
};

struct Intersection2DResult {
    IntersectionType type = IntersectionType::None;
    std::vector<Point2> points;

    [[nodiscard]] bool hasIntersection() const {
        return type == IntersectionType::Point || type == IntersectionType::Tangent
            || type == IntersectionType::Overlap || type == IntersectionType::Coincident;
    }
};

[[nodiscard]] Intersection2DResult intersect(const Line2D& lhs, const Line2D& rhs);
[[nodiscard]] Intersection2DResult intersect(const Segment2D& lhs, const Segment2D& rhs);
[[nodiscard]] Intersection2DResult intersect(const Line2D& line, const Circle2D& circle);
[[nodiscard]] Intersection2DResult intersect(const Circle2D& lhs, const Circle2D& rhs);
[[nodiscard]] Intersection2DResult intersect(const Segment2D& segment, const Circle2D& circle);

} // namespace mnx
