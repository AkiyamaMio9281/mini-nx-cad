#pragma once

#include "mnx/sketch/Sketch.h"
#include "mnx/commands/CommandManager.h"
#include "mnx/sketch/gui/SelectionManager.h"
#include "mnx/sketch/gui/SnapManager.h"
#include "mnx/sketch/gui/Tool.h"

#include <functional>
#include <memory>
#include <optional>
#include <string>

namespace mnx {

class SketchController {
public:
    SketchController();
    ~SketchController();

    [[nodiscard]] Sketch& sketch() {
        return sketch_;
    }

    [[nodiscard]] const Sketch& sketch() const {
        return sketch_;
    }

    [[nodiscard]] const SelectionManager& selectionManager() const {
        return selectionManager_;
    }

    [[nodiscard]] const std::optional<GeometryRef>& hoverRef() const {
        return hoverRef_;
    }

    [[nodiscard]] const std::optional<SnapResult>& activeSnap() const {
        return activeSnap_;
    }

    [[nodiscard]] std::optional<ToolPreview> activePreview() const;
    [[nodiscard]] SketchToolKind activeToolKind() const;
    [[nodiscard]] const std::string& statusText() const {
        return statusText_;
    }

    [[nodiscard]] CommandManager& commandManager() {
        return commandManager_;
    }

    [[nodiscard]] const CommandManager& commandManager() const {
        return commandManager_;
    }

    void setRefreshCallback(std::function<void()> callback);
    void setStatusCallback(std::function<void(std::string)> callback);

    void setActiveTool(SketchToolKind kind);
    void handleMousePress(const SketchMouseEvent& event);
    void handleMouseMove(const SketchMouseEvent& event);
    void handleMouseRelease(const SketchMouseEvent& event);
    void cancelActiveTool();

    [[nodiscard]] Point2 snappedPoint(const Point2& point);
    void updateHover(const Point2& point);
    void selectAt(const Point2& point, bool toggle);

    void addLine(const Point2& start, const Point2& end);
    void addCircle(const Point2& center, double radius);
    void addRectangle(const Point2& firstCorner, const Point2& oppositeCorner);
    void undo();
    void redo();

private:
    void requestRefresh();
    void setStatus(std::string statusText);
    [[nodiscard]] std::unique_ptr<Tool> makeTool(SketchToolKind kind) const;

    Sketch sketch_;
    CommandManager commandManager_;
    SelectionManager selectionManager_;
    SnapManager snapManager_;
    std::unique_ptr<Tool> activeTool_;
    std::optional<GeometryRef> hoverRef_;
    std::optional<SnapResult> activeSnap_;
    std::function<void()> refreshCallback_;
    std::function<void(std::string)> statusCallback_;
    std::string statusText_;
    double hitTolerance_ = 0.2;
};

} // namespace mnx
