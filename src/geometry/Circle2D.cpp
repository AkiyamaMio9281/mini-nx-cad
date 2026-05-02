#include "mnx/geometry/Circle2D.h"

#include <cmath>
#include <stdexcept>

namespace mnx {

Circle2D::Circle2D(const Point2& center, double radius) : center_(center), radius_(radius) {
    if(radius < 0.0) {
        throw std::invalid_argument("Circle2D radius must be non-negative");
    }
}

Point2 Circle2D::evaluate(double angleRadians) const {
    return center_ + Vec2(std::cos(angleRadians) * radius_, std::sin(angleRadians) * radius_);
}

AABB2 Circle2D::bounds() const {
    return AABB2(
        (center_ - Vec2(radius_, radius_)).vector(),
        (center_ + Vec2(radius_, radius_)).vector()
    );
}

} // namespace mnx
