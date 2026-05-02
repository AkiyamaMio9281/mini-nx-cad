#include "../core/TestSupport.h"

#include "mnx/sketch/SketchArc.h"
#include "mnx/sketch/SketchCircle.h"
#include "mnx/sketch/SketchLine.h"
#include "mnx/sketch/SketchPoint.h"
#include "mnx/sketch/SketchSpline.h"

#include <cmath>
#include <stdexcept>

using namespace mnx;

TEST(SketchEntityTests, EntitiesExposeStableIdsTypesAndBounds) {
    const SketchPoint point(EntityId(1), Point2(2.0, 3.0));
    const SketchLine line(EntityId(2), Point2(0.0, 0.0), Point2(4.0, 2.0));
    const SketchCircle circle(EntityId(3), Point2(1.0, 1.0), 2.0);
    const SketchArc arc(EntityId(4), Point2(0.0, 0.0), 2.0, 0.0, std::acos(-1.0) / 2.0);
    const SketchSpline spline(EntityId(5), {Point2(0.0, 0.0), Point2(2.0, 3.0), Point2(4.0, 0.0)});

    EXPECT_EQ(point.id(), EntityId(1));
    EXPECT_EQ(line.type(), SketchEntityType::Line);
    EXPECT_TRUE(point.bounds().contains(Vec2(2.0, 3.0)));
    EXPECT_TRUE(line.bounds().max().isApprox(Vec2(4.0, 2.0)));
    EXPECT_TRUE(circle.bounds().min().isApprox(Vec2(-1.0, -1.0)));
    EXPECT_TRUE(arc.bounds().contains(Vec2(0.0, 2.0), kLooseTolerance));
    EXPECT_TRUE(spline.bounds().max().isApprox(Vec2(4.0, 3.0)));
}

TEST(SketchEntityTests, TransformsAffectBoundsAndResolvedPoints) {
    SketchLine line(EntityId(2), Point2(0.0, 0.0), Point2(1.0, 0.0));
    line.setTransform(Transform2D::translation(Vec2(2.0, 3.0)));

    EXPECT_TRUE(line.bounds().min().isApprox(Vec2(2.0, 3.0)));
    EXPECT_TRUE(line.resolvePoint(GeometryRefKind::EndPoint).value().isApprox(Point2(3.0, 3.0)));
}

TEST(SketchEntityTests, InvalidEntityParametersThrow) {
    EXPECT_THROW(SketchCircle(EntityId(1), Point2(), -1.0), std::invalid_argument);
    EXPECT_THROW(SketchArc(EntityId(2), Point2(), -1.0, 0.0, 1.0), std::invalid_argument);
    EXPECT_THROW(SketchSpline(EntityId(3), {}, 0), std::invalid_argument);
}
