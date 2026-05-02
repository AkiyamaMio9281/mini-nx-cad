#include "mnx/commands/MoveSketchHandleCommand.h"

#include "mnx/sketch/SketchArc.h"
#include "mnx/sketch/SketchCircle.h"
#include "mnx/sketch/SketchLine.h"
#include "mnx/sketch/SketchPoint.h"

namespace mnx {

MoveSketchHandleCommand::MoveSketchHandleCommand(Sketch& sketch, GeometryRef ref, Point2 before, Point2 after)
    : sketch_(sketch), ref_(ref), before_(before), after_(after) {}

Status MoveSketchHandleCommand::execute() {
    return apply(after_);
}

Status MoveSketchHandleCommand::undo() {
    return apply(before_);
}

Status MoveSketchHandleCommand::apply(const Point2& point) {
    SketchEntity* entity = sketch_.findEntity(ref_.entityId());
    if(entity == nullptr) {
        return Status::notFound("Sketch entity was not found");
    }

    if(auto* sketchPoint = dynamic_cast<SketchPoint*>(entity)) {
        if(ref_.kind() == GeometryRefKind::CenterPoint || ref_.kind() == GeometryRefKind::WholeEntity) {
            sketchPoint->setPoint(point);
            return Status::ok();
        }
    }

    if(auto* line = dynamic_cast<SketchLine*>(entity)) {
        if(ref_.kind() == GeometryRefKind::StartPoint) {
            line->setStart(point);
            return Status::ok();
        }
        if(ref_.kind() == GeometryRefKind::EndPoint) {
            line->setEnd(point);
            return Status::ok();
        }
        if(ref_.kind() == GeometryRefKind::Midpoint || ref_.kind() == GeometryRefKind::WholeEntity) {
            const Vec2 delta = point - line->midpoint();
            line->setStart(line->start() + delta);
            line->setEnd(line->end() + delta);
            return Status::ok();
        }
    }

    if(auto* circle = dynamic_cast<SketchCircle*>(entity)) {
        if(ref_.kind() == GeometryRefKind::CenterPoint || ref_.kind() == GeometryRefKind::WholeEntity) {
            circle->setCenter(point);
            return Status::ok();
        }
    }

    if(auto* arc = dynamic_cast<SketchArc*>(entity)) {
        if(ref_.kind() == GeometryRefKind::CenterPoint) {
            arc->setCenter(point);
            return Status::ok();
        }
    }

    return Status::invalidArgument("Unsupported sketch handle move");
}

} // namespace mnx
