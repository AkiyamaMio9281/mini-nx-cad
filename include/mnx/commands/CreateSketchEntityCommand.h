#pragma once

#include "mnx/commands/Command.h"
#include "mnx/sketch/Sketch.h"

#include <memory>

namespace mnx {

class CreateSketchEntityCommand final : public Command {
public:
    CreateSketchEntityCommand(Sketch& sketch, std::unique_ptr<SketchEntity> entity);

    [[nodiscard]] std::string_view label() const override {
        return "Create Sketch Entity";
    }

    [[nodiscard]] Status execute() override;
    [[nodiscard]] Status undo() override;

private:
    Sketch& sketch_;
    std::unique_ptr<SketchEntity> entitySnapshot_;
    EntityId entityId_;
    bool executed_ = false;
};

} // namespace mnx
