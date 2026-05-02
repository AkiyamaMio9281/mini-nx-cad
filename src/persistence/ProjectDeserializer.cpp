#include "mnx/persistence/ProjectDeserializer.h"

#include "mnx/constraints/AngleConstraint.h"
#include "mnx/constraints/CoincidentConstraint.h"
#include "mnx/constraints/DistanceConstraint.h"
#include "mnx/constraints/EqualLengthConstraint.h"
#include "mnx/constraints/HorizontalConstraint.h"
#include "mnx/constraints/ParallelConstraint.h"
#include "mnx/constraints/PerpendicularConstraint.h"
#include "mnx/constraints/RadiusConstraint.h"
#include "mnx/constraints/TangentConstraint.h"
#include "mnx/constraints/VerticalConstraint.h"
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

#include <memory>
#include <string>
#include <vector>

namespace mnx::persistence {

namespace {

[[nodiscard]] Result<const JsonValue*> requireField(const JsonValue& object, const std::string& key) {
    const JsonValue* value = object.find(key);
    if(value == nullptr) {
        return Result<const JsonValue*>::failure(Status::invalidArgument("Missing field: " + key));
    }
    return value;
}

[[nodiscard]] Result<const JsonValue*> requireTypedField(
    const JsonValue& object,
    const std::string& key,
    bool (JsonValue::*predicate)() const,
    const std::string& expected
) {
    Result<const JsonValue*> field = requireField(object, key);
    if(!field) {
        return field;
    }
    if(!(field.value()->*predicate)()) {
        return Result<const JsonValue*>::failure(Status::invalidArgument("Field '" + key + "' must be " + expected));
    }
    return field;
}

[[nodiscard]] Result<const JsonValue*> requireObjectField(const JsonValue& object, const std::string& key) {
    return requireTypedField(object, key, &JsonValue::isObject, "an object");
}

[[nodiscard]] Result<const JsonValue*> requireArrayField(const JsonValue& object, const std::string& key) {
    return requireTypedField(object, key, &JsonValue::isArray, "an array");
}

[[nodiscard]] Result<double> requireNumberField(const JsonValue& object, const std::string& key) {
    Result<const JsonValue*> field = requireTypedField(object, key, &JsonValue::isNumber, "a number");
    if(!field) {
        return Result<double>::failure(field.status());
    }
    return field.value()->asNumber();
}

[[nodiscard]] Result<std::string> requireStringField(const JsonValue& object, const std::string& key) {
    Result<const JsonValue*> field = requireTypedField(object, key, &JsonValue::isString, "a string");
    if(!field) {
        return Result<std::string>::failure(field.status());
    }
    return field.value()->asString();
}

[[nodiscard]] Result<Point2> point2FromJson(const JsonValue& value) {
    if(!value.isObject()) {
        return Result<Point2>::failure(Status::invalidArgument("Point2 value must be an object"));
    }
    Result<double> x = requireNumberField(value, "x");
    if(!x) {
        return Result<Point2>::failure(x.status());
    }
    Result<double> y = requireNumberField(value, "y");
    if(!y) {
        return Result<Point2>::failure(y.status());
    }
    return Point2(x.value(), y.value());
}

[[nodiscard]] Result<Vec2> vec2FromJson(const JsonValue& value) {
    if(!value.isObject()) {
        return Result<Vec2>::failure(Status::invalidArgument("Vec2 value must be an object"));
    }
    Result<double> x = requireNumberField(value, "x");
    if(!x) {
        return Result<Vec2>::failure(x.status());
    }
    Result<double> y = requireNumberField(value, "y");
    if(!y) {
        return Result<Vec2>::failure(y.status());
    }
    return Vec2(x.value(), y.value());
}

[[nodiscard]] Result<Vec3> vec3FromJson(const JsonValue& value) {
    if(!value.isObject()) {
        return Result<Vec3>::failure(Status::invalidArgument("Vec3 value must be an object"));
    }
    Result<double> x = requireNumberField(value, "x");
    Result<double> y = requireNumberField(value, "y");
    Result<double> z = requireNumberField(value, "z");
    if(!x) {
        return Result<Vec3>::failure(x.status());
    }
    if(!y) {
        return Result<Vec3>::failure(y.status());
    }
    if(!z) {
        return Result<Vec3>::failure(z.status());
    }
    return Vec3(x.value(), y.value(), z.value());
}

[[nodiscard]] Result<Transform3D> transformFromJson(const JsonValue& value) {
    if(!value.isObject()) {
        return Result<Transform3D>::failure(Status::invalidArgument("Transform value must be an object"));
    }
    Result<const JsonValue*> matrixValue = requireArrayField(value, "matrix");
    if(!matrixValue) {
        return Result<Transform3D>::failure(matrixValue.status());
    }
    if(matrixValue.value()->asArray().size() != 16U) {
        return Result<Transform3D>::failure(Status::invalidArgument("Transform matrix must contain 16 values"));
    }
    Mat4::Storage storage {};
    for(std::size_t index = 0; index < storage.size(); ++index) {
        const JsonValue& entry = matrixValue.value()->asArray()[index];
        if(!entry.isNumber()) {
            return Result<Transform3D>::failure(Status::invalidArgument("Transform matrix values must be numeric"));
        }
        storage[index] = entry.asNumber();
    }
    return Transform3D(Mat4(storage));
}

[[nodiscard]] Result<GeometryRef> refFromJson(const JsonValue& value) {
    if(!value.isObject()) {
        return Result<GeometryRef>::failure(Status::invalidArgument("Geometry reference must be an object"));
    }
    Result<double> entityId = requireNumberField(value, "entityId");
    if(!entityId) {
        return Result<GeometryRef>::failure(entityId.status());
    }
    Result<double> kind = requireNumberField(value, "kind");
    if(!kind) {
        return Result<GeometryRef>::failure(kind.status());
    }
    const int kindValue = static_cast<int>(kind.value());
    if(kindValue < static_cast<int>(GeometryRefKind::WholeEntity)
       || kindValue > static_cast<int>(GeometryRefKind::Midpoint)) {
        return Result<GeometryRef>::failure(Status::invalidArgument("Unsupported geometry reference kind"));
    }
    return GeometryRef(
        EntityId(static_cast<EntityId::ValueType>(entityId.value())),
        static_cast<GeometryRefKind>(kindValue)
    );
}

[[nodiscard]] Result<std::vector<GeometryRef>> refsFromJson(const JsonValue& value) {
    Result<const JsonValue*> refsValue = requireArrayField(value, "refs");
    if(!refsValue) {
        return Result<std::vector<GeometryRef>>::failure(refsValue.status());
    }
    std::vector<GeometryRef> refs;
    for(const JsonValue& refValue : refsValue.value()->asArray()) {
        Result<GeometryRef> ref = refFromJson(refValue);
        if(!ref) {
            return Result<std::vector<GeometryRef>>::failure(ref.status());
        }
        refs.push_back(ref.value());
    }
    return refs;
}

[[nodiscard]] Status requireRefCount(const std::vector<GeometryRef>& refs, std::size_t count, const std::string& typeName) {
    if(refs.size() != count) {
        return Status::invalidArgument(typeName + " constraint has invalid reference count");
    }
    return Status::ok();
}

[[nodiscard]] Result<Sketch> sketchFromJson(const JsonValue& value) {
    if(!value.isObject()) {
        return Result<Sketch>::failure(Status::invalidArgument("Sketch value must be an object"));
    }
    Sketch sketch;
    Result<const JsonValue*> entities = requireArrayField(value, "entities");
    if(!entities) {
        return Result<Sketch>::failure(entities.status());
    }

    for(const JsonValue& entityValue : entities.value()->asArray()) {
        if(!entityValue.isObject()) {
            return Result<Sketch>::failure(Status::invalidArgument("Sketch entity must be an object"));
        }
        Result<double> idValue = requireNumberField(entityValue, "id");
        if(!idValue) {
            return Result<Sketch>::failure(idValue.status());
        }
        Result<double> typeValue = requireNumberField(entityValue, "type");
        if(!typeValue) {
            return Result<Sketch>::failure(typeValue.status());
        }
        const EntityId id(static_cast<EntityId::ValueType>(idValue.value()));
        const auto type = static_cast<SketchEntityType>(static_cast<int>(typeValue.value()));
        std::unique_ptr<SketchEntity> entity;
        switch(type) {
        case SketchEntityType::Point: {
            Result<const JsonValue*> pointValue = requireObjectField(entityValue, "point");
            if(!pointValue) {
                return Result<Sketch>::failure(pointValue.status());
            }
            Result<Point2> point = point2FromJson(*pointValue.value());
            if(!point) {
                return Result<Sketch>::failure(point.status());
            }
            entity = std::make_unique<SketchPoint>(id, point.value());
            break;
        }
        case SketchEntityType::Line: {
            Result<const JsonValue*> startValue = requireObjectField(entityValue, "start");
            Result<const JsonValue*> endValue = requireObjectField(entityValue, "end");
            if(!startValue) {
                return Result<Sketch>::failure(startValue.status());
            }
            if(!endValue) {
                return Result<Sketch>::failure(endValue.status());
            }
            Result<Point2> start = point2FromJson(*startValue.value());
            Result<Point2> end = point2FromJson(*endValue.value());
            if(!start) {
                return Result<Sketch>::failure(start.status());
            }
            if(!end) {
                return Result<Sketch>::failure(end.status());
            }
            entity = std::make_unique<SketchLine>(id, start.value(), end.value());
            break;
        }
        case SketchEntityType::Circle: {
            Result<const JsonValue*> centerValue = requireObjectField(entityValue, "center");
            if(!centerValue) {
                return Result<Sketch>::failure(centerValue.status());
            }
            Result<Point2> center = point2FromJson(*centerValue.value());
            Result<double> radius = requireNumberField(entityValue, "radius");
            if(!center) {
                return Result<Sketch>::failure(center.status());
            }
            if(!radius) {
                return Result<Sketch>::failure(radius.status());
            }
            entity = std::make_unique<SketchCircle>(id, center.value(), radius.value());
            break;
        }
        case SketchEntityType::Arc: {
            Result<const JsonValue*> centerValue = requireObjectField(entityValue, "center");
            if(!centerValue) {
                return Result<Sketch>::failure(centerValue.status());
            }
            Result<Point2> center = point2FromJson(*centerValue.value());
            Result<double> radius = requireNumberField(entityValue, "radius");
            Result<double> startAngle = requireNumberField(entityValue, "startAngle");
            Result<double> endAngle = requireNumberField(entityValue, "endAngle");
            if(!center) {
                return Result<Sketch>::failure(center.status());
            }
            if(!radius) {
                return Result<Sketch>::failure(radius.status());
            }
            if(!startAngle) {
                return Result<Sketch>::failure(startAngle.status());
            }
            if(!endAngle) {
                return Result<Sketch>::failure(endAngle.status());
            }
            entity = std::make_unique<SketchArc>(id, center.value(), radius.value(), startAngle.value(), endAngle.value());
            break;
        }
        case SketchEntityType::Spline: {
            Result<const JsonValue*> controls = requireArrayField(entityValue, "controlPoints");
            if(!controls) {
                return Result<Sketch>::failure(controls.status());
            }
            Result<double> degree = requireNumberField(entityValue, "degree");
            if(!degree) {
                return Result<Sketch>::failure(degree.status());
            }
            std::vector<Point2> points;
            for(const JsonValue& pointValue : controls.value()->asArray()) {
                Result<Point2> point = point2FromJson(pointValue);
                if(!point) {
                    return Result<Sketch>::failure(point.status());
                }
                points.push_back(point.value());
            }
            entity = std::make_unique<SketchSpline>(id, std::move(points), static_cast<int>(degree.value()));
            break;
        }
        default:
            return Result<Sketch>::failure(Status::invalidArgument("Unsupported sketch entity type"));
        }
        Status insertStatus = sketch.insertEntity(std::move(entity));
        if(!insertStatus) {
            return Result<Sketch>::failure(insertStatus);
        }
    }

    if(const JsonValue* ids = value.find("constraintIds"); ids != nullptr && ids->isArray()) {
        for(const JsonValue& idValue : ids->asArray()) {
            if(!idValue.isNumber()) {
                return Result<Sketch>::failure(Status::invalidArgument("Sketch constraint id must be numeric"));
            }
            sketch.addConstraintId(ConstraintId(static_cast<ConstraintId::ValueType>(idValue.asNumber())));
        }
    }
    return sketch;
}

[[nodiscard]] Result<std::unique_ptr<Feature>> featureFromJson(const JsonValue& value) {
    if(!value.isObject()) {
        return Result<std::unique_ptr<Feature>>::failure(Status::invalidArgument("Feature must be an object"));
    }
    Result<double> idValue = requireNumberField(value, "id");
    Result<std::string> nameValue = requireStringField(value, "name");
    Result<double> typeValue = requireNumberField(value, "type");
    if(!idValue) {
        return Result<std::unique_ptr<Feature>>::failure(idValue.status());
    }
    if(!nameValue) {
        return Result<std::unique_ptr<Feature>>::failure(nameValue.status());
    }
    if(!typeValue) {
        return Result<std::unique_ptr<Feature>>::failure(typeValue.status());
    }

    const FeatureId id(static_cast<FeatureId::ValueType>(idValue.value()));
    const std::string name = nameValue.value();
    const auto type = static_cast<FeatureType>(static_cast<int>(typeValue.value()));
    switch(type) {
    case FeatureType::Sketch: {
        Result<const JsonValue*> sketchValue = requireObjectField(value, "sketch");
        if(!sketchValue) {
            return Result<std::unique_ptr<Feature>>::failure(sketchValue.status());
        }
        Result<Sketch> sketch = sketchFromJson(*sketchValue.value());
        if(!sketch) {
            return Result<std::unique_ptr<Feature>>::failure(sketch.status());
        }
        std::unique_ptr<Feature> feature = std::make_unique<SketchFeature>(id, name, std::move(sketch.value()));
        return feature;
    }
    case FeatureType::Extrude: {
        Result<double> profileId = requireNumberField(value, "profileSketchFeatureId");
        Result<double> distance = requireNumberField(value, "distance");
        if(!profileId) {
            return Result<std::unique_ptr<Feature>>::failure(profileId.status());
        }
        if(!distance) {
            return Result<std::unique_ptr<Feature>>::failure(distance.status());
        }
        std::unique_ptr<Feature> feature = std::make_unique<ExtrudeFeature>(
            id,
            name,
            FeatureId(static_cast<FeatureId::ValueType>(profileId.value())),
            distance.value()
        );
        return feature;
    }
    case FeatureType::Revolve: {
        Result<double> profileId = requireNumberField(value, "profileSketchFeatureId");
        Result<double> angle = requireNumberField(value, "angleRadians");
        Result<const JsonValue*> axisValue = requireObjectField(value, "axis");
        if(!profileId) {
            return Result<std::unique_ptr<Feature>>::failure(profileId.status());
        }
        if(!angle) {
            return Result<std::unique_ptr<Feature>>::failure(angle.status());
        }
        if(!axisValue) {
            return Result<std::unique_ptr<Feature>>::failure(axisValue.status());
        }
        Result<const JsonValue*> originValue = requireObjectField(*axisValue.value(), "origin");
        Result<const JsonValue*> directionValue = requireObjectField(*axisValue.value(), "direction");
        if(!originValue) {
            return Result<std::unique_ptr<Feature>>::failure(originValue.status());
        }
        if(!directionValue) {
            return Result<std::unique_ptr<Feature>>::failure(directionValue.status());
        }
        Result<Point2> origin = point2FromJson(*originValue.value());
        Result<Vec2> direction = vec2FromJson(*directionValue.value());
        if(!origin) {
            return Result<std::unique_ptr<Feature>>::failure(origin.status());
        }
        if(!direction) {
            return Result<std::unique_ptr<Feature>>::failure(direction.status());
        }
        const RevolveAxis2D revolveAxis(origin.value(), direction.value());
        std::unique_ptr<Feature> feature = std::make_unique<RevolveFeature>(
            id,
            name,
            FeatureId(static_cast<FeatureId::ValueType>(profileId.value())),
            revolveAxis,
            angle.value()
        );
        return feature;
    }
    default:
        break;
    }
    return Result<std::unique_ptr<Feature>>::failure(Status::invalidArgument("Unsupported feature type"));
}

[[nodiscard]] Result<std::unique_ptr<Constraint>> constraintFromJson(const JsonValue& value) {
    if(!value.isObject()) {
        return Result<std::unique_ptr<Constraint>>::failure(Status::invalidArgument("Constraint must be an object"));
    }
    Result<double> idValue = requireNumberField(value, "id");
    Result<double> typeValue = requireNumberField(value, "type");
    if(!idValue) {
        return Result<std::unique_ptr<Constraint>>::failure(idValue.status());
    }
    if(!typeValue) {
        return Result<std::unique_ptr<Constraint>>::failure(typeValue.status());
    }
    const ConstraintId id(static_cast<ConstraintId::ValueType>(idValue.value()));
    const auto type = static_cast<ConstraintType>(static_cast<int>(typeValue.value()));
    const JsonValue* drivingValue = value.find("driving");
    if(drivingValue != nullptr && !drivingValue->isBool()) {
        return Result<std::unique_ptr<Constraint>>::failure(Status::invalidArgument("Constraint driving flag must be boolean"));
    }
    const bool driving = drivingValue == nullptr ? true : drivingValue->asBool(true);
    Result<std::vector<GeometryRef>> refsResult = refsFromJson(value);
    if(!refsResult) {
        return Result<std::unique_ptr<Constraint>>::failure(refsResult.status());
    }
    const std::vector<GeometryRef>& refs = refsResult.value();
    const JsonValue* numericValue = value.find("value");
    if(numericValue != nullptr && !numericValue->isNumber()) {
        return Result<std::unique_ptr<Constraint>>::failure(Status::invalidArgument("Constraint value must be numeric"));
    }
    const double numeric = numericValue == nullptr ? 0.0 : numericValue->asNumber();
    std::unique_ptr<Constraint> constraint;
    switch(type) {
    case ConstraintType::Coincident: {
        Status refsStatus = requireRefCount(refs, 2, "Coincident");
        if(!refsStatus) {
            return Result<std::unique_ptr<Constraint>>::failure(refsStatus);
        }
        constraint = std::make_unique<CoincidentConstraint>(id, refs[0], refs[1], driving);
        break;
    }
    case ConstraintType::Horizontal: {
        Status refsStatus = requireRefCount(refs, 1, "Horizontal");
        if(!refsStatus) {
            return Result<std::unique_ptr<Constraint>>::failure(refsStatus);
        }
        constraint = std::make_unique<HorizontalConstraint>(id, refs[0], driving);
        break;
    }
    case ConstraintType::Vertical: {
        Status refsStatus = requireRefCount(refs, 1, "Vertical");
        if(!refsStatus) {
            return Result<std::unique_ptr<Constraint>>::failure(refsStatus);
        }
        constraint = std::make_unique<VerticalConstraint>(id, refs[0], driving);
        break;
    }
    case ConstraintType::Parallel: {
        Status refsStatus = requireRefCount(refs, 2, "Parallel");
        if(!refsStatus) {
            return Result<std::unique_ptr<Constraint>>::failure(refsStatus);
        }
        constraint = std::make_unique<ParallelConstraint>(id, refs[0], refs[1], driving);
        break;
    }
    case ConstraintType::Perpendicular: {
        Status refsStatus = requireRefCount(refs, 2, "Perpendicular");
        if(!refsStatus) {
            return Result<std::unique_ptr<Constraint>>::failure(refsStatus);
        }
        constraint = std::make_unique<PerpendicularConstraint>(id, refs[0], refs[1], driving);
        break;
    }
    case ConstraintType::Distance: {
        Status refsStatus = requireRefCount(refs, 2, "Distance");
        if(!refsStatus) {
            return Result<std::unique_ptr<Constraint>>::failure(refsStatus);
        }
        if(numericValue == nullptr) {
            return Result<std::unique_ptr<Constraint>>::failure(Status::invalidArgument("Distance constraint is missing value"));
        }
        constraint = std::make_unique<DistanceConstraint>(id, refs[0], refs[1], numeric, driving);
        break;
    }
    case ConstraintType::Angle: {
        Status refsStatus = requireRefCount(refs, 2, "Angle");
        if(!refsStatus) {
            return Result<std::unique_ptr<Constraint>>::failure(refsStatus);
        }
        if(numericValue == nullptr) {
            return Result<std::unique_ptr<Constraint>>::failure(Status::invalidArgument("Angle constraint is missing value"));
        }
        constraint = std::make_unique<AngleConstraint>(id, refs[0], refs[1], numeric, driving);
        break;
    }
    case ConstraintType::Radius: {
        Status refsStatus = requireRefCount(refs, 1, "Radius");
        if(!refsStatus) {
            return Result<std::unique_ptr<Constraint>>::failure(refsStatus);
        }
        if(numericValue == nullptr) {
            return Result<std::unique_ptr<Constraint>>::failure(Status::invalidArgument("Radius constraint is missing value"));
        }
        constraint = std::make_unique<RadiusConstraint>(id, refs[0], numeric, driving);
        break;
    }
    case ConstraintType::Tangent: {
        Status refsStatus = requireRefCount(refs, 2, "Tangent");
        if(!refsStatus) {
            return Result<std::unique_ptr<Constraint>>::failure(refsStatus);
        }
        constraint = std::make_unique<TangentConstraint>(id, refs[0], refs[1], driving);
        break;
    }
    case ConstraintType::EqualLength: {
        Status refsStatus = requireRefCount(refs, 2, "EqualLength");
        if(!refsStatus) {
            return Result<std::unique_ptr<Constraint>>::failure(refsStatus);
        }
        constraint = std::make_unique<EqualLengthConstraint>(id, refs[0], refs[1], driving);
        break;
    }
    default:
        return Result<std::unique_ptr<Constraint>>::failure(Status::invalidArgument("Unsupported constraint type"));
    }
    if(const JsonValue* label = value.find("label"); label != nullptr) {
        if(!label->isString()) {
            return Result<std::unique_ptr<Constraint>>::failure(Status::invalidArgument("Constraint label must be a string"));
        }
        constraint->setLabel(label->asString());
    }
    return constraint;
}

} // namespace

Result<PartDocument> ProjectDeserializer::deserialize(const std::string& jsonText) const {
    Result<JsonValue> rootResult = JsonValue::parse(jsonText);
    if(!rootResult) {
        return Result<PartDocument>::failure(rootResult.status());
    }
    const JsonValue& root = rootResult.value();
    if(!root.isObject()) {
        return Result<PartDocument>::failure(Status::invalidArgument("Project root must be a JSON object"));
    }
    Result<double> schema = requireNumberField(root, "schemaVersion");
    if(!schema) {
        return Result<PartDocument>::failure(schema.status());
    }
    if(static_cast<int>(schema.value()) != kCurrentSchemaVersion) {
        return Result<PartDocument>::failure(Status::failedPrecondition("Unsupported project schema version"));
    }

    Result<const JsonValue*> documentField = requireObjectField(root, "document");
    if(!documentField) {
        return Result<PartDocument>::failure(documentField.status());
    }
    const JsonValue& documentJson = *documentField.value();
    Result<std::string> name = requireStringField(documentJson, "name");
    if(!name) {
        return Result<PartDocument>::failure(name.status());
    }
    PartDocument document(name.value());

    Result<const JsonValue*> features = requireArrayField(documentJson, "features");
    if(!features) {
        return Result<PartDocument>::failure(features.status());
    }
    for(const JsonValue& featureValue : features.value()->asArray()) {
        Result<std::unique_ptr<Feature>> feature = featureFromJson(featureValue);
        if(!feature) {
            return Result<PartDocument>::failure(feature.status());
        }
        Result<FeatureId> inserted = document.featureGraph().insertFeature(std::move(feature.value()));
        if(!inserted) {
            return Result<PartDocument>::failure(inserted.status());
        }
    }

    if(const JsonValue* constraints = documentJson.find("constraints"); constraints != nullptr) {
        if(!constraints->isArray()) {
            return Result<PartDocument>::failure(Status::invalidArgument("Project document constraints field must be an array"));
        }
        for(const JsonValue& constraintValue : constraints->asArray()) {
            Result<std::unique_ptr<Constraint>> constraint = constraintFromJson(constraintValue);
            if(!constraint) {
                return Result<PartDocument>::failure(constraint.status());
            }
            document.addConstraint(std::move(constraint.value()));
        }
    }
    document.featureGraph().markAllDirty();
    return document;
}

Result<AssemblyDocument> ProjectDeserializer::deserializeAssembly(const std::string& jsonText) const {
    Result<JsonValue> rootResult = JsonValue::parse(jsonText);
    if(!rootResult) {
        return Result<AssemblyDocument>::failure(rootResult.status());
    }
    const JsonValue& root = rootResult.value();
    if(!root.isObject()) {
        return Result<AssemblyDocument>::failure(Status::invalidArgument("Project root must be a JSON object"));
    }
    Result<double> schema = requireNumberField(root, "schemaVersion");
    if(!schema) {
        return Result<AssemblyDocument>::failure(schema.status());
    }
    if(static_cast<int>(schema.value()) != kCurrentSchemaVersion) {
        return Result<AssemblyDocument>::failure(Status::failedPrecondition("Unsupported project schema version"));
    }
    Result<const JsonValue*> assemblyValue = requireObjectField(root, "assembly");
    if(!assemblyValue) {
        return Result<AssemblyDocument>::failure(assemblyValue.status());
    }
    Result<std::string> name = requireStringField(*assemblyValue.value(), "name");
    if(!name) {
        return Result<AssemblyDocument>::failure(name.status());
    }

    AssemblyDocument assembly(name.value());
    Result<const JsonValue*> componentsValue = requireArrayField(*assemblyValue.value(), "components");
    if(!componentsValue) {
        return Result<AssemblyDocument>::failure(componentsValue.status());
    }
    for(const JsonValue& componentValue : componentsValue.value()->asArray()) {
        if(!componentValue.isObject()) {
            return Result<AssemblyDocument>::failure(Status::invalidArgument("Assembly component must be an object"));
        }
        Result<double> id = requireNumberField(componentValue, "id");
        Result<std::string> componentName = requireStringField(componentValue, "name");
        Result<std::string> sourceName = requireStringField(componentValue, "sourceDocumentName");
        Result<const JsonValue*> transformValue = requireObjectField(componentValue, "transform");
        if(!id) {
            return Result<AssemblyDocument>::failure(id.status());
        }
        if(!componentName) {
            return Result<AssemblyDocument>::failure(componentName.status());
        }
        if(!sourceName) {
            return Result<AssemblyDocument>::failure(sourceName.status());
        }
        if(!transformValue) {
            return Result<AssemblyDocument>::failure(transformValue.status());
        }
        Result<Transform3D> transform = transformFromJson(*transformValue.value());
        if(!transform) {
            return Result<AssemblyDocument>::failure(transform.status());
        }
        std::optional<ComponentId> parentId;
        if(const JsonValue* parentValue = componentValue.find("parentId"); parentValue != nullptr) {
            if(!parentValue->isNumber()) {
                return Result<AssemblyDocument>::failure(Status::invalidArgument("Assembly component parent id must be numeric"));
            }
            parentId = ComponentId(static_cast<ComponentId::ValueType>(parentValue->asNumber()));
        }
        auto part = std::make_shared<PartDocument>(sourceName.value());
        Result<ComponentId> inserted = assembly.insertComponentWithId(
            ComponentId(static_cast<ComponentId::ValueType>(id.value())),
            std::move(part),
            componentName.value(),
            transform.value(),
            parentId
        );
        if(!inserted) {
            return Result<AssemblyDocument>::failure(inserted.status());
        }
    }

    Result<const JsonValue*> matesValue = requireArrayField(*assemblyValue.value(), "mates");
    if(!matesValue) {
        return Result<AssemblyDocument>::failure(matesValue.status());
    }
    for(const JsonValue& mateValue : matesValue.value()->asArray()) {
        if(!mateValue.isObject()) {
            return Result<AssemblyDocument>::failure(Status::invalidArgument("Assembly mate must be an object"));
        }
        Result<double> id = requireNumberField(mateValue, "id");
        Result<double> type = requireNumberField(mateValue, "type");
        Result<double> firstComponentId = requireNumberField(mateValue, "firstComponentId");
        Result<double> secondComponentId = requireNumberField(mateValue, "secondComponentId");
        if(!id) {
            return Result<AssemblyDocument>::failure(id.status());
        }
        if(!type) {
            return Result<AssemblyDocument>::failure(type.status());
        }
        if(!firstComponentId) {
            return Result<AssemblyDocument>::failure(firstComponentId.status());
        }
        if(!secondComponentId) {
            return Result<AssemblyDocument>::failure(secondComponentId.status());
        }

        const ConstraintId mateId(static_cast<ConstraintId::ValueType>(id.value()));
        const ComponentId firstId(static_cast<ComponentId::ValueType>(firstComponentId.value()));
        const ComponentId secondId(static_cast<ComponentId::ValueType>(secondComponentId.value()));
        const auto mateType = static_cast<MateType>(static_cast<int>(type.value()));
        MateConstraint mate;
        if(mateType == MateType::Fixed) {
            Result<const JsonValue*> fixedValue = requireObjectField(mateValue, "fixedTransform");
            if(!fixedValue) {
                return Result<AssemblyDocument>::failure(fixedValue.status());
            }
            Result<Transform3D> fixedTransform = transformFromJson(*fixedValue.value());
            if(!fixedTransform) {
                return Result<AssemblyDocument>::failure(fixedTransform.status());
            }
            mate = MateConstraint::fixed(mateId, firstId, fixedTransform.value());
        } else {
            Result<const JsonValue*> firstPointValue = requireObjectField(mateValue, "firstLocalPoint");
            Result<const JsonValue*> secondPointValue = requireObjectField(mateValue, "secondLocalPoint");
            Result<const JsonValue*> firstDirectionValue = requireObjectField(mateValue, "firstLocalDirection");
            Result<const JsonValue*> secondDirectionValue = requireObjectField(mateValue, "secondLocalDirection");
            if(!firstPointValue) {
                return Result<AssemblyDocument>::failure(firstPointValue.status());
            }
            if(!secondPointValue) {
                return Result<AssemblyDocument>::failure(secondPointValue.status());
            }
            if(!firstDirectionValue) {
                return Result<AssemblyDocument>::failure(firstDirectionValue.status());
            }
            if(!secondDirectionValue) {
                return Result<AssemblyDocument>::failure(secondDirectionValue.status());
            }
            Result<Vec3> firstPoint = vec3FromJson(*firstPointValue.value());
            Result<Vec3> secondPoint = vec3FromJson(*secondPointValue.value());
            Result<Vec3> firstDirection = vec3FromJson(*firstDirectionValue.value());
            Result<Vec3> secondDirection = vec3FromJson(*secondDirectionValue.value());
            if(!firstPoint) {
                return Result<AssemblyDocument>::failure(firstPoint.status());
            }
            if(!secondPoint) {
                return Result<AssemblyDocument>::failure(secondPoint.status());
            }
            if(!firstDirection) {
                return Result<AssemblyDocument>::failure(firstDirection.status());
            }
            if(!secondDirection) {
                return Result<AssemblyDocument>::failure(secondDirection.status());
            }

            if(mateType == MateType::Coincident) {
                mate = MateConstraint::coincident(mateId, firstId, firstPoint.value(), secondId, secondPoint.value());
            } else if(mateType == MateType::Distance) {
                Result<double> distance = requireNumberField(mateValue, "distance");
                if(!distance) {
                    return Result<AssemblyDocument>::failure(distance.status());
                }
                mate = MateConstraint::distance(
                    mateId,
                    firstId,
                    firstPoint.value(),
                    secondId,
                    secondPoint.value(),
                    distance.value()
                );
            } else if(mateType == MateType::Parallel) {
                mate = MateConstraint::parallel(
                    mateId,
                    firstId,
                    firstDirection.value(),
                    secondId,
                    secondDirection.value()
                );
            } else {
                return Result<AssemblyDocument>::failure(Status::invalidArgument("Unsupported assembly mate type"));
            }
        }
        Status status = assembly.addMate(std::move(mate));
        if(!status) {
            return Result<AssemblyDocument>::failure(status);
        }
    }

    return assembly;
}

} // namespace mnx::persistence
