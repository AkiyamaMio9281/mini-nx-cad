#pragma once

#include "mnx/commands/Command.h"

#include <memory>
#include <span>
#include <vector>

namespace mnx {

class CommandManager {
public:
    [[nodiscard]] Status execute(std::unique_ptr<Command> command);
    [[nodiscard]] Status undo();
    [[nodiscard]] Status redo();

    [[nodiscard]] bool canUndo() const {
        return !undoStack_.empty();
    }

    [[nodiscard]] bool canRedo() const {
        return !redoStack_.empty();
    }

    [[nodiscard]] std::size_t undoCount() const {
        return undoStack_.size();
    }

    [[nodiscard]] std::size_t redoCount() const {
        return redoStack_.size();
    }

    [[nodiscard]] std::span<const std::unique_ptr<Command>> undoStack() const {
        return undoStack_;
    }

    [[nodiscard]] std::span<const std::unique_ptr<Command>> redoStack() const {
        return redoStack_;
    }

    void clear();

private:
    std::vector<std::unique_ptr<Command>> undoStack_;
    std::vector<std::unique_ptr<Command>> redoStack_;
};

} // namespace mnx
