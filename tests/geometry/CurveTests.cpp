#include "../core/TestSupport.h"

#include "mnx/geometry/BSpline2D.h"
#include "mnx/geometry/Bezier2D.h"
#include "mnx/geometry/algorithms/CurveSampling.h"

using namespace mnx;

TEST(CurveTests, CubicBezierEvaluatesTangentsAndBounds) {
    const Bezier2D curve({Point2(0.0, 0.0), Point2(1.0, 2.0), Point2(2.0, 2.0), Point2(3.0, 0.0)});

    EXPECT_TRUE(curve.evaluate(0.0).isApprox(Point2(0.0, 0.0)));
    EXPECT_TRUE(curve.evaluate(1.0).isApprox(Point2(3.0, 0.0)));
    EXPECT_TRUE(curve.tangent(0.0).isApprox(Vec2(3.0, 6.0)));
    EXPECT_TRUE(curve.bounds().contains(Vec2(1.5, 1.5)));
}

TEST(CurveTests, CurveSamplingIncludesEndpoints) {
    const Bezier2D curve({Point2(0.0, 0.0), Point2(1.0, 0.0), Point2(2.0, 0.0), Point2(3.0, 0.0)});
    const auto samples = sampleCurve(curve, 4);

    EXPECT_EQ(samples.size(), 4U);
    EXPECT_TRUE(samples.front().isApprox(Point2(0.0, 0.0)));
    EXPECT_TRUE(samples.back().isApprox(Point2(3.0, 0.0)));
}

TEST(CurveTests, BSplineScaffoldProvidesLinearControlPolygonEvaluation) {
    const BSpline2D curve({Point2(0.0, 0.0), Point2(10.0, 0.0), Point2(10.0, 10.0)}, 3);

    EXPECT_EQ(curve.degree(), 3);
    EXPECT_TRUE(curve.evaluate(0.25).isApprox(Point2(5.0, 0.0)));
    EXPECT_TRUE(curve.bounds().max().isApprox(Vec2(10.0, 10.0)));
}
