#include "mnx/solver/VariableRegistry.h"

#include "mnx/sketch/SketchArc.h"
#include "mnx/sketch/SketchCircle.h"
#include "mnx/sketch/SketchLine.h"
#include "mnx/sketch/SketchPoint.h"

#include <algorithm>
#include <sstream>

namespace {

std::string makeLabel(mnx::EntityId id, const char* suffix) {
    std::ostringstream stream;
    stream << "entity_" << id.value() << '_' << suffix;
    return stream.str();
}

} // namespace

namespace mnx {

void VariableRegistry::extractFromSketch(const Sketch& sketch) {
    variables_.clear();

    for(const SketchEntity* entity : sketch.entities()) {
        if(const auto* point = dynamic_cast<const SketchPoint*>(entity)) {
            addVariable(entity->id(), SolverVariableKind::PointX, point->point().x(), makeLabel(entity->id(), "x"));
            addVariable(entity->id(), SolverVariableKind::PointY, point->point().y(), makeLabel(entity->id(), "y"));
        } else if(const auto* line = dynamic_cast<const SketchLine*>(entity)) {
            addVariable(entity->id(), SolverVariableKind::PointX, line->start().x(), makeLabel(entity->id(), "start_x"));
            addVariable(entity->id(), SolverVariableKind::PointY, line->start().y(), makeLabel(entity->id(), "start_y"));
            addVariable(entity->id(), SolverVariableKind::Radius, line->end().x(), makeLabel(entity->id(), "end_x"));
            addVariable(entity->id(), SolverVariableKind::StartAngle, line->end().y(), makeLabel(entity->id(), "end_y"));
        } else if(const auto* circle = dynamic_cast<const SketchCircle*>(entity)) {
            addVariable(entity->id(), SolverVariableKind::PointX, circle->center().x(), makeLabel(entity->id(), "center_x"));
            addVariable(entity->id(), SolverVariableKind::PointY, circle->center().y(), makeLabel(entity->id(), "center_y"));
            addVariable(entity->id(), SolverVariableKind::Radius, circle->radius(), makeLabel(entity->id(), "radius"));
        } else if(const auto* arc = dynamic_cast<const SketchArc*>(entity)) {
            addVariable(entity->id(), SolverVariableKind::PointX, arc->center().x(), makeLabel(entity->id(), "center_x"));
            addVariable(entity->id(), SolverVariableKind::PointY, arc->center().y(), makeLabel(entity->id(), "center_y"));
            addVariable(entity->id(), SolverVariableKind::Radius, arc->radius(), makeLabel(entity->id(), "radius"));
            addVariable(entity->id(), SolverVariableKind::StartAngle, arc->startAngle(), makeLabel(entity->id(), "start_angle"));
            addVariable(entity->id(), SolverVariableKind::EndAngle, arc->endAngle(), makeLabel(entity->id(), "end_angle"));
        }
    }
}

void VariableRegistry::writeBackToSketch(Sketch& sketch, std::span<const double> values) const {
    for(SketchEntity* entity : sketch.entities()) {
        const auto x = findVariable(entity->id(), SolverVariableKind::PointX);
        const auto y = findVariable(entity->id(), SolverVariableKind::PointY);
        const auto radius = findVariable(entity->id(), SolverVariableKind::Radius);
        const auto startAngle = findVariable(entity->id(), SolverVariableKind::StartAngle);
        const auto endAngle = findVariable(entity->id(), SolverVariableKind::EndAngle);

        if(auto* point = dynamic_cast<SketchPoint*>(entity)) {
            if(x && y) {
                point->setPoint(Point2(values[*x], values[*y]));
            }
        } else if(auto* line = dynamic_cast<SketchLine*>(entity)) {
            if(x && y && radius && startAngle) {
                line->setStart(Point2(values[*x], values[*y]));
                line->setEnd(Point2(values[*radius], values[*startAngle]));
            }
        } else if(auto* circle = dynamic_cast<SketchCircle*>(entity)) {
            if(x && y && radius) {
                circle->setCenter(Point2(values[*x], values[*y]));
                circle->setRadius(std::max(0.0, values[*radius]));
            }
        } else if(auto* arc = dynamic_cast<SketchArc*>(entity)) {
            if(x && y && radius && startAngle && endAngle) {
                arc->setCenter(Point2(values[*x], values[*y]));
                arc->setRadius(std::max(0.0, values[*radius]));
                arc->setAngles(values[*startAngle], values[*endAngle]);
            }
        }
    }
}

std::vector<double> VariableRegistry::initialValues() const {
    std::vector<double> values;
    values.reserve(variables_.size());
    for(const SolverVariable& variable : variables_) {
        values.push_back(variable.value());
    }
    return values;
}

std::optional<Point2> VariableRegistry::pointValue(const GeometryRef& ref, std::span<const double> values) const {
    const auto x = findVariable(ref.entityId(), SolverVariableKind::PointX);
    const auto y = findVariable(ref.entityId(), SolverVariableKind::PointY);
    const auto radius = findVariable(ref.entityId(), SolverVariableKind::Radius);
    const auto startAngle = findVariable(ref.entityId(), SolverVariableKind::StartAngle);

    if(!x || !y) {
        return std::nullopt;
    }

    switch(ref.kind()) {
    case GeometryRefKind::CenterPoint:
    case GeometryRefKind::WholeEntity:
        return Point2(values[*x], values[*y]);
    case GeometryRefKind::StartPoint:
        return Point2(values[*x], values[*y]);
    case GeometryRefKind::EndPoint:
        if(radius && startAngle) {
            return Point2(values[*radius], values[*startAngle]);
        }
        return std::nullopt;
    case GeometryRefKind::Midpoint:
        if(radius && startAngle) {
            return Point2((values[*x] + values[*radius]) * 0.5, (values[*y] + values[*startAngle]) * 0.5);
        }
        return Point2(values[*x], values[*y]);
    }
    return std::nullopt;
}

std::optional<double> VariableRegistry::radiusValue(const GeometryRef& ref, std::span<const double> values) const {
    const auto radius = findVariable(ref.entityId(), SolverVariableKind::Radius);
    if(!radius) {
        return std::nullopt;
    }
    return values[*radius];
}

std::optional<Vec2> VariableRegistry::lineDirection(const GeometryRef& ref, std::span<const double> values) const {
    if(ref.kind() != GeometryRefKind::WholeEntity) {
        return std::nullopt;
    }

    const auto x0 = findVariable(ref.entityId(), SolverVariableKind::PointX);
    const auto y0 = findVariable(ref.entityId(), SolverVariableKind::PointY);
    const auto x1 = findVariable(ref.entityId(), SolverVariableKind::Radius);
    const auto y1 = findVariable(ref.entityId(), SolverVariableKind::StartAngle);
    if(!x0 || !y0 || !x1 || !y1) {
        return std::nullopt;
    }
    return Vec2(values[*x1] - values[*x0], values[*y1] - values[*y0]);
}

std::optional<std::size_t> VariableRegistry::findVariable(EntityId entityId, SolverVariableKind kind) const {
    for(const SolverVariable& variable : variables_) {
        if(variable.entityId() == entityId && variable.kind() == kind) {
            return variable.index();
        }
    }
    return std::nullopt;
}

void VariableRegistry::addVariable(EntityId entityId, SolverVariableKind kind, double value, std::string label) {
    variables_.emplace_back(entityId, kind, variables_.size(), value, std::move(label));
}

} // namespace mnx
