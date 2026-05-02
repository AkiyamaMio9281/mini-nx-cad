#pragma once

#include "mnx/sketch/SketchEntity.h"

namespace mnx {

class SketchLine final : public SketchEntity {
public:
    SketchLine(EntityId id, const Point2& start, const Point2& end);

    [[nodiscard]] const Point2& start() const {
        return start_;
    }

    [[nodiscard]] const Point2& end() const {
        return end_;
    }

    void setStart(const Point2& start) {
        start_ = start;
    }

    void setEnd(const Point2& end) {
        end_ = end;
    }

    [[nodiscard]] Point2 midpoint() const;
    [[nodiscard]] AABB2 bounds() const override;
    [[nodiscard]] std::vector<GeometryRefKind> supportedRefKinds() const override;
    [[nodiscard]] Result<Point2> resolvePoint(GeometryRefKind kind) const override;
    [[nodiscard]] std::unique_ptr<SketchEntity> clone() const override;

private:
    Point2 start_;
    Point2 end_;
};

} // namespace mnx
