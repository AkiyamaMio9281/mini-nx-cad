#pragma once

#include "mnx/assembly/MateType.h"
#include "mnx/core/ids/ComponentId.h"
#include "mnx/core/ids/ConstraintId.h"
#include "mnx/core/math/Transform3D.h"
#include "mnx/core/math/Vec3.h"

#include <optional>
#include <string>

namespace mnx {

class MateConstraint {
public:
    MateConstraint() = default;

    [[nodiscard]] static MateConstraint fixed(
        ConstraintId id,
        ComponentId componentId,
        Transform3D targetTransform,
        std::string label = {}
    );
    [[nodiscard]] static MateConstraint coincident(
        ConstraintId id,
        ComponentId firstComponentId,
        Vec3 firstLocalPoint,
        ComponentId secondComponentId,
        Vec3 secondLocalPoint,
        std::string label = {}
    );
    [[nodiscard]] static MateConstraint distance(
        ConstraintId id,
        ComponentId firstComponentId,
        Vec3 firstLocalPoint,
        ComponentId secondComponentId,
        Vec3 secondLocalPoint,
        double distance,
        std::string label = {}
    );
    [[nodiscard]] static MateConstraint parallel(
        ConstraintId id,
        ComponentId firstComponentId,
        Vec3 firstLocalDirection,
        ComponentId secondComponentId,
        Vec3 secondLocalDirection,
        std::string label = {}
    );

    [[nodiscard]] ConstraintId id() const;
    [[nodiscard]] MateType type() const;
    [[nodiscard]] ComponentId firstComponentId() const;
    [[nodiscard]] ComponentId secondComponentId() const;
    [[nodiscard]] const Vec3& firstLocalPoint() const;
    [[nodiscard]] const Vec3& secondLocalPoint() const;
    [[nodiscard]] const Vec3& firstLocalDirection() const;
    [[nodiscard]] const Vec3& secondLocalDirection() const;
    [[nodiscard]] double distance() const;
    [[nodiscard]] const Transform3D& fixedTransform() const;
    [[nodiscard]] const std::string& label() const;

private:
    ConstraintId id_;
    MateType type_ = MateType::Fixed;
    ComponentId firstComponentId_;
    ComponentId secondComponentId_;
    Vec3 firstLocalPoint_;
    Vec3 secondLocalPoint_;
    Vec3 firstLocalDirection_ {1.0, 0.0, 0.0};
    Vec3 secondLocalDirection_ {1.0, 0.0, 0.0};
    double distance_ = 0.0;
    Transform3D fixedTransform_;
    std::string label_;
};

} // namespace mnx
