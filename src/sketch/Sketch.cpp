#include "mnx/sketch/Sketch.h"

#include <algorithm>
#include <utility>

namespace mnx {

Sketch::Sketch(SketchPlane plane) : plane_(std::move(plane)) {}

SketchPoint& Sketch::addPoint(const Point2& point) {
    auto entity = std::make_unique<SketchPoint>(nextEntityId(), point);
    return static_cast<SketchPoint&>(addEntity(std::move(entity)));
}

SketchLine& Sketch::addLine(const Point2& start, const Point2& end) {
    auto entity = std::make_unique<SketchLine>(nextEntityId(), start, end);
    return static_cast<SketchLine&>(addEntity(std::move(entity)));
}

SketchCircle& Sketch::addCircle(const Point2& center, double radius) {
    auto entity = std::make_unique<SketchCircle>(nextEntityId(), center, radius);
    return static_cast<SketchCircle&>(addEntity(std::move(entity)));
}

SketchArc& Sketch::addArc(const Point2& center, double radius, double startAngle, double endAngle) {
    auto entity = std::make_unique<SketchArc>(nextEntityId(), center, radius, startAngle, endAngle);
    return static_cast<SketchArc&>(addEntity(std::move(entity)));
}

SketchSpline& Sketch::addSpline(std::vector<Point2> controlPoints, int degree) {
    auto entity = std::make_unique<SketchSpline>(nextEntityId(), std::move(controlPoints), degree);
    return static_cast<SketchSpline&>(addEntity(std::move(entity)));
}

Status Sketch::insertEntity(std::unique_ptr<SketchEntity> entity) {
    if(!entity || !entity->id().isValid()) {
        return Status::invalidArgument("Cannot insert an invalid sketch entity");
    }
    if(findEntity(entity->id()) != nullptr) {
        return Status::failedPrecondition("Sketch entity id already exists");
    }

    advanceNextEntityIdPast(entity->id());
    entities_.push_back(std::move(entity));
    return Status::ok();
}

Status Sketch::removeEntity(EntityId id) {
    const auto iterator = std::find_if(entities_.begin(), entities_.end(), [id](const auto& entity) {
        return entity->id() == id;
    });
    if(iterator == entities_.end()) {
        return Status::notFound("Sketch entity was not found");
    }

    entities_.erase(iterator);
    return Status::ok();
}

SketchEntity* Sketch::findEntity(EntityId id) {
    const auto iterator = std::find_if(entities_.begin(), entities_.end(), [id](const auto& entity) {
        return entity->id() == id;
    });
    return iterator == entities_.end() ? nullptr : iterator->get();
}

const SketchEntity* Sketch::findEntity(EntityId id) const {
    const auto iterator = std::find_if(entities_.begin(), entities_.end(), [id](const auto& entity) {
        return entity->id() == id;
    });
    return iterator == entities_.end() ? nullptr : iterator->get();
}

std::vector<SketchEntity*> Sketch::entities() {
    std::vector<SketchEntity*> result;
    result.reserve(entities_.size());
    for(const auto& entity : entities_) {
        result.push_back(entity.get());
    }
    return result;
}

std::vector<const SketchEntity*> Sketch::entities() const {
    std::vector<const SketchEntity*> result;
    result.reserve(entities_.size());
    for(const auto& entity : entities_) {
        result.push_back(entity.get());
    }
    return result;
}

AABB2 Sketch::bounds() const {
    AABB2 box;
    for(const auto& entity : entities_) {
        box.expand(entity->bounds());
    }
    return box;
}

Result<Point2> Sketch::resolvePoint(const GeometryRef& ref) const {
    const SketchEntity* entity = findEntity(ref.entityId());
    if(entity == nullptr) {
        return Result<Point2>::failure(Status::notFound("GeometryRef entity was not found"));
    }
    return entity->resolvePoint(ref.kind());
}

bool Sketch::supportsRef(const GeometryRef& ref) const {
    const SketchEntity* entity = findEntity(ref.entityId());
    if(entity == nullptr) {
        return false;
    }

    const std::vector<GeometryRefKind> kinds = entity->supportedRefKinds();
    return std::find(kinds.begin(), kinds.end(), ref.kind()) != kinds.end();
}

void Sketch::addConstraintId(ConstraintId id) {
    if(id.isValid() && !hasConstraintId(id)) {
        constraintIds_.push_back(id);
    }
}

bool Sketch::removeConstraintId(ConstraintId id) {
    const auto oldSize = constraintIds_.size();
    constraintIds_.erase(std::remove(constraintIds_.begin(), constraintIds_.end(), id), constraintIds_.end());
    return constraintIds_.size() != oldSize;
}

bool Sketch::hasConstraintId(ConstraintId id) const {
    return std::find(constraintIds_.begin(), constraintIds_.end(), id) != constraintIds_.end();
}

EntityId Sketch::nextEntityId() {
    return EntityId(nextEntityId_++);
}

SketchEntity& Sketch::addEntity(std::unique_ptr<SketchEntity> entity) {
    advanceNextEntityIdPast(entity->id());
    SketchEntity& reference = *entity;
    entities_.push_back(std::move(entity));
    return reference;
}

void Sketch::advanceNextEntityIdPast(EntityId id) {
    if(id.value() >= nextEntityId_) {
        nextEntityId_ = id.value() + 1;
    }
}

} // namespace mnx
