#include "mnx/sketch/gui/SketchView.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>

namespace mnx {

SketchView::SketchView(SketchController& controller, QWidget* parent)
    : QWidget(parent), controller_(controller) {
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(640, 480);

    controller_.setRefreshCallback([this]() {
        update();
    });
}

Point2 SketchView::mapToSketch(const QPoint& point) const {
    return renderer_.toSketch(QPointF(point));
}

void SketchView::paintEvent(QPaintEvent*) {
    renderer_.setViewportHeight(static_cast<double>(height()));

    QPainter painter(this);
    renderer_.draw(
        painter,
        controller_.sketch(),
        controller_.selectionManager().selection(),
        controller_.hoverRef(),
        controller_.activeSnap(),
        controller_.activePreview()
    );
}

void SketchView::mousePressEvent(QMouseEvent* event) {
    setFocus();
    controller_.handleMousePress(makeMouseEvent(*event));
}

void SketchView::mouseMoveEvent(QMouseEvent* event) {
    controller_.handleMouseMove(makeMouseEvent(*event));
}

void SketchView::mouseReleaseEvent(QMouseEvent* event) {
    controller_.handleMouseRelease(makeMouseEvent(*event));
}

void SketchView::keyPressEvent(QKeyEvent* event) {
    if(event->key() == Qt::Key_Escape) {
        controller_.cancelActiveTool();
        return;
    }
    QWidget::keyPressEvent(event);
}

SketchMouseEvent SketchView::makeMouseEvent(const QMouseEvent& event) const {
    return {
        mapToSketch(event.position().toPoint()),
        event.button() == Qt::LeftButton || event.buttons().testFlag(Qt::LeftButton),
        event.modifiers().testFlag(Qt::ShiftModifier),
    };
}

} // namespace mnx
