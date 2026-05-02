#include "mnx/assembly/MateConstraint.h"

#include <utility>

namespace mnx {

MateConstraint MateConstraint::fixed(
    ConstraintId id,
    ComponentId componentId,
    Transform3D targetTransform,
    std::string label
) {
    MateConstraint mate;
    mate.id_ = id;
    mate.type_ = MateType::Fixed;
    mate.firstComponentId_ = componentId;
    mate.fixedTransform_ = targetTransform;
    mate.label_ = std::move(label);
    return mate;
}

MateConstraint MateConstraint::coincident(
    ConstraintId id,
    ComponentId firstComponentId,
    Vec3 firstLocalPoint,
    ComponentId secondComponentId,
    Vec3 secondLocalPoint,
    std::string label
) {
    MateConstraint mate;
    mate.id_ = id;
    mate.type_ = MateType::Coincident;
    mate.firstComponentId_ = firstComponentId;
    mate.secondComponentId_ = secondComponentId;
    mate.firstLocalPoint_ = firstLocalPoint;
    mate.secondLocalPoint_ = secondLocalPoint;
    mate.label_ = std::move(label);
    return mate;
}

MateConstraint MateConstraint::distance(
    ConstraintId id,
    ComponentId firstComponentId,
    Vec3 firstLocalPoint,
    ComponentId secondComponentId,
    Vec3 secondLocalPoint,
    double distance,
    std::string label
) {
    MateConstraint mate = coincident(id, firstComponentId, firstLocalPoint, secondComponentId, secondLocalPoint, std::move(label));
    mate.type_ = MateType::Distance;
    mate.distance_ = distance;
    return mate;
}

MateConstraint MateConstraint::parallel(
    ConstraintId id,
    ComponentId firstComponentId,
    Vec3 firstLocalDirection,
    ComponentId secondComponentId,
    Vec3 secondLocalDirection,
    std::string label
) {
    MateConstraint mate;
    mate.id_ = id;
    mate.type_ = MateType::Parallel;
    mate.firstComponentId_ = firstComponentId;
    mate.secondComponentId_ = secondComponentId;
    mate.firstLocalDirection_ = firstLocalDirection;
    mate.secondLocalDirection_ = secondLocalDirection;
    mate.label_ = std::move(label);
    return mate;
}

ConstraintId MateConstraint::id() const {
    return id_;
}

MateType MateConstraint::type() const {
    return type_;
}

ComponentId MateConstraint::firstComponentId() const {
    return firstComponentId_;
}

ComponentId MateConstraint::secondComponentId() const {
    return secondComponentId_;
}

const Vec3& MateConstraint::firstLocalPoint() const {
    return firstLocalPoint_;
}

const Vec3& MateConstraint::secondLocalPoint() const {
    return secondLocalPoint_;
}

const Vec3& MateConstraint::firstLocalDirection() const {
    return firstLocalDirection_;
}

const Vec3& MateConstraint::secondLocalDirection() const {
    return secondLocalDirection_;
}

double MateConstraint::distance() const {
    return distance_;
}

const Transform3D& MateConstraint::fixedTransform() const {
    return fixedTransform_;
}

const std::string& MateConstraint::label() const {
    return label_;
}

} // namespace mnx
