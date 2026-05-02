#pragma once

#include "mnx/commands/Command.h"
#include "mnx/constraints/Dimension.h"

namespace mnx {

class EditDimensionCommand final : public Command {
public:
    EditDimensionCommand(Dimension& dimension, Dimension before, Dimension after);

    [[nodiscard]] std::string_view label() const override {
        return "Edit Dimension";
    }

    [[nodiscard]] Status execute() override;
    [[nodiscard]] Status undo() override;

private:
    Dimension& dimension_;
    Dimension before_;
    Dimension after_;
};

} // namespace mnx
