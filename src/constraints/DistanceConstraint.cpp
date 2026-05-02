#include "mnx/constraints/DistanceConstraint.h"

#include <stdexcept>

namespace mnx {

DistanceConstraint::DistanceConstraint(
    ConstraintId id,
    GeometryRef first,
    GeometryRef second,
    double distance,
    bool driving
) : Constraint(id, ConstraintType::Distance, {first, second}, driving, "Distance") {
    setDistance(distance);
}

void DistanceConstraint::setDistance(double distance) {
    if(distance < 0.0) {
        throw std::invalid_argument("DistanceConstraint distance must be non-negative");
    }
    distance_ = distance;
}

Status DistanceConstraint::validate(const Sketch& sketch) const {
    Status status = validateRefCount(2);
    if(!status.isOk()) {
        return status;
    }
    status = validateAllRefsExist(sketch);
    if(!status.isOk()) {
        return status;
    }
    for(const GeometryRef& ref : refs()) {
        if(!isPointLikeRef(ref.kind()) && !isEntityLikeRef(ref.kind())) {
            return Status::invalidArgument("DistanceConstraint uses an unsupported geometry ref kind");
        }
    }
    return Status::ok();
}

std::unique_ptr<Constraint> DistanceConstraint::clone() const {
    return std::make_unique<DistanceConstraint>(*this);
}

} // namespace mnx
