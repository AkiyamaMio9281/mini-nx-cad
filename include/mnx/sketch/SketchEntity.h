#pragma once

#include "mnx/core/ids/EntityId.h"
#include "mnx/core/math/AABB2.h"
#include "mnx/core/math/Transform2D.h"
#include "mnx/core/utils/Result.h"
#include "mnx/geometry/Point2.h"
#include "mnx/sketch/GeometryRef.h"
#include "mnx/sketch/SketchEntityType.h"

#include <vector>
#include <memory>

namespace mnx {

class SketchEntity {
public:
    explicit SketchEntity(EntityId id, SketchEntityType type);
    virtual ~SketchEntity() = default;

    [[nodiscard]] EntityId id() const {
        return id_;
    }

    [[nodiscard]] SketchEntityType type() const {
        return type_;
    }

    [[nodiscard]] const Transform2D& transform() const {
        return transform_;
    }

    void setTransform(const Transform2D& transform) {
        transform_ = transform;
    }

    [[nodiscard]] virtual AABB2 bounds() const = 0;
    [[nodiscard]] virtual std::vector<GeometryRefKind> supportedRefKinds() const = 0;
    [[nodiscard]] virtual Result<Point2> resolvePoint(GeometryRefKind kind) const = 0;
    [[nodiscard]] virtual std::unique_ptr<SketchEntity> clone() const = 0;

protected:
    [[nodiscard]] Point2 applyTransform(const Point2& point) const;
    [[nodiscard]] Vec2 applyTransform(const Vec2& vector) const;

private:
    EntityId id_;
    SketchEntityType type_;
    Transform2D transform_;
};

} // namespace mnx
