#pragma once

#include "mnx/commands/Command.h"
#include "mnx/sketch/Sketch.h"

#include <memory>

namespace mnx {

class DeleteSketchEntityCommand final : public Command {
public:
    DeleteSketchEntityCommand(Sketch& sketch, EntityId entityId);

    [[nodiscard]] std::string_view label() const override {
        return "Delete Sketch Entity";
    }

    [[nodiscard]] Status execute() override;
    [[nodiscard]] Status undo() override;

private:
    Sketch& sketch_;
    EntityId entityId_;
    std::unique_ptr<SketchEntity> removedEntity_;
};

} // namespace mnx
