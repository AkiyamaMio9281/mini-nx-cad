#pragma once

#include "mnx/sketch/SketchEntity.h"

namespace mnx {

class SketchPoint final : public SketchEntity {
public:
    SketchPoint(EntityId id, const Point2& point);

    [[nodiscard]] const Point2& point() const {
        return point_;
    }

    void setPoint(const Point2& point) {
        point_ = point;
    }

    [[nodiscard]] AABB2 bounds() const override;
    [[nodiscard]] std::vector<GeometryRefKind> supportedRefKinds() const override;
    [[nodiscard]] Result<Point2> resolvePoint(GeometryRefKind kind) const override;
    [[nodiscard]] std::unique_ptr<SketchEntity> clone() const override;

private:
    Point2 point_;
};

} // namespace mnx
