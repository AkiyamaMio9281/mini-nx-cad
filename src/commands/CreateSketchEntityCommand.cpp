#include "mnx/commands/CreateSketchEntityCommand.h"

namespace mnx {

CreateSketchEntityCommand::CreateSketchEntityCommand(Sketch& sketch, std::unique_ptr<SketchEntity> entity)
    : sketch_(sketch), entitySnapshot_(std::move(entity)) {
    if(entitySnapshot_) {
        entityId_ = entitySnapshot_->id();
    }
}

Status CreateSketchEntityCommand::execute() {
    if(!entitySnapshot_) {
        return Status::invalidArgument("CreateSketchEntityCommand has no entity snapshot");
    }
    if(executed_ && sketch_.findEntity(entityId_) != nullptr) {
        return Status::failedPrecondition("Sketch entity already exists");
    }

    Status status = sketch_.insertEntity(entitySnapshot_->clone());
    if(status.isOk()) {
        executed_ = true;
    }
    return status;
}

Status CreateSketchEntityCommand::undo() {
    Status status = sketch_.removeEntity(entityId_);
    if(status.isOk()) {
        executed_ = false;
    }
    return status;
}

} // namespace mnx
