#include "mnx/sketch/gui/SketchRenderer.h"

#include "mnx/sketch/SketchArc.h"
#include "mnx/sketch/SketchCircle.h"
#include "mnx/sketch/SketchLine.h"
#include "mnx/sketch/SketchPoint.h"
#include "mnx/sketch/SketchSpline.h"

#include <QBrush>
#include <QPainter>
#include <QPen>
#include <QRectF>

#include <cmath>

namespace {

QRectF normalizedRect(const QPointF& first, const QPointF& second) {
    return QRectF(first, second).normalized();
}

} // namespace

namespace mnx {

void SketchRenderer::draw(
    QPainter& painter,
    const Sketch& sketch,
    const SelectionState& selection,
    const std::optional<GeometryRef>& hoverRef,
    const std::optional<SnapResult>& snap,
    const std::optional<ToolPreview>& preview
) const {
    painter.fillRect(painter.viewport(), QColor(250, 250, 248));
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen axisPen(QColor(220, 220, 216));
    axisPen.setWidthF(1.0);
    painter.setPen(axisPen);
    painter.drawLine(QPointF(0.0, toView(Point2()).y()), QPointF(painter.viewport().width(), toView(Point2()).y()));
    painter.drawLine(QPointF(toView(Point2()).x(), 0.0), QPointF(toView(Point2()).x(), painter.viewport().height()));

    for(const SketchEntity* entity : sketch.entities()) {
        const GeometryRef whole = GeometryRef::whole(entity->id());
        const bool selected = selection.contains(whole);
        const bool hovered = hoverRef && *hoverRef == whole;
        drawEntity(painter, *entity, selected, hovered);
    }

    if(preview) {
        drawPreview(painter, *preview);
    }

    if(snap) {
        QPen snapPen(QColor(20, 130, 120));
        snapPen.setWidthF(1.5);
        painter.setPen(snapPen);
        painter.setBrush(Qt::NoBrush);
        const QPointF center = toView(snap->point);
        painter.drawRect(QRectF(center.x() - 4.0, center.y() - 4.0, 8.0, 8.0));
    }
}

QPointF SketchRenderer::toView(const Point2& point) const {
    return {
        origin_.x() + point.x() * scale_,
        viewportHeight_ - origin_.y() - point.y() * scale_,
    };
}

Point2 SketchRenderer::toSketch(const QPointF& point) const {
    return {
        (point.x() - origin_.x()) / scale_,
        (viewportHeight_ - origin_.y() - point.y()) / scale_,
    };
}

void SketchRenderer::drawEntity(QPainter& painter, const SketchEntity& entity, bool selected, bool hovered) const {
    QPen pen(selected ? QColor(30, 95, 180) : QColor(44, 47, 51));
    if(hovered && !selected) {
        pen.setColor(QColor(230, 135, 40));
    }
    pen.setWidthF(selected || hovered ? 2.4 : 1.6);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    if(const auto* point = dynamic_cast<const SketchPoint*>(&entity)) {
        const QPointF p = toView(point->resolvePoint(GeometryRefKind::CenterPoint).value());
        painter.setBrush(selected ? QColor(30, 95, 180) : QColor(44, 47, 51));
        painter.drawEllipse(p, 3.5, 3.5);
        painter.setBrush(Qt::NoBrush);
        return;
    }

    if(const auto* line = dynamic_cast<const SketchLine*>(&entity)) {
        painter.drawLine(
            toView(line->resolvePoint(GeometryRefKind::StartPoint).value()),
            toView(line->resolvePoint(GeometryRefKind::EndPoint).value())
        );
        return;
    }

    if(const auto* circle = dynamic_cast<const SketchCircle*>(&entity)) {
        const QPointF center = toView(circle->resolvePoint(GeometryRefKind::CenterPoint).value());
        const double radius = circle->radius() * scale_;
        painter.drawEllipse(center, radius, radius);
        return;
    }

    if(const auto* arc = dynamic_cast<const SketchArc*>(&entity)) {
        const QPointF center = toView(arc->resolvePoint(GeometryRefKind::CenterPoint).value());
        const double radius = arc->radius() * scale_;
        QRectF rect(center.x() - radius, center.y() - radius, radius * 2.0, radius * 2.0);
        const int start = static_cast<int>(-arc->startAngle() * 180.0 / 3.14159265358979323846 * 16.0);
        const int span =
            static_cast<int>(-(arc->endAngle() - arc->startAngle()) * 180.0 / 3.14159265358979323846 * 16.0);
        painter.drawArc(rect, start, span);
        return;
    }

    if(const auto* spline = dynamic_cast<const SketchSpline*>(&entity)) {
        auto points = spline->controlPoints();
        for(std::size_t index = 1; index < points.size(); ++index) {
            painter.drawLine(toView(points[index - 1]), toView(points[index]));
        }
    }
}

void SketchRenderer::drawPreview(QPainter& painter, const ToolPreview& preview) const {
    QPen pen(QColor(25, 120, 170, 180));
    pen.setStyle(Qt::DashLine);
    pen.setWidthF(1.5);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    switch(preview.type) {
    case ToolPreviewType::Line:
        painter.drawLine(toView(preview.first), toView(preview.second));
        break;
    case ToolPreviewType::Circle: {
        const double radius = (preview.second - preview.first).norm() * scale_;
        painter.drawEllipse(toView(preview.first), radius, radius);
        break;
    }
    case ToolPreviewType::Rectangle:
        painter.drawRect(normalizedRect(toView(preview.first), toView(preview.second)));
        break;
    case ToolPreviewType::None:
        break;
    }
}

} // namespace mnx
