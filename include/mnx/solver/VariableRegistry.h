#pragma once

#include "mnx/geometry/Point2.h"
#include "mnx/sketch/GeometryRef.h"
#include "mnx/sketch/Sketch.h"
#include "mnx/solver/SolverVariable.h"

#include <optional>
#include <span>
#include <vector>

namespace mnx {

class VariableRegistry {
public:
    void extractFromSketch(const Sketch& sketch);
    void writeBackToSketch(Sketch& sketch, std::span<const double> values) const;

    [[nodiscard]] std::vector<double> initialValues() const;
    [[nodiscard]] std::span<const SolverVariable> variables() const {
        return variables_;
    }

    [[nodiscard]] std::optional<Point2> pointValue(const GeometryRef& ref, std::span<const double> values) const;
    [[nodiscard]] std::optional<double> radiusValue(const GeometryRef& ref, std::span<const double> values) const;
    [[nodiscard]] std::optional<Vec2> lineDirection(const GeometryRef& ref, std::span<const double> values) const;

private:
    [[nodiscard]] std::optional<std::size_t> findVariable(EntityId entityId, SolverVariableKind kind) const;
    void addVariable(EntityId entityId, SolverVariableKind kind, double value, std::string label);

    std::vector<SolverVariable> variables_;
};

} // namespace mnx
