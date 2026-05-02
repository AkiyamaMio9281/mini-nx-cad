#pragma once

#include "mnx/sketch/SketchEntity.h"

#include <span>
#include <vector>

namespace mnx {

class SketchSpline final : public SketchEntity {
public:
    SketchSpline(EntityId id, std::vector<Point2> controlPoints, int degree = 3);

    [[nodiscard]] std::span<const Point2> controlPoints() const {
        return controlPoints_;
    }

    [[nodiscard]] int degree() const {
        return degree_;
    }

    void setControlPoints(std::vector<Point2> controlPoints);
    void setDegree(int degree);

    [[nodiscard]] AABB2 bounds() const override;
    [[nodiscard]] std::vector<GeometryRefKind> supportedRefKinds() const override;
    [[nodiscard]] Result<Point2> resolvePoint(GeometryRefKind kind) const override;
    [[nodiscard]] std::unique_ptr<SketchEntity> clone() const override;

private:
    std::vector<Point2> controlPoints_;
    int degree_ = 3;
};

} // namespace mnx
