#pragma once

#include "mnx/geometry/Curve2D.h"

#include <cstddef>
#include <vector>

namespace mnx {

[[nodiscard]] std::vector<Point2> sampleCurve(const Curve2D& curve, std::size_t sampleCount);

} // namespace mnx
