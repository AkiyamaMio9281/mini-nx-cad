#include "mnx/sketch/gui/CircleTool.h"

#include "mnx/sketch/gui/SketchController.h"

namespace mnx {

void CircleTool::onMousePress(SketchController& controller, const SketchMouseEvent& event) {
    if(!event.leftButton) {
        return;
    }

    const Point2 point = controller.snappedPoint(event.position);
    if(!center_) {
        center_ = point;
        current_ = point;
        return;
    }

    controller.addCircle(*center_, (point - *center_).norm());
    center_.reset();
}

void CircleTool::onMouseMove(SketchController& controller, const SketchMouseEvent& event) {
    current_ = controller.snappedPoint(event.position);
}

void CircleTool::onMouseRelease(SketchController&, const SketchMouseEvent&) {}

void CircleTool::cancel(SketchController&) {
    center_.reset();
}

std::optional<ToolPreview> CircleTool::preview() const {
    if(!center_) {
        return std::nullopt;
    }
    return ToolPreview {ToolPreviewType::Circle, *center_, current_};
}

} // namespace mnx
