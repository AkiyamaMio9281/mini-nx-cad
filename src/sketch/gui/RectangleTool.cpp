#include "mnx/sketch/gui/RectangleTool.h"

#include "mnx/sketch/gui/SketchController.h"

namespace mnx {

void RectangleTool::onMousePress(SketchController& controller, const SketchMouseEvent& event) {
    if(!event.leftButton) {
        return;
    }

    const Point2 point = controller.snappedPoint(event.position);
    if(!firstCorner_) {
        firstCorner_ = point;
        current_ = point;
        return;
    }

    controller.addRectangle(*firstCorner_, point);
    firstCorner_.reset();
}

void RectangleTool::onMouseMove(SketchController& controller, const SketchMouseEvent& event) {
    current_ = controller.snappedPoint(event.position);
}

void RectangleTool::onMouseRelease(SketchController&, const SketchMouseEvent&) {}

void RectangleTool::cancel(SketchController&) {
    firstCorner_.reset();
}

std::optional<ToolPreview> RectangleTool::preview() const {
    if(!firstCorner_) {
        return std::nullopt;
    }
    return ToolPreview {ToolPreviewType::Rectangle, *firstCorner_, current_};
}

} // namespace mnx
