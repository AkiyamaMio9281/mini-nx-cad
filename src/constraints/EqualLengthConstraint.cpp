#include "mnx/constraints/EqualLengthConstraint.h"

namespace mnx {

EqualLengthConstraint::EqualLengthConstraint(ConstraintId id, GeometryRef first, GeometryRef second, bool driving)
    : Constraint(id, ConstraintType::EqualLength, {first, second}, driving, "Equal Length") {}

Status EqualLengthConstraint::validate(const Sketch& sketch) const {
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
            return Status::invalidArgument("EqualLengthConstraint currently supports whole line refs only");
        }
    }
    return Status::ok();
}

std::unique_ptr<Constraint> EqualLengthConstraint::clone() const {
    return std::make_unique<EqualLengthConstraint>(*this);
}

} // namespace mnx
