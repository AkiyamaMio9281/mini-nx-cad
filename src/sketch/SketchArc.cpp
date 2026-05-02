#include "mnx/sketch/SketchArc.h"

#include "mnx/geometry/Arc2D.h"

#include <stdexcept>

namespace mnx {

SketchArc::SketchArc(EntityId id, const Point2& center, double radius, double startAngle, double endAngle)
    : SketchEntity(id, SketchEntityType::Arc), center_(center), startAngle_(startAngle), endAngle_(endAngle) {
    setRadius(radius);
}

void SketchArc::setRadius(double radius) {
    if(radius < 0.0) {
        throw std::invalid_argument("SketchArc radius must be non-negative");
    }
    radius_ = radius;
}

void SketchArc::setAngles(double startAngle, double endAngle) {
    startAngle_ = startAngle;
    endAngle_ = endAngle;
}

Point2 SketchArc::startPoint() const {
    return Arc2D(center_, radius_, startAngle_, endAngle_).startPoint();
}

Point2 SketchArc::endPoint() const {
    return Arc2D(center_, radius_, startAngle_, endAngle_).endPoint();
}

Point2 SketchArc::midpoint() const {
    return Arc2D(center_, radius_, startAngle_, endAngle_).evaluate(0.5);
}

AABB2 SketchArc::bounds() const {
    AABB2 box = Arc2D(center_, radius_, startAngle_, endAngle_).bounds();
    AABB2 transformed;
    transformed.expand(applyTransform(Point2(box.min())).vector());
    transformed.expand(applyTransform(Point2(box.max())).vector());
    return transformed;
}

std::vector<GeometryRefKind> SketchArc::supportedRefKinds() const {
    return {
        GeometryRefKind::WholeEntity,
        GeometryRefKind::StartPoint,
        GeometryRefKind::EndPoint,
        GeometryRefKind::CenterPoint,
        GeometryRefKind::Midpoint,
    };
}

Result<Point2> SketchArc::resolvePoint(GeometryRefKind kind) const {
    switch(kind) {
    case GeometryRefKind::StartPoint:
        return applyTransform(startPoint());
    case GeometryRefKind::EndPoint:
        return applyTransform(endPoint());
    case GeometryRefKind::CenterPoint:
        return applyTransform(center_);
    case GeometryRefKind::Midpoint:
    case GeometryRefKind::WholeEntity:
        return applyTransform(midpoint());
    }
    return Result<Point2>::failure(Status::invalidArgument("Unsupported point reference for SketchArc"));
}

std::unique_ptr<SketchEntity> SketchArc::clone() const {
    return std::make_unique<SketchArc>(*this);
}

} // namespace mnx
