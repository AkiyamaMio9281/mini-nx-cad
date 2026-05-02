#include "../core/TestSupport.h"

#include "mnx/geometry/Circle2D.h"

#include <cmath>
#include <stdexcept>

using namespace mnx;

TEST(Circle2DTests, EvaluatesAndComputesBounds) {
    const Circle2D circle(Point2(1.0, 2.0), 3.0);

    EXPECT_TRUE(circle.evaluate(0.0).isApprox(Point2(4.0, 2.0)));
    EXPECT_TRUE(circle.evaluate(std::acos(-1.0) / 2.0).isApprox(Point2(1.0, 5.0), kLooseTolerance));
    EXPECT_TRUE(circle.bounds().min().isApprox(Vec2(-2.0, -1.0)));
    EXPECT_TRUE(circle.bounds().max().isApprox(Vec2(4.0, 5.0)));
}

TEST(Circle2DTests, RejectsNegativeRadius) {
    EXPECT_THROW(Circle2D(Point2(), -1.0), std::invalid_argument);
}
