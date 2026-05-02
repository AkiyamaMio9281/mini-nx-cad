#include "mnx/commands/RemoveConstraintCommand.h"

#include <algorithm>

namespace mnx {

RemoveConstraintCommand::RemoveConstraintCommand(
    Sketch& sketch,
    std::vector<std::unique_ptr<Constraint>>& constraints,
    ConstraintId constraintId
) : sketch_(sketch), constraints_(constraints), constraintId_(constraintId) {}

Status RemoveConstraintCommand::execute() {
    const auto iterator = std::find_if(constraints_.begin(), constraints_.end(), [this](const auto& constraint) {
        return constraint->id() == constraintId_;
    });
    if(iterator == constraints_.end()) {
        return Status::notFound("Constraint was not found");
    }

    removedConstraint_ = (*iterator)->clone();
    constraints_.erase(iterator);
    (void)sketch_.removeConstraintId(constraintId_);
    return Status::ok();
}

Status RemoveConstraintCommand::undo() {
    if(!removedConstraint_) {
        return Status::failedPrecondition("No removed constraint snapshot is available");
    }
    constraints_.push_back(removedConstraint_->clone());
    sketch_.addConstraintId(constraintId_);
    return Status::ok();
}

} // namespace mnx
