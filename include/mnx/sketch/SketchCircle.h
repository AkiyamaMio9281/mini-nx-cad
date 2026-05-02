#pragma once

#include "mnx/sketch/SketchEntity.h"

namespace mnx {

class SketchCircle final : public SketchEntity {
public:
    SketchCircle(EntityId id, const Point2& center, double radius);

    [[nodiscard]] const Point2& center() const {
        return center_;
    }

    [[nodiscard]] double radius() const {
        return radius_;
    }

    void setCenter(const Point2& center) {
        center_ = center;
    }

    void setRadius(double radius);

    [[nodiscard]] AABB2 bounds() const override;
    [[nodiscard]] std::vector<GeometryRefKind> supportedRefKinds() const override;
    [[nodiscard]] Result<Point2> resolvePoint(GeometryRefKind kind) const override;
    [[nodiscard]] std::unique_ptr<SketchEntity> clone() const override;

private:
    Point2 center_;
    double radius_ = 0.0;
};

} // namespace mnx
