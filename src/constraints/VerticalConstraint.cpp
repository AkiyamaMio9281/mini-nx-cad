#include "mnx/constraints/VerticalConstraint.h"

namespace mnx {

VerticalConstraint::VerticalConstraint(ConstraintId id, GeometryRef target, bool driving)
    : Constraint(id, ConstraintType::Vertical, {target}, driving, "Vertical") {}

Status VerticalConstraint::validate(const Sketch& sketch) const {
    Status status = validateRefCount(1);
    if(!status.isOk()) {
        return status;
    }
    status = validateAllRefsExist(sketch);
    if(!status.isOk()) {
        return status;
    }
    if(!isEntityLikeRef(target().kind())) {
        return Status::invalidArgument("VerticalConstraint requires a whole line-like entity ref");
    }

    const SketchEntity* entity = sketch.findEntity(target().entityId());
    if(entity == nullptr || entity->type() != SketchEntityType::Line) {
        return Status::invalidArgument("VerticalConstraint currently supports sketch lines only");
    }
    return Status::ok();
}

std::unique_ptr<Constraint> VerticalConstraint::clone() const {
    return std::make_unique<VerticalConstraint>(*this);
}

} // namespace mnx
