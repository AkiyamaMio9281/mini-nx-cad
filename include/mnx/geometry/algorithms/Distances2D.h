#pragma once

#include "mnx/geometry/Circle2D.h"
#include "mnx/geometry/Line2D.h"
#include "mnx/geometry/Segment2D.h"

namespace mnx {

[[nodiscard]] double distance(const Point2& lhs, const Point2& rhs);
[[nodiscard]] double distance(const Point2& point, const Line2D& line);
[[nodiscard]] double distance(const Point2& point, const Segment2D& segment);
[[nodiscard]] double distance(const Point2& point, const Circle2D& circle);

} // namespace mnx
