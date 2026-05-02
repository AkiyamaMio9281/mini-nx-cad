#include "mnx/sketch/SketchPoint.h"

namespace mnx {

SketchPoint::SketchPoint(EntityId id, const Point2& point)
    : SketchEntity(id, SketchEntityType::Point), point_(point) {}

AABB2 SketchPoint::bounds() const {
    AABB2 box;
    box.expand(applyTransform(point_).vector());
    return box;
}

std::vector<GeometryRefKind> SketchPoint::supportedRefKinds() const {
    return {GeometryRefKind::WholeEntity, GeometryRefKind::CenterPoint};
}

Result<Point2> SketchPoint::resolvePoint(GeometryRefKind kind) const {
    if(kind == GeometryRefKind::WholeEntity || kind == GeometryRefKind::CenterPoint) {
        return applyTransform(point_);
    }
    return Result<Point2>::failure(Status::invalidArgument("Unsupported point reference for SketchPoint"));
}

std::unique_ptr<SketchEntity> SketchPoint::clone() const {
    return std::make_unique<SketchPoint>(*this);
}

} // namespace mnx
