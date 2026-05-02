#include "mnx/geometry/Bezier2D.h"

#include <algorithm>

namespace mnx {

Bezier2D::Bezier2D(const ControlPoints& controlPoints) : controlPoints_(controlPoints) {}

Point2 Bezier2D::evaluate(double t) const {
    const double clampedT = std::clamp(t, 0.0, 1.0);
    const double u = 1.0 - clampedT;
    const Vec2 value = controlPoints_[0].vector() * (u * u * u)
        + controlPoints_[1].vector() * (3.0 * u * u * clampedT)
        + controlPoints_[2].vector() * (3.0 * u * clampedT * clampedT)
        + controlPoints_[3].vector() * (clampedT * clampedT * clampedT);
    return Point2(value);
}

Vec2 Bezier2D::tangent(double t) const {
    const double clampedT = std::clamp(t, 0.0, 1.0);
    const double u = 1.0 - clampedT;
    return (controlPoints_[1] - controlPoints_[0]) * (3.0 * u * u)
        + (controlPoints_[2] - controlPoints_[1]) * (6.0 * u * clampedT)
        + (controlPoints_[3] - controlPoints_[2]) * (3.0 * clampedT * clampedT);
}

AABB2 Bezier2D::bounds() const {
    AABB2 box;
    constexpr int kSamples = 64;
    for(int index = 0; index <= kSamples; ++index) {
        box.expand(evaluate(static_cast<double>(index) / static_cast<double>(kSamples)).vector());
    }
    return box;
}

} // namespace mnx
