#pragma once

#include "mnx/sketch/GeometryRef.h"
#include "mnx/sketch/SelectionState.h"
#include "mnx/sketch/Sketch.h"

#include <optional>

namespace mnx {

struct HitTestResult {
    GeometryRef ref;
    double distance = 0.0;
};

class SelectionManager {
public:
    [[nodiscard]] const SelectionState& selection() const {
        return selection_;
    }

    [[nodiscard]] std::optional<HitTestResult> hitTest(
        const Sketch& sketch,
        const Point2& position,
        double tolerance
    ) const;

    void replaceSelection(const GeometryRef& ref);
    void toggleSelection(const GeometryRef& ref);
    void clear();

private:
    SelectionState selection_;
};

} // namespace mnx
