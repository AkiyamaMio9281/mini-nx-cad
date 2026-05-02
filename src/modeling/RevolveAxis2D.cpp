#include "mnx/modeling/RevolveAxis2D.h"

#include "mnx/core/math/Tolerance.h"

namespace mnx {

RevolveAxis2D::RevolveAxis2D(const Point2& origin, const Vec2& direction) : origin_(origin) {
    if(direction.norm() > kDefaultTolerance) {
        direction_ = direction.normalized();
        valid_ = true;
    }
}

bool RevolveAxis2D::isValid() const {
    return valid_;
}

const Point2& RevolveAxis2D::origin() const {
    return origin_;
}

const Vec2& RevolveAxis2D::direction() const {
    return direction_;
}

double RevolveAxis2D::signedDistance(const Point2& point) const {
    return (point - origin_).cross(direction_);
}

double RevolveAxis2D::axialCoordinate(const Point2& point) const {
    return (point - origin_).dot(direction_);
}

} // namespace mnx
