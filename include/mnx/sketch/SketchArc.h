#pragma once

#include "mnx/sketch/SketchEntity.h"

namespace mnx {

class SketchArc final : public SketchEntity {
public:
    SketchArc(EntityId id, const Point2& center, double radius, double startAngle, double endAngle);

    [[nodiscard]] const Point2& center() const {
        return center_;
    }

    [[nodiscard]] double radius() const {
        return radius_;
    }

    [[nodiscard]] double startAngle() const {
        return startAngle_;
    }

    [[nodiscard]] double endAngle() const {
        return endAngle_;
    }

    void setCenter(const Point2& center) {
        center_ = center;
    }

    void setRadius(double radius);
    void setAngles(double startAngle, double endAngle);

    [[nodiscard]] Point2 startPoint() const;
    [[nodiscard]] Point2 endPoint() const;
    [[nodiscard]] Point2 midpoint() const;
    [[nodiscard]] AABB2 bounds() const override;
    [[nodiscard]] std::vector<GeometryRefKind> supportedRefKinds() const override;
    [[nodiscard]] Result<Point2> resolvePoint(GeometryRefKind kind) const override;
    [[nodiscard]] std::unique_ptr<SketchEntity> clone() const override;

private:
    Point2 center_;
    double radius_ = 0.0;
    double startAngle_ = 0.0;
    double endAngle_ = 0.0;
};

} // namespace mnx
