#include "mnx/constraints/TangentConstraint.h"

namespace mnx {

TangentConstraint::TangentConstraint(ConstraintId id, GeometryRef first, GeometryRef second, bool driving)
    : Constraint(id, ConstraintType::Tangent, {first, second}, driving, "Tangent") {}

Status TangentConstraint::validate(const Sketch& sketch) const {
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
        if(ref.kind() != GeometryRefKind::WholeEntity || entity == nullptr) {
            return Status::invalidArgument("TangentConstraint requires whole entity refs");
        }
        const SketchEntityType type = entity->type();
        if(type != SketchEntityType::Line && type != SketchEntityType::Circle && type != SketchEntityType::Arc) {
            return Status::invalidArgument("TangentConstraint supports line, circle, and arc refs only");
        }
    }
    return Status::ok();
}

std::unique_ptr<Constraint> TangentConstraint::clone() const {
    return std::make_unique<TangentConstraint>(*this);
}

} // namespace mnx
