#include "../core/TestSupport.h"

#include "mnx/sketch/Sketch.h"

#include <cmath>

using namespace mnx;

TEST(SketchTests, AddQueryIterateAndRemoveEntities) {
    Sketch sketch;
    const SketchPoint& point = sketch.addPoint(Point2(1.0, 2.0));
    const SketchLine& line = sketch.addLine(Point2(0.0, 0.0), Point2(3.0, 0.0));
    const EntityId pointId = point.id();
    const EntityId lineId = line.id();

    EXPECT_TRUE(pointId.isValid());
    EXPECT_TRUE(lineId.isValid());
    EXPECT_NE(pointId, lineId);
    EXPECT_EQ(sketch.entityCount(), 2U);
    EXPECT_EQ(sketch.findEntity(pointId)->type(), SketchEntityType::Point);
    EXPECT_EQ(sketch.entities().size(), 2U);

    EXPECT_TRUE(sketch.removeEntity(pointId).isOk());
    EXPECT_EQ(sketch.entityCount(), 1U);
    EXPECT_TRUE(sketch.findEntity(pointId) == nullptr);
    EXPECT_TRUE(sketch.findEntity(lineId) != nullptr);
    EXPECT_FALSE(sketch.removeEntity(pointId).isOk());
}

TEST(SketchTests, BoundsUnionAllEntities) {
    Sketch sketch;
    (void)sketch.addLine(Point2(-1.0, 2.0), Point2(3.0, 4.0));
    (void)sketch.addCircle(Point2(10.0, 0.0), 2.0);
    (void)sketch.addArc(Point2(0.0, 0.0), 1.0, 0.0, std::acos(-1.0) / 2.0);

    const AABB2 bounds = sketch.bounds();
    EXPECT_TRUE(bounds.isValid());
    EXPECT_TRUE(bounds.min().isApprox(Vec2(-1.0, -2.0), kLooseTolerance));
    EXPECT_TRUE(bounds.max().isApprox(Vec2(12.0, 4.0), kLooseTolerance));
}

TEST(SketchTests, ConstraintIdsAreAssociatedDeterministically) {
    Sketch sketch;
    sketch.addConstraintId(ConstraintId(10));
    sketch.addConstraintId(ConstraintId(10));
    sketch.addConstraintId(ConstraintId(11));

    EXPECT_EQ(sketch.constraintIds().size(), 2U);
    EXPECT_TRUE(sketch.hasConstraintId(ConstraintId(10)));
    EXPECT_TRUE(sketch.removeConstraintId(ConstraintId(10)));
    EXPECT_FALSE(sketch.hasConstraintId(ConstraintId(10)));
    EXPECT_FALSE(sketch.removeConstraintId(ConstraintId(10)));
}
