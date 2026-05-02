#include "../core/TestSupport.h"

#include "mnx/geometry/Segment2D.h"

using namespace mnx;

TEST(Segment2DTests, LengthMidpointBoundsAndProjectionWork) {
    const Segment2D segment(Point2(0.0, 0.0), Point2(3.0, 4.0));

    EXPECT_DOUBLE_EQ(segment.length(), 5.0);
    EXPECT_TRUE(segment.midpoint().isApprox(Point2(1.5, 2.0)));
    EXPECT_TRUE(segment.bounds().min().isApprox(Vec2(0.0, 0.0)));
    EXPECT_TRUE(segment.bounds().max().isApprox(Vec2(3.0, 4.0)));
    EXPECT_TRUE(segment.projection(Point2(3.0, 0.0)).isApprox(Point2(1.08, 1.44), kLooseTolerance));
}

TEST(Segment2DTests, ProjectionClampsToSegment) {
    const Segment2D segment(Point2(0.0, 0.0), Point2(1.0, 0.0));

    EXPECT_TRUE(segment.projection(Point2(2.0, 3.0)).isApprox(Point2(1.0, 0.0)));
    EXPECT_TRUE(segment.projection(Point2(-2.0, 3.0)).isApprox(Point2(0.0, 0.0)));
}
