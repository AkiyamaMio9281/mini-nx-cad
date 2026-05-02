#pragma once

#include "mnx/core/math/Mat4.h"
#include "mnx/core/math/Vec3.h"

namespace mnx::rendering {

struct Ray {
    Vec3 origin;
    Vec3 direction;
};

class Camera {
public:
    Camera();

    [[nodiscard]] const Vec3& target() const;
    void setTarget(const Vec3& target);
    [[nodiscard]] double distance() const;
    void setDistance(double distance);
    [[nodiscard]] double yaw() const;
    [[nodiscard]] double pitch() const;
    [[nodiscard]] Vec3 position() const;
    [[nodiscard]] Vec3 forward() const;
    [[nodiscard]] Vec3 right() const;
    [[nodiscard]] Vec3 up() const;

    void orbit(double deltaYaw, double deltaPitch);
    void pan(double deltaRight, double deltaUp);
    void zoom(double scale);

    [[nodiscard]] Mat4 viewMatrix() const;
    [[nodiscard]] Mat4 projectionMatrix(double aspectRatio) const;
    [[nodiscard]] Ray screenRay(double normalizedX, double normalizedY, double aspectRatio) const;

private:
    Vec3 target_;
    double distance_ = 10.0;
    double yaw_ = 0.0;
    double pitch_ = 0.45;
    double verticalFovRadians_ = 0.7853981633974483;
    double nearPlane_ = 0.01;
    double farPlane_ = 10000.0;
};

} // namespace mnx::rendering
