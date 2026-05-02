#pragma once

#include "mnx/core/math/AABB2.h"
#include "mnx/geometry/Arc2D.h"
#include "mnx/geometry/Circle2D.h"
#include "mnx/geometry/Polyline2D.h"
#include "mnx/geometry/Segment2D.h"

#include <span>

namespace mnx {

[[nodiscard]] AABB2 boundsOf(std::span<const Point2> points);
[[nodiscard]] AABB2 boundsOf(const Segment2D& segment);
[[nodiscard]] AABB2 boundsOf(const Circle2D& circle);
[[nodiscard]] AABB2 boundsOf(const Arc2D& arc);
[[nodiscard]] AABB2 boundsOf(const Polyline2D& polyline);

} // namespace mnx
