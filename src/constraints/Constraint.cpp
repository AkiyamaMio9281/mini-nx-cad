#include "mnx/constraints/Constraint.h"

#include <sstream>
#include <utility>

namespace mnx {

Constraint::Constraint(
    ConstraintId id,
    ConstraintType type,
    std::vector<GeometryRef> refs,
    bool driving,
    std::string label
) : id_(id), type_(type), refs_(std::move(refs)), driving_(driving), label_(std::move(label)) {}

void Constraint::setLabel(std::string label) {
    label_ = std::move(label);
}

std::string Constraint::debugDescription() const {
    std::ostringstream stream;
    stream << "constraint " << id_.value() << " refs=" << refs_.size();
    return stream.str();
}

Status Constraint::validate(const Sketch& sketch) const {
    return validateAllRefsExist(sketch);
}

Status Constraint::validateRefCount(std::size_t expected) const {
    if(refs_.size() != expected) {
        return Status::invalidArgument("Constraint has an unexpected number of geometry refs");
    }
    return Status::ok();
}

Status Constraint::validateRefExists(const Sketch& sketch, const GeometryRef& ref) const {
    if(!ref.isValid()) {
        return Status::invalidArgument("Constraint uses an invalid geometry ref");
    }
    if(sketch.findEntity(ref.entityId()) == nullptr) {
        return Status::notFound("Constraint references a missing sketch entity");
    }
    if(!sketch.supportsRef(ref)) {
        return Status::invalidArgument("Constraint references an unsupported sub-entity");
    }
    return Status::ok();
}

Status Constraint::validateAllRefsExist(const Sketch& sketch) const {
    for(const GeometryRef& ref : refs_) {
        Status status = validateRefExists(sketch, ref);
        if(!status.isOk()) {
            return status;
        }
    }
    return Status::ok();
}

bool isPointLikeRef(GeometryRefKind kind) {
    return kind == GeometryRefKind::StartPoint || kind == GeometryRefKind::EndPoint
        || kind == GeometryRefKind::CenterPoint || kind == GeometryRefKind::Midpoint;
}

bool isEntityLikeRef(GeometryRefKind kind) {
    return kind == GeometryRefKind::WholeEntity;
}

} // namespace mnx
