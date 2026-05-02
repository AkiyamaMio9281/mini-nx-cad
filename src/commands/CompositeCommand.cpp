#include "mnx/commands/CompositeCommand.h"

#include <utility>

namespace mnx {

CompositeCommand::CompositeCommand(std::string label) : label_(std::move(label)) {}

void CompositeCommand::add(std::unique_ptr<Command> command) {
    commands_.push_back(std::move(command));
}

Status CompositeCommand::execute() {
    std::size_t executedCount = 0;
    for(auto& command : commands_) {
        Status status = command->execute();
        if(!status.isOk()) {
            while(executedCount > 0) {
                --executedCount;
                (void)commands_[executedCount]->undo();
            }
            return status;
        }
        ++executedCount;
    }
    return Status::ok();
}

Status CompositeCommand::undo() {
    for(auto iterator = commands_.rbegin(); iterator != commands_.rend(); ++iterator) {
        Status status = (*iterator)->undo();
        if(!status.isOk()) {
            return status;
        }
    }
    return Status::ok();
}

} // namespace mnx
