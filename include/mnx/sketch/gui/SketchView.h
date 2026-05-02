#pragma once

#include "mnx/sketch/gui/SketchController.h"
#include "mnx/sketch/gui/SketchRenderer.h"

#include <QWidget>

namespace mnx {

class SketchView final : public QWidget {
public:
    explicit SketchView(SketchController& controller, QWidget* parent = nullptr);

    [[nodiscard]] Point2 mapToSketch(const QPoint& point) const;

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    [[nodiscard]] SketchMouseEvent makeMouseEvent(const QMouseEvent& event) const;

    SketchController& controller_;
    SketchRenderer renderer_;
};

} // namespace mnx
