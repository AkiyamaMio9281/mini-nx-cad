#include "mnx/sketch/gui/LineTool.h"

#include "mnx/sketch/gui/SketchController.h"

namespace mnx {

void LineTool::onMousePress(SketchController& controller, const SketchMouseEvent& event) {
    if(!event.leftButton) {
        return;
    }

    const Point2 point = controller.snappedPoint(event.position);
    if(!start_) {
        start_ = point;
        current_ = point;
        return;
    }

    controller.addLine(*start_, point);
    start_.reset();
}

void LineTool::onMouseMove(SketchController& controller, const SketchMouseEvent& event) {
    current_ = controller.snappedPoint(event.position);
}

void LineTool::onMouseRelease(SketchController&, const SketchMouseEvent&) {}

void LineTool::cancel(SketchController&) {
    start_.reset();
}

std::optional<ToolPreview> LineTool::preview() const {
    if(!start_) {
        return std::nullopt;
    }
    return ToolPreview {ToolPreviewType::Line, *start_, current_};
}

} // namespace mnx
