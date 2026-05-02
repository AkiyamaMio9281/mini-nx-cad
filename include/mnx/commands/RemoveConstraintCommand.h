#pragma once

#include "mnx/commands/Command.h"
#include "mnx/constraints/Constraint.h"
#include "mnx/sketch/Sketch.h"

#include <memory>
#include <vector>

namespace mnx {

class RemoveConstraintCommand final : public Command {
public:
    RemoveConstraintCommand(
        Sketch& sketch,
        std::vector<std::unique_ptr<Constraint>>& constraints,
        ConstraintId constraintId
    );

    [[nodiscard]] std::string_view label() const override {
        return "Remove Constraint";
    }

    [[nodiscard]] Status execute() override;
    [[nodiscard]] Status undo() override;

private:
    Sketch& sketch_;
    std::vector<std::unique_ptr<Constraint>>& constraints_;
    ConstraintId constraintId_;
    std::unique_ptr<Constraint> removedConstraint_;
};

} // namespace mnx
