#pragma once

#include "mnx/core/ids/EntityId.h"

#include <cstddef>
#include <string>

namespace mnx {

enum class SolverVariableKind {
    PointX,
    PointY,
    Radius,
    StartAngle,
    EndAngle,
};

class SolverVariable {
public:
    SolverVariable() = default;
    SolverVariable(EntityId entityId, SolverVariableKind kind, std::size_t index, double value, std::string label);

    [[nodiscard]] EntityId entityId() const {
        return entityId_;
    }

    [[nodiscard]] SolverVariableKind kind() const {
        return kind_;
    }

    [[nodiscard]] std::size_t index() const {
        return index_;
    }

    [[nodiscard]] double value() const {
        return value_;
    }

    void setValue(double value) {
        value_ = value;
    }

    [[nodiscard]] const std::string& label() const {
        return label_;
    }

private:
    EntityId entityId_;
    SolverVariableKind kind_ = SolverVariableKind::PointX;
    std::size_t index_ = 0;
    double value_ = 0.0;
    std::string label_;
};

} // namespace mnx
