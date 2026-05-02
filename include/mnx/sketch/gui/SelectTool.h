#pragma once

#include "mnx/sketch/gui/Tool.h"

namespace mnx {

class SelectTool final : public Tool {
public:
    [[nodiscard]] SketchToolKind kind() const override {
        return SketchToolKind::Select;
    }

    [[nodiscard]] std::string_view label() const override {
        return "Select";
    }

    void onMousePress(SketchController& controller, const SketchMouseEvent& event) override;
    void onMouseMove(SketchController& controller, const SketchMouseEvent& event) override;
    void onMouseRelease(SketchController& controller, const SketchMouseEvent& event) override;
    void cancel(SketchController& controller) override;
};

} // namespace mnx
