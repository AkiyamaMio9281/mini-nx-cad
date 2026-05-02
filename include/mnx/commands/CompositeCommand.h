#pragma once

#include "mnx/commands/Command.h"

#include <memory>
#include <string>
#include <vector>

namespace mnx {

class CompositeCommand final : public Command {
public:
    explicit CompositeCommand(std::string label);

    void add(std::unique_ptr<Command> command);

    [[nodiscard]] std::string_view label() const override {
        return label_;
    }

    [[nodiscard]] Status execute() override;
    [[nodiscard]] Status undo() override;

private:
    std::string label_;
    std::vector<std::unique_ptr<Command>> commands_;
};

} // namespace mnx
