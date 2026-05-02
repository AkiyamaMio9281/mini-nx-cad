#include "mnx/constraints/AngleConstraint.h"

namespace mnx {

AngleConstraint::AngleConstraint(
    ConstraintId id,
    GeometryRef first,
    GeometryRef second,
    double angleRadians,
    bool driving
) : Constraint(id, ConstraintType::Angle, {first, second}, driving, "Angle"), angleRadians_(angleRadians) {}

Status AngleConstraint::validate(const Sketch& sketch) const {
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
            return Status::invalidArgument("AngleConstraint currently supports whole line refs only");
        }
    }
    return Status::ok();
}

std::unique_ptr<Constraint> AngleConstraint::clone() const {
    return std::make_unique<AngleConstraint>(*this);
}

} // namespace mnx
