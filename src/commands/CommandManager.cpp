#include "mnx/commands/CommandManager.h"

namespace mnx {

Status CommandManager::execute(std::unique_ptr<Command> command) {
    if(!command) {
        return Status::invalidArgument("Cannot execute a null command");
    }

    Status status = command->execute();
    if(!status.isOk()) {
        return status;
    }

    undoStack_.push_back(std::move(command));
    redoStack_.clear();
    return Status::ok();
}

Status CommandManager::undo() {
    if(undoStack_.empty()) {
        return Status::failedPrecondition("No command is available to undo");
    }

    std::unique_ptr<Command> command = std::move(undoStack_.back());
    undoStack_.pop_back();
    Status status = command->undo();
    if(!status.isOk()) {
        undoStack_.push_back(std::move(command));
        return status;
    }

    redoStack_.push_back(std::move(command));
    return Status::ok();
}

Status CommandManager::redo() {
    if(redoStack_.empty()) {
        return Status::failedPrecondition("No command is available to redo");
    }

    std::unique_ptr<Command> command = std::move(redoStack_.back());
    redoStack_.pop_back();
    Status status = command->execute();
    if(!status.isOk()) {
        redoStack_.push_back(std::move(command));
        return status;
    }

    undoStack_.push_back(std::move(command));
    return Status::ok();
}

void CommandManager::clear() {
    undoStack_.clear();
    redoStack_.clear();
}

} // namespace mnx
