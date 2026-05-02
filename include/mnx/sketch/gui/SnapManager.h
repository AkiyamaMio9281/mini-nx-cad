#pragma once

#include "mnx/sketch/GeometryRef.h"
#include "mnx/sketch/Sketch.h"

#include <optional>

namespace mnx {

enum class SnapKind {
    Endpoint,
    Midpoint,
    Center,
};

struct SnapResult {
    Point2 point;
    GeometryRef ref;
    SnapKind kind = SnapKind::Endpoint;
    double distance = 0.0;
};

class SnapManager {
public:
    [[nodiscard]] std::optional<SnapResult> snap(
        const Sketch& sketch,
        const Point2& position,
        double tolerance
    ) const;
};

} // namespace mnx
