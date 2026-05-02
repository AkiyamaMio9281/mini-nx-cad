#pragma once

#include "mnx/commands/Command.h"
#include "mnx/sketch/Sketch.h"

namespace mnx {

class MoveSketchHandleCommand final : public Command {
public:
    MoveSketchHandleCommand(Sketch& sketch, GeometryRef ref, Point2 before, Point2 after);

    [[nodiscard]] std::string_view label() const override {
        return "Move Sketch Handle";
    }

    [[nodiscard]] Status execute() override;
    [[nodiscard]] Status undo() override;

private:
    [[nodiscard]] Status apply(const Point2& point);

    Sketch& sketch_;
    GeometryRef ref_;
    Point2 before_;
    Point2 after_;
};

} // namespace mnx
