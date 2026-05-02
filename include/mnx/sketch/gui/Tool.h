#pragma once

#include "mnx/geometry/Point2.h"

#include <optional>
#include <string_view>

namespace mnx {

class SketchController;

enum class SketchToolKind {
    Select,
    Line,
    Circle,
    Rectangle,
};

struct SketchMouseEvent {
    Point2 position;
    bool leftButton = false;
    bool shiftModifier = false;
};

enum class ToolPreviewType {
    None,
    Line,
    Circle,
    Rectangle,
};

struct ToolPreview {
    ToolPreviewType type = ToolPreviewType::None;
    Point2 first;
    Point2 second;
};

class Tool {
public:
    virtual ~Tool() = default;

    [[nodiscard]] virtual SketchToolKind kind() const = 0;
    [[nodiscard]] virtual std::string_view label() const = 0;

    virtual void onMousePress(SketchController& controller, const SketchMouseEvent& event) = 0;
    virtual void onMouseMove(SketchController& controller, const SketchMouseEvent& event) = 0;
    virtual void onMouseRelease(SketchController& controller, const SketchMouseEvent& event) = 0;
    virtual void cancel(SketchController& controller) = 0;

    [[nodiscard]] virtual std::optional<ToolPreview> preview() const {
        return std::nullopt;
    }
};

} // namespace mnx
