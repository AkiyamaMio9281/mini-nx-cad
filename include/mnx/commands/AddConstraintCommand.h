#pragma once

#include "mnx/commands/Command.h"
#include "mnx/constraints/Constraint.h"
#include "mnx/sketch/Sketch.h"

#include <memory>
#include <vector>

namespace mnx {

class AddConstraintCommand final : public Command {
public:
    AddConstraintCommand(
        Sketch& sketch,
        std::vector<std::unique_ptr<Constraint>>& constraints,
        std::unique_ptr<Constraint> constraint
    );

    [[nodiscard]] std::string_view label() const override {
        return "Add Constraint";
    }

    [[nodiscard]] Status execute() override;
    [[nodiscard]] Status undo() override;

private:
    Sketch& sketch_;
    std::vector<std::unique_ptr<Constraint>>& constraints_;
    std::unique_ptr<Constraint> constraintSnapshot_;
    ConstraintId constraintId_;
};

} // namespace mnx
