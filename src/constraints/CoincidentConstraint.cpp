#include "mnx/constraints/CoincidentConstraint.h"

namespace mnx {

CoincidentConstraint::CoincidentConstraint(ConstraintId id, GeometryRef first, GeometryRef second, bool driving)
    : Constraint(id, ConstraintType::Coincident, {first, second}, driving, "Coincident") {}

Status CoincidentConstraint::validate(const Sketch& sketch) const {
    Status status = validateRefCount(2);
    if(!status.isOk()) {
        return status;
    }
    status = validateAllRefsExist(sketch);
    if(!status.isOk()) {
        return status;
    }
    if(!isPointLikeRef(first().kind()) || !isPointLikeRef(second().kind())) {
        return Status::invalidArgument("CoincidentConstraint requires point-like geometry refs");
    }
    return Status::ok();
}

std::unique_ptr<Constraint> CoincidentConstraint::clone() const {
    return std::make_unique<CoincidentConstraint>(*this);
}

} // namespace mnx
