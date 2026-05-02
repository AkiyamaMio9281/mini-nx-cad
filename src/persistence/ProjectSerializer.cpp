#include "mnx/persistence/ProjectSerializer.h"

#include "mnx/constraints/AngleConstraint.h"
#include "mnx/constraints/DistanceConstraint.h"
#include "mnx/constraints/RadiusConstraint.h"
#include "mnx/modeling/ExtrudeFeature.h"
#include "mnx/modeling/RevolveFeature.h"
#include "mnx/modeling/SketchFeature.h"
#include "mnx/persistence/JsonConverters.h"
#include "mnx/persistence/SchemaVersion.h"
#include "mnx/sketch/SketchArc.h"
#include "mnx/sketch/SketchCircle.h"
#include "mnx/sketch/SketchLine.h"
#include "mnx/sketch/SketchPoint.h"
#include "mnx/sketch/SketchSpline.h"

namespace mnx::persistence {

namespace {

using Object = JsonValue::Object;
using Array = JsonValue::Array;

[[nodiscard]] JsonValue point2Json(const Point2& point) {
    return Object {{"x", point.x()}, {"y", point.y()}};
}

[[nodiscard]] JsonValue refJson(const GeometryRef& ref) {
    return Object {{"entityId", static_cast<double>(ref.entityId().value())}, {"kind", static_cast<double>(static_cast<int>(ref.kind()))}};
}

[[nodiscard]] JsonValue vec3Json(const Vec3& vector) {
    return Object {{"x", vector.x()}, {"y", vector.y()}, {"z", vector.z()}};
}

[[nodiscard]] JsonValue transformJson(const Transform3D& transform) {
    Array values;
    for(double value : transform.matrix().values()) {
        values.push_back(value);
    }
    return Object {{"matrix", values}};
}

[[nodiscard]] JsonValue sketchJson(const Sketch& sketch) {
    Array entities;
    for(const SketchEntity* entity : sketch.entities()) {
        Object object {{"id", static_cast<double>(entity->id().value())}, {"type", static_cast<double>(static_cast<int>(entity->type()))}};
        switch(entity->type()) {
        case SketchEntityType::Point: {
            const auto* point = static_cast<const SketchPoint*>(entity);
            object["point"] = point2Json(point->point());
            break;
        }
        case SketchEntityType::Line: {
            const auto* line = static_cast<const SketchLine*>(entity);
            object["start"] = point2Json(line->start());
            object["end"] = point2Json(line->end());
            break;
        }
        case SketchEntityType::Circle: {
            const auto* circle = static_cast<const SketchCircle*>(entity);
            object["center"] = point2Json(circle->center());
            object["radius"] = circle->radius();
            break;
        }
        case SketchEntityType::Arc: {
            const auto* arc = static_cast<const SketchArc*>(entity);
            object["center"] = point2Json(arc->center());
            object["radius"] = arc->radius();
            object["startAngle"] = arc->startAngle();
            object["endAngle"] = arc->endAngle();
            break;
        }
        case SketchEntityType::Spline: {
            const auto* spline = static_cast<const SketchSpline*>(entity);
            Array points;
            for(const Point2& point : spline->controlPoints()) {
                points.push_back(point2Json(point));
            }
            object["degree"] = spline->degree();
            object["controlPoints"] = points;
            break;
        }
        }
        entities.push_back(object);
    }

    Array constraintIds;
    for(ConstraintId id : sketch.constraintIds()) {
        constraintIds.push_back(static_cast<double>(id.value()));
    }
    return Object {{"entities", entities}, {"constraintIds", constraintIds}};
}

[[nodiscard]] JsonValue featureJson(const Feature& feature) {
    Object object {
        {"id", static_cast<double>(feature.id().value())},
        {"name", feature.name()},
        {"type", static_cast<double>(static_cast<int>(feature.type()))},
    };
    Array dependencies;
    for(FeatureId id : feature.dependencies()) {
        dependencies.push_back(static_cast<double>(id.value()));
    }
    object["dependencies"] = dependencies;

    if(const auto* sketch = dynamic_cast<const SketchFeature*>(&feature)) {
        object["sketch"] = sketchJson(sketch->sketch());
    } else if(const auto* extrude = dynamic_cast<const ExtrudeFeature*>(&feature)) {
        object["profileSketchFeatureId"] = static_cast<double>(extrude->profileSketchFeatureId().value());
        object["distance"] = extrude->distance();
    } else if(const auto* revolve = dynamic_cast<const RevolveFeature*>(&feature)) {
        object["profileSketchFeatureId"] = static_cast<double>(revolve->profileSketchFeatureId().value());
        object["angleRadians"] = revolve->angleRadians();
        object["axis"] = Object {
            {"origin", point2Json(revolve->axis().origin())},
            {"direction", Object {{"x", revolve->axis().direction().x()}, {"y", revolve->axis().direction().y()}}},
        };
    }
    return object;
}

[[nodiscard]] JsonValue constraintJson(const Constraint& constraint) {
    Object object {
        {"id", static_cast<double>(constraint.id().value())},
        {"type", static_cast<double>(static_cast<int>(constraint.type()))},
        {"driving", constraint.isDriving()},
        {"label", constraint.label()},
    };
    Array refs;
    for(const GeometryRef& ref : constraint.refs()) {
        refs.push_back(refJson(ref));
    }
    object["refs"] = refs;
    if(const auto* distance = dynamic_cast<const DistanceConstraint*>(&constraint)) {
        object["value"] = distance->distance();
    } else if(const auto* radius = dynamic_cast<const RadiusConstraint*>(&constraint)) {
        object["value"] = radius->radius();
    } else if(const auto* angle = dynamic_cast<const AngleConstraint*>(&constraint)) {
        object["value"] = angle->angleRadians();
    }
    return object;
}

} // namespace

std::string ProjectSerializer::serialize(const PartDocument& document) const {
    Array features;
    for(const Feature* feature : document.featureGraph().features()) {
        if(feature != nullptr) {
            features.push_back(featureJson(*feature));
        }
    }

    Array constraints;
    for(const auto& constraint : document.constraints()) {
        if(constraint) {
            constraints.push_back(constraintJson(*constraint));
        }
    }

    const JsonValue root(Object {
        {"schemaVersion", kCurrentSchemaVersion},
        {"document", Object {{"name", document.name()}, {"features", features}, {"constraints", constraints}}},
        {"assemblies", Array {}},
    });
    return root.dump() + "\n";
}

std::string ProjectSerializer::serializeAssembly(const AssemblyDocument& assembly) const {
    Array components;
    for(const ComponentInstance& component : assembly.components()) {
        Object object {
            {"id", static_cast<double>(component.id().value())},
            {"name", component.name()},
            {"sourceDocumentName", component.sourceDocumentName()},
            {"transform", transformJson(component.transformNode().localTransform())},
        };
        if(component.transformNode().parentId().has_value()) {
            object["parentId"] = static_cast<double>(component.transformNode().parentId()->value());
        }
        components.push_back(object);
    }

    Array mates;
    for(const MateConstraint& mate : assembly.mates()) {
        Object object {
            {"id", static_cast<double>(mate.id().value())},
            {"type", static_cast<double>(static_cast<int>(mate.type()))},
            {"firstComponentId", static_cast<double>(mate.firstComponentId().value())},
            {"secondComponentId", static_cast<double>(mate.secondComponentId().value())},
            {"firstLocalPoint", vec3Json(mate.firstLocalPoint())},
            {"secondLocalPoint", vec3Json(mate.secondLocalPoint())},
            {"firstLocalDirection", vec3Json(mate.firstLocalDirection())},
            {"secondLocalDirection", vec3Json(mate.secondLocalDirection())},
            {"distance", mate.distance()},
            {"fixedTransform", transformJson(mate.fixedTransform())},
            {"label", mate.label()},
        };
        mates.push_back(object);
    }

    const JsonValue root(Object {
        {"schemaVersion", kCurrentSchemaVersion},
        {"assembly", Object {{"name", assembly.name()}, {"components", components}, {"mates", mates}}},
    });
    return root.dump() + "\n";
}

} // namespace mnx::persistence
