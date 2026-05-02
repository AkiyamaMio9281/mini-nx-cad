#include "mnx/constraints/HorizontalConstraint.h"

namespace mnx {

HorizontalConstraint::HorizontalConstraint(ConstraintId id, GeometryRef target, bool driving)
    : Constraint(id, ConstraintType::Horizontal, {target}, driving, "Horizontal") {}

Status HorizontalConstraint::validate(const Sketch& sketch) const {
    Status status = validateRefCount(1);
    if(!status.isOk()) {
        return status;
    }
    status = validateAllRefsExist(sketch);
    if(!status.isOk()) {
        return status;
    }
    if(!isEntityLikeRef(target().kind())) {
        return Status::invalidArgument("HorizontalConstraint requires a whole line-like entity ref");
    }

    const SketchEntity* entity = sketch.findEntity(target().entityId());
    if(entity == nullptr || entity->type() != SketchEntityType::Line) {
        return Status::invalidArgument("HorizontalConstraint currently supports sketch lines only");
    }
    return Status::ok();
}

std::unique_ptr<Constraint> HorizontalConstraint::clone() const {
    return std::make_unique<HorizontalConstraint>(*this);
}

} // namespace mnx
