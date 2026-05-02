#include "mnx/sketch/SketchCircle.h"

#include "mnx/geometry/Circle2D.h"

#include <stdexcept>

namespace mnx {

SketchCircle::SketchCircle(EntityId id, const Point2& center, double radius)
    : SketchEntity(id, SketchEntityType::Circle), center_(center) {
    setRadius(radius);
}

void SketchCircle::setRadius(double radius) {
    if(radius < 0.0) {
        throw std::invalid_argument("SketchCircle radius must be non-negative");
    }
    radius_ = radius;
}

AABB2 SketchCircle::bounds() const {
    const Point2 transformedCenter = applyTransform(center_);
    const double transformedRadius = applyTransform(Vec2(radius_, 0.0)).norm();
    return Circle2D(transformedCenter, transformedRadius).bounds();
}

std::vector<GeometryRefKind> SketchCircle::supportedRefKinds() const {
    return {GeometryRefKind::WholeEntity, GeometryRefKind::CenterPoint};
}

Result<Point2> SketchCircle::resolvePoint(GeometryRefKind kind) const {
    if(kind == GeometryRefKind::CenterPoint || kind == GeometryRefKind::WholeEntity) {
        return applyTransform(center_);
    }
    return Result<Point2>::failure(Status::invalidArgument("Unsupported point reference for SketchCircle"));
}

std::unique_ptr<SketchEntity> SketchCircle::clone() const {
    return std::make_unique<SketchCircle>(*this);
}

} // namespace mnx
