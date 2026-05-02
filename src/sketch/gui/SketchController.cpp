#include "mnx/sketch/gui/SketchController.h"

#include "mnx/sketch/gui/CircleTool.h"
#include "mnx/sketch/gui/LineTool.h"
#include "mnx/sketch/gui/RectangleTool.h"
#include "mnx/sketch/gui/SelectTool.h"
#include "mnx/commands/CompositeCommand.h"
#include "mnx/commands/CreateSketchEntityCommand.h"
#include "mnx/sketch/SketchCircle.h"
#include "mnx/sketch/SketchLine.h"

#include <cmath>
#include <utility>

namespace mnx {

SketchController::SketchController() : activeTool_(makeTool(SketchToolKind::Select)) {
    setStatus("Select");
}

SketchController::~SketchController() = default;

std::optional<ToolPreview> SketchController::activePreview() const {
    return activeTool_ ? activeTool_->preview() : std::nullopt;
}

SketchToolKind SketchController::activeToolKind() const {
    return activeTool_ ? activeTool_->kind() : SketchToolKind::Select;
}

void SketchController::setRefreshCallback(std::function<void()> callback) {
    refreshCallback_ = std::move(callback);
}

void SketchController::setStatusCallback(std::function<void(std::string)> callback) {
    statusCallback_ = std::move(callback);
}

void SketchController::setActiveTool(SketchToolKind kind) {
    if(activeTool_) {
        activeTool_->cancel(*this);
    }
    activeTool_ = makeTool(kind);
    setStatus(std::string(activeTool_->label()));
    requestRefresh();
}

void SketchController::handleMousePress(const SketchMouseEvent& event) {
    if(activeTool_) {
        activeTool_->onMousePress(*this, event);
    }
    requestRefresh();
}

void SketchController::handleMouseMove(const SketchMouseEvent& event) {
    activeSnap_ = snapManager_.snap(sketch_, event.position, hitTolerance_);
    updateHover(event.position);
    if(activeTool_) {
        activeTool_->onMouseMove(*this, event);
    }
    requestRefresh();
}

void SketchController::handleMouseRelease(const SketchMouseEvent& event) {
    if(activeTool_) {
        activeTool_->onMouseRelease(*this, event);
    }
    requestRefresh();
}

void SketchController::cancelActiveTool() {
    if(activeTool_) {
        activeTool_->cancel(*this);
    }
    requestRefresh();
}

Point2 SketchController::snappedPoint(const Point2& point) {
    activeSnap_ = snapManager_.snap(sketch_, point, hitTolerance_);
    return activeSnap_ ? activeSnap_->point : point;
}

void SketchController::updateHover(const Point2& point) {
    const auto hit = selectionManager_.hitTest(sketch_, point, hitTolerance_);
    hoverRef_ = hit ? std::optional<GeometryRef>(hit->ref) : std::nullopt;
}

void SketchController::selectAt(const Point2& point, bool toggle) {
    const auto hit = selectionManager_.hitTest(sketch_, point, hitTolerance_);
    if(!hit) {
        if(!toggle) {
            selectionManager_.clear();
        }
        return;
    }

    if(toggle) {
        selectionManager_.toggleSelection(hit->ref);
    } else {
        selectionManager_.replaceSelection(hit->ref);
    }
}

void SketchController::addLine(const Point2& start, const Point2& end) {
    if((end - start).norm() <= kDefaultTolerance) {
        return;
    }
    SketchLine& line = sketch_.addLine(start, end);
    auto entity = line.clone();
    (void)sketch_.removeEntity(line.id());
    (void)commandManager_.execute(std::make_unique<CreateSketchEntityCommand>(sketch_, std::move(entity)));
    setStatus("Line added");
    requestRefresh();
}

void SketchController::addCircle(const Point2& center, double radius) {
    if(radius <= kDefaultTolerance) {
        return;
    }
    SketchCircle& circle = sketch_.addCircle(center, radius);
    auto entity = circle.clone();
    (void)sketch_.removeEntity(circle.id());
    (void)commandManager_.execute(std::make_unique<CreateSketchEntityCommand>(sketch_, std::move(entity)));
    setStatus("Circle added");
    requestRefresh();
}

void SketchController::addRectangle(const Point2& firstCorner, const Point2& oppositeCorner) {
    if(firstCorner.isApprox(oppositeCorner)) {
        return;
    }

    const Point2 second(oppositeCorner.x(), firstCorner.y());
    const Point2 third(oppositeCorner.x(), oppositeCorner.y());
    const Point2 fourth(firstCorner.x(), oppositeCorner.y());

    auto composite = std::make_unique<CompositeCommand>("Create Rectangle");
    SketchLine& first = sketch_.addLine(firstCorner, second);
    auto firstEntity = first.clone();
    (void)sketch_.removeEntity(first.id());
    SketchLine& secondLine = sketch_.addLine(second, third);
    auto secondEntity = secondLine.clone();
    (void)sketch_.removeEntity(secondLine.id());
    SketchLine& thirdLine = sketch_.addLine(third, fourth);
    auto thirdEntity = thirdLine.clone();
    (void)sketch_.removeEntity(thirdLine.id());
    SketchLine& fourthLine = sketch_.addLine(fourth, firstCorner);
    auto fourthEntity = fourthLine.clone();
    (void)sketch_.removeEntity(fourthLine.id());

    composite->add(std::make_unique<CreateSketchEntityCommand>(sketch_, std::move(firstEntity)));
    composite->add(std::make_unique<CreateSketchEntityCommand>(sketch_, std::move(secondEntity)));
    composite->add(std::make_unique<CreateSketchEntityCommand>(sketch_, std::move(thirdEntity)));
    composite->add(std::make_unique<CreateSketchEntityCommand>(sketch_, std::move(fourthEntity)));
    (void)commandManager_.execute(std::move(composite));
    setStatus("Rectangle added");
    requestRefresh();
}

void SketchController::undo() {
    const Status status = commandManager_.undo();
    setStatus(status.isOk() ? "Undo" : status.message());
    requestRefresh();
}

void SketchController::redo() {
    const Status status = commandManager_.redo();
    setStatus(status.isOk() ? "Redo" : status.message());
    requestRefresh();
}

void SketchController::requestRefresh() {
    if(refreshCallback_) {
        refreshCallback_();
    }
}

void SketchController::setStatus(std::string statusText) {
    statusText_ = std::move(statusText);
    if(statusCallback_) {
        statusCallback_(statusText_);
    }
}

std::unique_ptr<Tool> SketchController::makeTool(SketchToolKind kind) const {
    switch(kind) {
    case SketchToolKind::Select:
        return std::make_unique<SelectTool>();
    case SketchToolKind::Line:
        return std::make_unique<LineTool>();
    case SketchToolKind::Circle:
        return std::make_unique<CircleTool>();
    case SketchToolKind::Rectangle:
        return std::make_unique<RectangleTool>();
    }
    return std::make_unique<SelectTool>();
}

} // namespace mnx
