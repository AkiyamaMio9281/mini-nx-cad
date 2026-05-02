#include "mnx/commands/AddConstraintCommand.h"

#include <algorithm>

namespace mnx {

AddConstraintCommand::AddConstraintCommand(
    Sketch& sketch,
    std::vector<std::unique_ptr<Constraint>>& constraints,
    std::unique_ptr<Constraint> constraint
) : sketch_(sketch), constraints_(constraints), constraintSnapshot_(std::move(constraint)) {
    if(constraintSnapshot_) {
        constraintId_ = constraintSnapshot_->id();
    }
}

Status AddConstraintCommand::execute() {
    if(!constraintSnapshot_) {
        return Status::invalidArgument("AddConstraintCommand has no constraint snapshot");
    }
    const auto existing = std::find_if(constraints_.begin(), constraints_.end(), [this](const auto& constraint) {
        return constraint->id() == constraintId_;
    });
    if(existing != constraints_.end()) {
        return Status::failedPrecondition("Constraint already exists");
    }

    constraints_.push_back(constraintSnapshot_->clone());
    sketch_.addConstraintId(constraintId_);
    return Status::ok();
}

Status AddConstraintCommand::undo() {
    const auto oldSize = constraints_.size();
    constraints_.erase(
        std::remove_if(constraints_.begin(), constraints_.end(), [this](const auto& constraint) {
            return constraint->id() == constraintId_;
        }),
        constraints_.end()
    );
    (void)sketch_.removeConstraintId(constraintId_);
    return oldSize == constraints_.size() ? Status::notFound("Constraint was not found") : Status::ok();
}

} // namespace mnx
