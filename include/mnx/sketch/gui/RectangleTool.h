#pragma once

#include "mnx/sketch/gui/Tool.h"

namespace mnx {

class RectangleTool final : public Tool {
public:
    [[nodiscard]] SketchToolKind kind() const override {
        return SketchToolKind::Rectangle;
    }

    [[nodiscard]] std::string_view label() const override {
        return "Rectangle";
    }

    void onMousePress(SketchController& controller, const SketchMouseEvent& event) override;
    void onMouseMove(SketchController& controller, const SketchMouseEvent& event) override;
    void onMouseRelease(SketchController& controller, const SketchMouseEvent& event) override;
    void cancel(SketchController& controller) override;
    [[nodiscard]] std::optional<ToolPreview> preview() const override;

private:
    std::optional<Point2> firstCorner_;
    Point2 current_;
};

} // namespace mnx
