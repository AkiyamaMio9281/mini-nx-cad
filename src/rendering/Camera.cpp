#include "mnx/rendering/Camera.h"

#include "mnx/core/math/Tolerance.h"

#include <algorithm>
#include <cmath>

namespace mnx::rendering {

namespace {

constexpr double kMinDistance = 0.05;
constexpr double kMinPitch = -1.5;
constexpr double kMaxPitch = 1.5;

[[nodiscard]] Mat4 lookAt(const Vec3& eye, const Vec3& target, const Vec3& upHint) {
    const Vec3 forward = (target - eye).normalized();
    const Vec3 right = forward.cross(upHint).normalized();
    const Vec3 up = right.cross(forward).normalized();
    return Mat4(
        {right.x(), right.y(), right.z(), -right.dot(eye),
         up.x(), up.y(), up.z(), -up.dot(eye),
         -forward.x(), -forward.y(), -forward.z(), forward.dot(eye),
         0.0, 0.0, 0.0, 1.0}
    );
}

} // namespace

Camera::Camera() = default;

const Vec3& Camera::target() const {
    return target_;
}

void Camera::setTarget(const Vec3& target) {
    target_ = target;
}

double Camera::distance() const {
    return distance_;
}

void Camera::setDistance(double distance) {
    distance_ = std::max(distance, kMinDistance);
}

double Camera::yaw() const {
    return yaw_;
}

double Camera::pitch() const {
    return pitch_;
}

Vec3 Camera::position() const {
    const double cosPitch = std::cos(pitch_);
    const Vec3 offset(
        distance_ * cosPitch * std::sin(yaw_),
        distance_ * std::sin(pitch_),
        distance_ * cosPitch * std::cos(yaw_)
    );
    return target_ + offset;
}

Vec3 Camera::forward() const {
    return (target_ - position()).normalized();
}

Vec3 Camera::right() const {
    return forward().cross(Vec3(0.0, 1.0, 0.0)).normalized();
}

Vec3 Camera::up() const {
    return right().cross(forward()).normalized();
}

void Camera::orbit(double deltaYaw, double deltaPitch) {
    yaw_ += deltaYaw;
    pitch_ = std::clamp(pitch_ + deltaPitch, kMinPitch, kMaxPitch);
}

void Camera::pan(double deltaRight, double deltaUp) {
    target_ += right() * deltaRight + up() * deltaUp;
}

void Camera::zoom(double scale) {
    if(scale > kDefaultTolerance) {
        setDistance(distance_ * scale);
    }
}

Mat4 Camera::viewMatrix() const {
    return lookAt(position(), target_, Vec3(0.0, 1.0, 0.0));
}

Mat4 Camera::projectionMatrix(double aspectRatio) const {
    const double safeAspect = aspectRatio > kDefaultTolerance ? aspectRatio : 1.0;
    const double f = 1.0 / std::tan(verticalFovRadians_ * 0.5);
    return Mat4(
        {f / safeAspect, 0.0, 0.0, 0.0,
         0.0, f, 0.0, 0.0,
         0.0, 0.0, (farPlane_ + nearPlane_) / (nearPlane_ - farPlane_),
         (2.0 * farPlane_ * nearPlane_) / (nearPlane_ - farPlane_),
         0.0, 0.0, -1.0, 0.0}
    );
}

Ray Camera::screenRay(double normalizedX, double normalizedY, double aspectRatio) const {
    const double safeAspect = aspectRatio > kDefaultTolerance ? aspectRatio : 1.0;
    const double halfHeight = std::tan(verticalFovRadians_ * 0.5);
    const double halfWidth = halfHeight * safeAspect;
    const Vec3 direction =
        (forward() + right() * (normalizedX * halfWidth) + up() * (normalizedY * halfHeight)).normalized();
    return {position(), direction};
}

} // namespace mnx::rendering
