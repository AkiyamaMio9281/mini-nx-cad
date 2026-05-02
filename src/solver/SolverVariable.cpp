#include "mnx/solver/SolverVariable.h"

#include <utility>

namespace mnx {

SolverVariable::SolverVariable(
    EntityId entityId,
    SolverVariableKind kind,
    std::size_t index,
    double value,
    std::string label
) : entityId_(entityId), kind_(kind), index_(index), value_(value), label_(std::move(label)) {}

} // namespace mnx
