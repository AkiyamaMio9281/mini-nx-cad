#pragma once

#include "mnx/sketch/gui/Tool.h"

namespace mnx {

class LineTool final : public Tool {
public:
    [[nodiscard]] SketchToolKind kind() const override {
        return SketchToolKind::Line;
    }

    [[nodiscard]] std::string_view label() const override {
        return "Line";
    }

    void onMousePress(SketchController& controller, const SketchMouseEvent& event) override;
    void onMouseMove(SketchController& controller, const SketchMouseEvent& event) override;
    void onMouseRelease(SketchController& controller, const SketchMouseEvent& event) override;
    void cancel(SketchController& controller) override;
    [[nodiscard]] std::optional<ToolPreview> preview() const override;

private:
    std::optional<Point2> start_;
    Point2 current_;
};

} // namespace mnx
