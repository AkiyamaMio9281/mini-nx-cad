#include "../core/TestSupport.h"

#include "mnx/geometry/algorithms/Intersections2D.h"

using namespace mnx;

TEST(IntersectionTests, LineLineReturnsPointOrRelationship) {
    const auto crossing = intersect(Line2D(Point2(), Vec2(1.0, 0.0)), Line2D(Point2(1.0, -1.0), Vec2(0.0, 1.0)));
    const auto parallel = intersect(Line2D(Point2(), Vec2(1.0, 0.0)), Line2D(Point2(0.0, 1.0), Vec2(1.0, 0.0)));
    const auto coincident = intersect(Line2D(Point2(), Vec2(1.0, 0.0)), Line2D(Point2(1.0, 0.0), Vec2(2.0, 0.0)));

    EXPECT_EQ(crossing.type, IntersectionType::Point);
    EXPECT_TRUE(crossing.points.front().isApprox(Point2(1.0, 0.0)));
    EXPECT_EQ(parallel.type, IntersectionType::Parallel);
    EXPECT_EQ(coincident.type, IntersectionType::Coincident);
}

TEST(IntersectionTests, SegmentSegmentHandlesCrossingAndOverlap) {
    const auto crossing =
        intersect(Segment2D(Point2(0.0, 0.0), Point2(2.0, 2.0)), Segment2D(Point2(0.0, 2.0), Point2(2.0, 0.0)));
    const auto overlap =
        intersect(Segment2D(Point2(0.0, 0.0), Point2(3.0, 0.0)), Segment2D(Point2(1.0, 0.0), Point2(2.0, 0.0)));

    EXPECT_EQ(crossing.type, IntersectionType::Point);
    EXPECT_TRUE(crossing.points.front().isApprox(Point2(1.0, 1.0)));
    EXPECT_EQ(overlap.type, IntersectionType::Overlap);
    EXPECT_EQ(overlap.points.size(), 2U);
}

TEST(IntersectionTests, LineCircleAndSegmentCircleReturnStructuredResults) {
    const Circle2D circle(Point2(), 1.0);
    const auto secant = intersect(Line2D(Point2(0.0, 0.0), Vec2(1.0, 0.0)), circle);
    const auto tangent = intersect(Line2D(Point2(0.0, 1.0), Vec2(1.0, 0.0)), circle);
    const auto segment = intersect(Segment2D(Point2(-2.0, 0.0), Point2(0.0, 0.0)), circle);

    EXPECT_EQ(secant.type, IntersectionType::Point);
    EXPECT_EQ(secant.points.size(), 2U);
    EXPECT_EQ(tangent.type, IntersectionType::Tangent);
    EXPECT_EQ(segment.type, IntersectionType::Point);
    EXPECT_EQ(segment.points.size(), 1U);
}

TEST(IntersectionTests, CircleCircleHandlesTwoPointTangentAndCoincidentCases) {
    const auto twoPoints = intersect(Circle2D(Point2(0.0, 0.0), 2.0), Circle2D(Point2(3.0, 0.0), 2.0));
    const auto tangent = intersect(Circle2D(Point2(0.0, 0.0), 1.0), Circle2D(Point2(2.0, 0.0), 1.0));
    const auto coincident = intersect(Circle2D(Point2(0.0, 0.0), 1.0), Circle2D(Point2(0.0, 0.0), 1.0));

    EXPECT_EQ(twoPoints.type, IntersectionType::Point);
    EXPECT_EQ(twoPoints.points.size(), 2U);
    EXPECT_EQ(tangent.type, IntersectionType::Tangent);
    EXPECT_TRUE(tangent.points.front().isApprox(Point2(1.0, 0.0)));
    EXPECT_EQ(coincident.type, IntersectionType::Coincident);
}
