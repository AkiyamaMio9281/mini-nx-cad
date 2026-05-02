#include "mnx/sketch/SketchEntity.h"

namespace mnx {

SketchEntity::SketchEntity(EntityId id, SketchEntityType type) : id_(id), type_(type) {}

Point2 SketchEntity::applyTransform(const Point2& point) const {
    return Point2(transform_.applyToPoint(point.vector()));
}

Vec2 SketchEntity::applyTransform(const Vec2& vector) const {
    return transform_.applyToVector(vector);
}

} // namespace mnx
