#include "mnx/constraints/RadiusConstraint.h"

#include <stdexcept>

namespace mnx {

RadiusConstraint::RadiusConstraint(ConstraintId id, GeometryRef target, double radius, bool driving)
    : Constraint(id, ConstraintType::Radius, {target}, driving, "Radius") {
    setRadius(radius);
}

void RadiusConstraint::setRadius(double radius) {
    if(radius < 0.0) {
        throw std::invalid_argument("RadiusConstraint radius must be non-negative");
    }
    radius_ = radius;
}

Status RadiusConstraint::validate(const Sketch& sketch) const {
    Status status = validateRefCount(1);
    if(!status.isOk()) {
        return status;
    }
    status = validateAllRefsExist(sketch);
    if(!status.isOk()) {
        return status;
    }

    const GeometryRef& target = refs()[0];
    const SketchEntity* entity = sketch.findEntity(target.entityId());
    if(target.kind() != GeometryRefKind::WholeEntity || entity == nullptr) {
        return Status::invalidArgument("RadiusConstraint requires a whole circle or arc ref");
    }
    if(entity->type() != SketchEntityType::Circle && entity->type() != SketchEntityType::Arc) {
        return Status::invalidArgument("RadiusConstraint supports circles and arcs only");
    }
    return Status::ok();
}

std::unique_ptr<Constraint> RadiusConstraint::clone() const {
    return std::make_unique<RadiusConstraint>(*this);
}

} // namespace mnx
