#include "mnx/sketch/gui/SelectTool.h"

#include "mnx/sketch/gui/SketchController.h"

namespace mnx {

void SelectTool::onMousePress(SketchController& controller, const SketchMouseEvent& event) {
    if(event.leftButton) {
        controller.selectAt(event.position, event.shiftModifier);
    }
}

void SelectTool::onMouseMove(SketchController& controller, const SketchMouseEvent& event) {
    controller.updateHover(event.position);
}

void SelectTool::onMouseRelease(SketchController&, const SketchMouseEvent&) {}

void SelectTool::cancel(SketchController&) {}

} // namespace mnx
