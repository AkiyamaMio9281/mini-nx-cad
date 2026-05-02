#include "../core/TestSupport.h"

#include "mnx/geometry/algorithms/Distances2D.h"
#include "mnx/geometry/algorithms/Projections2D.h"

using namespace mnx;

TEST(ProjectionTests, ProjectPointOntoLineSegmentAndCircle) {
    const Line2D line(Point2(0.0, 0.0), Vec2(1.0, 0.0));
    const Segment2D segment(Point2(0.0, 0.0), Point2(1.0, 0.0));
    const Circle2D circle(Point2(0.0, 0.0), 2.0);

    EXPECT_TRUE(projectPoint(Point2(3.0, 4.0), line).isApprox(Point2(3.0, 0.0)));
    EXPECT_TRUE(projectPoint(Point2(3.0, 4.0), segment).isApprox(Point2(1.0, 0.0)));
    EXPECT_TRUE(projectPoint(Point2(0.0, 3.0), circle).isApprox(Point2(0.0, 2.0)));
}

TEST(ProjectionTests, DistancesUseProjectionRules) {
    EXPECT_DOUBLE_EQ(distance(Point2(0.0, 0.0), Point2(3.0, 4.0)), 5.0);
    EXPECT_DOUBLE_EQ(distance(Point2(0.0, 3.0), Line2D(Point2(), Vec2(1.0, 0.0))), 3.0);
    EXPECT_DOUBLE_EQ(distance(Point2(3.0, 4.0), Segment2D(Point2(), Point2(1.0, 0.0))), 4.4721359549995796);
    EXPECT_DOUBLE_EQ(distance(Point2(3.0, 0.0), Circle2D(Point2(), 2.0)), 1.0);
}
