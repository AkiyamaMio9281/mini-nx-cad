#include "mnx/geometry/Line2D.h"

#include <stdexcept>

namespace mnx {

Line2D::Line2D(const Point2& point, const Vec2& direction) : point_(point) {
    if(isZero(direction.squaredNorm())) {
        throw std::invalid_argument("Line2D direction must be non-zero");
    }
    direction_ = direction.normalized();
}

Point2 Line2D::evaluate(double parameter) const {
    return point_ + direction_ * parameter;
}

Point2 Line2D::projection(const Point2& point) const {
    return evaluate((point - point_).dot(direction_));
}

double Line2D::distanceTo(const Point2& point) const {
    return std::abs((point - point_).cross(direction_));
}

} // namespace mnx
