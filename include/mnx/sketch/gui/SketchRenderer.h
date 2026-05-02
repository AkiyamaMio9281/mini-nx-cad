#pragma once

#include "mnx/sketch/GeometryRef.h"
#include "mnx/sketch/SelectionState.h"
#include "mnx/sketch/Sketch.h"
#include "mnx/sketch/gui/SnapManager.h"
#include "mnx/sketch/gui/Tool.h"

#include <QPointF>
#include <optional>

class QPainter;

namespace mnx {

class SketchRenderer {
public:
    void draw(
        QPainter& painter,
        const Sketch& sketch,
        const SelectionState& selection,
        const std::optional<GeometryRef>& hoverRef,
        const std::optional<SnapResult>& snap,
        const std::optional<ToolPreview>& preview
    ) const;

    [[nodiscard]] QPointF toView(const Point2& point) const;
    [[nodiscard]] Point2 toSketch(const QPointF& point) const;

    void setViewportHeight(double viewportHeight) {
        viewportHeight_ = viewportHeight;
    }

    void setScale(double scale) {
        scale_ = scale;
    }

    void setOrigin(const QPointF& origin) {
        origin_ = origin;
    }

private:
    void drawEntity(QPainter& painter, const SketchEntity& entity, bool selected, bool hovered) const;
    void drawPreview(QPainter& painter, const ToolPreview& preview) const;

    double scale_ = 40.0;
    double viewportHeight_ = 0.0;
    QPointF origin_ {32.0, 32.0};
};

} // namespace mnx
