#include "mnx/sketch/SketchLine.h"

namespace mnx {

SketchLine::SketchLine(EntityId id, const Point2& start, const Point2& end)
    : SketchEntity(id, SketchEntityType::Line), start_(start), end_(end) {}

Point2 SketchLine::midpoint() const {
    return Point2((start_.vector() + end_.vector()) * 0.5);
}

AABB2 SketchLine::bounds() const {
    AABB2 box;
    box.expand(applyTransform(start_).vector());
    box.expand(applyTransform(end_).vector());
    return box;
}

std::vector<GeometryRefKind> SketchLine::supportedRefKinds() const {
    return {
        GeometryRefKind::WholeEntity,
        GeometryRefKind::StartPoint,
        GeometryRefKind::EndPoint,
        GeometryRefKind::Midpoint,
    };
}

Result<Point2> SketchLine::resolvePoint(GeometryRefKind kind) const {
    switch(kind) {
    case GeometryRefKind::StartPoint:
        return applyTransform(start_);
    case GeometryRefKind::EndPoint:
        return applyTransform(end_);
    case GeometryRefKind::Midpoint:
    case GeometryRefKind::WholeEntity:
        return applyTransform(midpoint());
    case GeometryRefKind::CenterPoint:
        break;
    }
    return Result<Point2>::failure(Status::invalidArgument("Unsupported point reference for SketchLine"));
}

std::unique_ptr<SketchEntity> SketchLine::clone() const {
    return std::make_unique<SketchLine>(*this);
}

} // namespace mnx
