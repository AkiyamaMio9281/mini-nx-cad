#include "../core/TestSupport.h"

#include "mnx/geometry/Arc2D.h"

#include <cmath>

using namespace mnx;

TEST(Arc2DTests, EvaluatesCounterClockwiseArc) {
    const Arc2D arc(Point2(0.0, 0.0), 2.0, 0.0, std::acos(-1.0) / 2.0);

    EXPECT_TRUE(arc.startPoint().isApprox(Point2(2.0, 0.0)));
    EXPECT_TRUE(arc.endPoint().isApprox(Point2(0.0, 2.0), kLooseTolerance));
    EXPECT_TRUE(arc.evaluate(0.5).isApprox(Point2(std::sqrt(2.0), std::sqrt(2.0)), kLooseTolerance));
    EXPECT_TRUE(arc.tangent(0.0).isApprox(Vec2(0.0, 1.0), kLooseTolerance));
}

TEST(Arc2DTests, BoundsIncludeInteriorCardinalAngles) {
    const Arc2D arc(Point2(0.0, 0.0), 2.0, 0.0, std::acos(-1.0));

    EXPECT_TRUE(arc.bounds().min().isApprox(Vec2(-2.0, 0.0), kLooseTolerance));
    EXPECT_TRUE(arc.bounds().max().isApprox(Vec2(2.0, 2.0), kLooseTolerance));
}
