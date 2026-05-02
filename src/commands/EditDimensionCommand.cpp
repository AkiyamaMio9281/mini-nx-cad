#include "mnx/commands/EditDimensionCommand.h"

namespace mnx {

EditDimensionCommand::EditDimensionCommand(Dimension& dimension, Dimension before, Dimension after)
    : dimension_(dimension), before_(std::move(before)), after_(std::move(after)) {}

Status EditDimensionCommand::execute() {
    dimension_ = after_;
    return Status::ok();
}

Status EditDimensionCommand::undo() {
    dimension_ = before_;
    return Status::ok();
}

} // namespace mnx
