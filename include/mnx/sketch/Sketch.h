#pragma once

#include "mnx/core/ids/ConstraintId.h"
#include "mnx/core/utils/Result.h"
#include "mnx/sketch/SketchArc.h"
#include "mnx/sketch/SketchCircle.h"
#include "mnx/sketch/SketchLine.h"
#include "mnx/sketch/SketchPlane.h"
#include "mnx/sketch/SketchPoint.h"
#include "mnx/sketch/SketchSpline.h"

#include <memory>
#include <span>
#include <vector>

namespace mnx {

class Sketch {
public:
    explicit Sketch(SketchPlane plane = SketchPlane());

    [[nodiscard]] const SketchPlane& plane() const {
        return plane_;
    }

    void setPlane(const SketchPlane& plane) {
        plane_ = plane;
    }

    [[nodiscard]] SketchPoint& addPoint(const Point2& point);
    [[nodiscard]] SketchLine& addLine(const Point2& start, const Point2& end);
    [[nodiscard]] SketchCircle& addCircle(const Point2& center, double radius);
    [[nodiscard]] SketchArc& addArc(const Point2& center, double radius, double startAngle, double endAngle);
    [[nodiscard]] SketchSpline& addSpline(std::vector<Point2> controlPoints, int degree = 3);
    [[nodiscard]] Status insertEntity(std::unique_ptr<SketchEntity> entity);

    [[nodiscard]] Status removeEntity(EntityId id);
    [[nodiscard]] SketchEntity* findEntity(EntityId id);
    [[nodiscard]] const SketchEntity* findEntity(EntityId id) const;

    [[nodiscard]] std::vector<SketchEntity*> entities();
    [[nodiscard]] std::vector<const SketchEntity*> entities() const;
    [[nodiscard]] std::size_t entityCount() const {
        return entities_.size();
    }

    [[nodiscard]] AABB2 bounds() const;
    [[nodiscard]] Result<Point2> resolvePoint(const GeometryRef& ref) const;
    [[nodiscard]] bool supportsRef(const GeometryRef& ref) const;

    void addConstraintId(ConstraintId id);
    [[nodiscard]] bool removeConstraintId(ConstraintId id);
    [[nodiscard]] bool hasConstraintId(ConstraintId id) const;
    [[nodiscard]] std::span<const ConstraintId> constraintIds() const {
        return constraintIds_;
    }

private:
    [[nodiscard]] EntityId nextEntityId();
    SketchEntity& addEntity(std::unique_ptr<SketchEntity> entity);
    void advanceNextEntityIdPast(EntityId id);

    SketchPlane plane_;
    std::vector<std::unique_ptr<SketchEntity>> entities_;
    std::vector<ConstraintId> constraintIds_;
    EntityId::ValueType nextEntityId_ = 1;
};

} // namespace mnx
