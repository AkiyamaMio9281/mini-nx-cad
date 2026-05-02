#pragma once

#include "mnx/geometry/Circle2D.h"
#include "mnx/geometry/Line2D.h"
#include "mnx/geometry/Segment2D.h"

namespace mnx {

[[nodiscard]] Point2 projectPoint(const Point2& point, const Line2D& line);
[[nodiscard]] Point2 projectPoint(const Point2& point, const Segment2D& segment);
[[nodiscard]] Point2 projectPoint(const Point2& point, const Circle2D& circle);

} // namespace mnx
