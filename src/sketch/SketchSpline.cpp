#include "mnx/sketch/SketchSpline.h"

#include <stdexcept>
#include <utility>

namespace mnx {

SketchSpline::SketchSpline(EntityId id, std::vector<Point2> controlPoints, int degree)
    : SketchEntity(id, SketchEntityType::Spline), controlPoints_(std::move(controlPoints)), degree_(degree) {
    if(degree_ < 1) {
        throw std::invalid_argument("SketchSpline degree must be positive");
    }
}

void SketchSpline::setControlPoints(std::vector<Point2> controlPoints) {
    controlPoints_ = std::move(controlPoints);
}

void SketchSpline::setDegree(int degree) {
    if(degree < 1) {
        throw std::invalid_argument("SketchSpline degree must be positive");
    }
    degree_ = degree;
}

AABB2 SketchSpline::bounds() const {
    AABB2 box;
    for(const Point2& point : controlPoints_) {
        box.expand(applyTransform(point).vector());
    }
    return box;
}

std::vector<GeometryRefKind> SketchSpline::supportedRefKinds() const {
    return {
        GeometryRefKind::WholeEntity,
        GeometryRefKind::StartPoint,
        GeometryRefKind::EndPoint,
        GeometryRefKind::Midpoint,
    };
}

Result<Point2> SketchSpline::resolvePoint(GeometryRefKind kind) const {
    if(controlPoints_.empty()) {
        return Result<Point2>::failure(Status::failedPrecondition("SketchSpline has no control points"));
    }

    switch(kind) {
    case GeometryRefKind::StartPoint:
        return applyTransform(controlPoints_.front());
    case GeometryRefKind::EndPoint:
        return applyTransform(controlPoints_.back());
    case GeometryRefKind::Midpoint:
    case GeometryRefKind::WholeEntity:
        return applyTransform(controlPoints_[controlPoints_.size() / 2]);
    case GeometryRefKind::CenterPoint:
        break;
    }
    return Result<Point2>::failure(Status::invalidArgument("Unsupported point reference for SketchSpline"));
}

std::unique_ptr<SketchEntity> SketchSpline::clone() const {
    return std::make_unique<SketchSpline>(*this);
}

} // namespace mnx
