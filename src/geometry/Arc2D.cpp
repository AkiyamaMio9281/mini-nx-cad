#include "mnx/geometry/Arc2D.h"

#include <array>
#include <cmath>
#include <stdexcept>

namespace {

constexpr double kTwoPi = 6.28318530717958647692;

double normalizePositive(double angle) {
    double normalized = std::fmod(angle, kTwoPi);
    if(normalized < 0.0) {
        normalized += kTwoPi;
    }
    return normalized;
}

} // namespace

namespace mnx {

Arc2D::Arc2D(
    const Point2& center,
    double radius,
    double startAngle,
    double endAngle,
    bool counterClockwise
) : center_(center), radius_(radius), startAngle_(startAngle), endAngle_(endAngle) {
    if(radius < 0.0) {
        throw std::invalid_argument("Arc2D radius must be non-negative");
    }

    sweepAngle_ = endAngle - startAngle;
    if(counterClockwise && sweepAngle_ < 0.0) {
        sweepAngle_ += kTwoPi;
    }
    if(!counterClockwise && sweepAngle_ > 0.0) {
        sweepAngle_ -= kTwoPi;
    }
}

Point2 Arc2D::startPoint() const {
    return evaluate(0.0);
}

Point2 Arc2D::endPoint() const {
    return evaluate(1.0);
}

Point2 Arc2D::evaluate(double t) const {
    const double angle = startAngle_ + sweepAngle_ * t;
    return center_ + Vec2(std::cos(angle) * radius_, std::sin(angle) * radius_);
}

Vec2 Arc2D::tangent(double t) const {
    const double angle = startAngle_ + sweepAngle_ * t;
    const double direction = sweepAngle_ >= 0.0 ? 1.0 : -1.0;
    return Vec2(-std::sin(angle) * direction, std::cos(angle) * direction);
}

bool Arc2D::containsAngle(double angle, double tolerance) const {
    if(isZero(std::abs(sweepAngle_) - kTwoPi, tolerance)) {
        return true;
    }

    const double start = normalizePositive(startAngle_);
    const double candidate = normalizePositive(angle);
    const double sweep = std::abs(sweepAngle_);
    const double relative =
        sweepAngle_ >= 0.0 ? normalizePositive(candidate - start) : normalizePositive(start - candidate);

    return relative <= sweep + tolerance;
}

AABB2 Arc2D::bounds() const {
    AABB2 box;
    box.expand(startPoint().vector());
    box.expand(endPoint().vector());

    const std::array<double, 4> cardinalAngles {
        0.0,
        1.57079632679489661923,
        3.14159265358979323846,
        4.71238898038468985769,
    };
    for(const double angle : cardinalAngles) {
        if(containsAngle(angle)) {
            box.expand((center_ + Vec2(std::cos(angle) * radius_, std::sin(angle) * radius_)).vector());
        }
    }

    return box;
}

} // namespace mnx
