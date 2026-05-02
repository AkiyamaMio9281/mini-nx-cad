#include "mnx/solver/EquationBuilder.h"

#include "mnx/constraints/AngleConstraint.h"
#include "mnx/constraints/CoincidentConstraint.h"
#include "mnx/constraints/DistanceConstraint.h"
#include "mnx/constraints/EqualLengthConstraint.h"
#include "mnx/constraints/ParallelConstraint.h"
#include "mnx/constraints/PerpendicularConstraint.h"
#include "mnx/constraints/RadiusConstraint.h"
#include "mnx/constraints/TangentConstraint.h"

#include <algorithm>
#include <cmath>

namespace {

constexpr double kEquationEpsilon = 1.0e-12;

double safeNorm(const mnx::Vec2& vector) {
    return std::max(vector.norm(), kEquationEpsilon);
}

double normalizedCross(const mnx::Vec2& lhs, const mnx::Vec2& rhs) {
    return lhs.cross(rhs) / (safeNorm(lhs) * safeNorm(rhs));
}

double normalizedDot(const mnx::Vec2& lhs, const mnx::Vec2& rhs) {
    return lhs.dot(rhs) / (safeNorm(lhs) * safeNorm(rhs));
}

} // namespace

namespace mnx {

std::vector<ConstraintEquation> EquationBuilder::build(
    const Sketch& sketch,
    const VariableRegistry& registry,
    std::span<const Constraint*> constraints,
    SolverDiagnostics& diagnostics
) const {
    std::vector<ConstraintEquation> equations;

    for(const Constraint* constraint : constraints) {
        if(constraint == nullptr) {
            diagnostics.addWarning(SolverWarning::InvalidConstraint, "Null constraint pointer");
            continue;
        }

        const Status validation = constraint->validate(sketch);
        if(!validation.isOk()) {
            diagnostics.addWarning(SolverWarning::InvalidConstraint, validation.message());
            continue;
        }

        switch(constraint->type()) {
        case ConstraintType::Coincident: {
            const auto* coincident = dynamic_cast<const CoincidentConstraint*>(constraint);
            const GeometryRef first = coincident->first();
            const GeometryRef second = coincident->second();
            equations.emplace_back("coincident_x", [first, second, &registry](std::span<const double> values) {
                const auto lhs = registry.pointValue(first, values);
                const auto rhs = registry.pointValue(second, values);
                return lhs && rhs ? lhs->x() - rhs->x() : 0.0;
            });
            equations.emplace_back("coincident_y", [first, second, &registry](std::span<const double> values) {
                const auto lhs = registry.pointValue(first, values);
                const auto rhs = registry.pointValue(second, values);
                return lhs && rhs ? lhs->y() - rhs->y() : 0.0;
            });
            break;
        }
        case ConstraintType::Horizontal: {
            const GeometryRef ref = constraint->refs()[0];
            equations.emplace_back("horizontal", [ref, &registry](std::span<const double> values) {
                const auto direction = registry.lineDirection(ref, values);
                return direction ? direction->y() : 0.0;
            });
            break;
        }
        case ConstraintType::Vertical: {
            const GeometryRef ref = constraint->refs()[0];
            equations.emplace_back("vertical", [ref, &registry](std::span<const double> values) {
                const auto direction = registry.lineDirection(ref, values);
                return direction ? direction->x() : 0.0;
            });
            break;
        }
        case ConstraintType::Distance: {
            const auto* distance = dynamic_cast<const DistanceConstraint*>(constraint);
            const GeometryRef first = distance->refs()[0];
            const GeometryRef second = distance->refs()[1];
            const double target = distance->distance();
            equations.emplace_back("distance", [first, second, target, &registry](std::span<const double> values) {
                const auto lhs = registry.pointValue(first, values);
                const auto rhs = registry.pointValue(second, values);
                return lhs && rhs ? (*lhs - *rhs).norm() - target : 0.0;
            });
            break;
        }
        case ConstraintType::Radius: {
            const auto* radius = dynamic_cast<const RadiusConstraint*>(constraint);
            const GeometryRef target = radius->refs()[0];
            const double targetRadius = radius->radius();
            equations.emplace_back("radius", [target, targetRadius, &registry](std::span<const double> values) {
                const auto value = registry.radiusValue(target, values);
                return value ? *value - targetRadius : 0.0;
            });
            break;
        }
        case ConstraintType::Parallel: {
            const auto* parallel = dynamic_cast<const ParallelConstraint*>(constraint);
            const GeometryRef first = parallel->refs()[0];
            const GeometryRef second = parallel->refs()[1];
            equations.emplace_back("parallel", [first, second, &registry](std::span<const double> values) {
                const auto lhs = registry.lineDirection(first, values);
                const auto rhs = registry.lineDirection(second, values);
                return lhs && rhs ? normalizedCross(*lhs, *rhs) : 0.0;
            });
            break;
        }
        case ConstraintType::Perpendicular: {
            const auto* perpendicular = dynamic_cast<const PerpendicularConstraint*>(constraint);
            const GeometryRef first = perpendicular->refs()[0];
            const GeometryRef second = perpendicular->refs()[1];
            equations.emplace_back("perpendicular", [first, second, &registry](std::span<const double> values) {
                const auto lhs = registry.lineDirection(first, values);
                const auto rhs = registry.lineDirection(second, values);
                return lhs && rhs ? normalizedDot(*lhs, *rhs) : 0.0;
            });
            break;
        }
        case ConstraintType::Angle: {
            const auto* angle = dynamic_cast<const AngleConstraint*>(constraint);
            const GeometryRef first = angle->refs()[0];
            const GeometryRef second = angle->refs()[1];
            const double target = angle->angleRadians();
            equations.emplace_back("angle", [first, second, target, &registry](std::span<const double> values) {
                const auto lhs = registry.lineDirection(first, values);
                const auto rhs = registry.lineDirection(second, values);
                if(!lhs || !rhs) {
                    return 0.0;
                }
                const double actual = std::atan2(lhs->cross(*rhs), lhs->dot(*rhs));
                return actual - target;
            });
            break;
        }
        case ConstraintType::Tangent: {
            const auto* tangent = dynamic_cast<const TangentConstraint*>(constraint);
            const GeometryRef first = tangent->refs()[0];
            const GeometryRef second = tangent->refs()[1];
            equations.emplace_back("tangent", [first, second, &registry](std::span<const double> values) {
                const auto lineDirection = registry.lineDirection(first, values);
                const auto linePoint = registry.pointValue(GeometryRef::startPoint(first.entityId()), values);
                const auto center = registry.pointValue(GeometryRef::centerPoint(second.entityId()), values);
                const auto radius = registry.radiusValue(second, values);
                if(lineDirection && linePoint && center && radius) {
                    return ((*center - *linePoint).cross(*lineDirection) / safeNorm(*lineDirection)) - *radius;
                }

                const auto reversedDirection = registry.lineDirection(second, values);
                const auto reversedPoint = registry.pointValue(GeometryRef::startPoint(second.entityId()), values);
                const auto reversedCenter = registry.pointValue(GeometryRef::centerPoint(first.entityId()), values);
                const auto reversedRadius = registry.radiusValue(first, values);
                if(reversedDirection && reversedPoint && reversedCenter && reversedRadius) {
                    return ((*reversedCenter - *reversedPoint).cross(*reversedDirection) / safeNorm(*reversedDirection))
                        - *reversedRadius;
                }
                return 0.0;
            });
            break;
        }
        case ConstraintType::EqualLength: {
            const auto* equalLength = dynamic_cast<const EqualLengthConstraint*>(constraint);
            const GeometryRef first = equalLength->refs()[0];
            const GeometryRef second = equalLength->refs()[1];
            equations.emplace_back("equal_length", [first, second, &registry](std::span<const double> values) {
                const auto lhs = registry.lineDirection(first, values);
                const auto rhs = registry.lineDirection(second, values);
                return lhs && rhs ? lhs->norm() - rhs->norm() : 0.0;
            });
            break;
        }
        }
    }

    return equations;
}

} // namespace mnx
