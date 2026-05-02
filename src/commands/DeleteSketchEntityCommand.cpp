#include "mnx/commands/DeleteSketchEntityCommand.h"

namespace mnx {

DeleteSketchEntityCommand::DeleteSketchEntityCommand(Sketch& sketch, EntityId entityId)
    : sketch_(sketch), entityId_(entityId) {}

Status DeleteSketchEntityCommand::execute() {
    const SketchEntity* entity = sketch_.findEntity(entityId_);
    if(entity == nullptr) {
        return Status::notFound("Sketch entity was not found");
    }

    removedEntity_ = entity->clone();
    return sketch_.removeEntity(entityId_);
}

Status DeleteSketchEntityCommand::undo() {
    if(!removedEntity_) {
        return Status::failedPrecondition("No removed entity snapshot is available");
    }
    return sketch_.insertEntity(removedEntity_->clone());
}

} // namespace mnx
