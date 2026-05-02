#include "mnx/constraints/ParallelConstraint.h"

namespace mnx {

ParallelConstraint::ParallelConstraint(ConstraintId id, GeometryRef first, GeometryRef second, bool driving)
    : Constraint(id, ConstraintType::Parallel, {first, second}, driving, "Parallel") {}

Status ParallelConstraint::validate(const Sketch& sketch) const {
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
            return Status::invalidArgument("ParallelConstraint currently supports whole line refs only");
        }
    }
    return Status::ok();
}

std::unique_ptr<Constraint> ParallelConstraint::clone() const {
    return std::make_unique<ParallelConstraint>(*this);
}

} // namespace mnx
