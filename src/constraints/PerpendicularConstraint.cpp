#include "mnx/constraints/PerpendicularConstraint.h"

namespace mnx {

PerpendicularConstraint::PerpendicularConstraint(ConstraintId id, GeometryRef first, GeometryRef second, bool driving)
    : Constraint(id, ConstraintType::Perpendicular, {first, second}, driving, "Perpendicular") {}

Status PerpendicularConstraint::validate(const Sketch& sketch) const {
    Status status = validateRefCount(2);
    if(!status.isOk()) {
        return status;
    }
    status = validateAllRefsExist(sketch);
    if(!status.isOk()) {
        return status;
    }
    for(const GeometryRef& ref : refs()) {
        const SketchEntity* entity = sketch.findEntity(ref.entityId());
        if(ref.kind() != GeometryRefKind::WholeEntity || entity == nullptr || entity->type() != SketchEntityType::Line) {
            return Status::invalidArgument("PerpendicularConstraint currently supports whole line refs only");
        }
    }
    return Status::ok();
}

std::unique_ptr<Constraint> PerpendicularConstraint::clone() const {
    return std::make_unique<PerpendicularConstraint>(*this);
}

} // namespace mnx
