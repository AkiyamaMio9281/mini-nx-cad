#include "../core/TestSupport.h"

#include "mnx/geometry/algorithms/Bounds2D.h"

#include <array>

using namespace mnx;

TEST(Bounds2DTests, ComputesBoundsForPointSpansAndShapes) {
    const std::array<Point2, 3> points {Point2(2.0, 3.0), Point2(-1.0, 4.0), Point2(5.0, -2.0)};
    const AABB2 pointBounds = boundsOf(points);
    const AABB2 circleBounds = boundsOf(Circle2D(Point2(1.0, 1.0), 2.0));
    const AABB2 segmentBounds = boundsOf(Segment2D(Point2(-2.0, 1.0), Point2(3.0, -4.0)));

    EXPECT_TRUE(pointBounds.min().isApprox(Vec2(-1.0, -2.0)));
    EXPECT_TRUE(pointBounds.max().isApprox(Vec2(5.0, 4.0)));
    EXPECT_TRUE(circleBounds.min().isApprox(Vec2(-1.0, -1.0)));
    EXPECT_TRUE(circleBounds.max().isApprox(Vec2(3.0, 3.0)));
    EXPECT_TRUE(segmentBounds.min().isApprox(Vec2(-2.0, -4.0)));
    EXPECT_TRUE(segmentBounds.max().isApprox(Vec2(3.0, 1.0)));
}
