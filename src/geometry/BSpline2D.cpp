#include "mnx/geometry/BSpline2D.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace mnx {

BSpline2D::BSpline2D(std::vector<Point2> controlPoints, int degree)
    : controlPoints_(std::move(controlPoints)), degree_(degree) {
    if(degree < 1) {
        throw std::invalid_argument("BSpline2D degree must be positive");
    }
}

Point2 BSpline2D::evaluate(double t) const {
    if(controlPoints_.empty()) {
        throw std::runtime_error("Cannot evaluate a BSpline2D without control points");
    }
    if(controlPoints_.size() == 1) {
        return controlPoints_.front();
    }

    const double scaled = std::clamp(t, 0.0, 1.0) * static_cast<double>(controlPoints_.size() - 1);
    const auto index = static_cast<std::size_t>(scaled);
    if(index >= controlPoints_.size() - 1) {
        return controlPoints_.back();
    }

    const double localT = scaled - static_cast<double>(index);
    return controlPoints_[index] + (controlPoints_[index + 1] - controlPoints_[index]) * localT;
}

Vec2 BSpline2D::tangent(double t) const {
    if(controlPoints_.size() < 2) {
        return {};
    }

    const double scaled = std::clamp(t, 0.0, 1.0) * static_cast<double>(controlPoints_.size() - 1);
    const std::size_t index = std::min(static_cast<std::size_t>(scaled), controlPoints_.size() - 2);
    return controlPoints_[index + 1] - controlPoints_[index];
}

AABB2 BSpline2D::bounds() const {
    AABB2 box;
    for(const Point2& point : controlPoints_) {
        box.expand(point.vector());
    }
    return box;
}

} // namespace mnx
