#include "../core/TestSupport.h"

#include "mnx/geometry/Polyline2D.h"

#include <stdexcept>

using namespace mnx;

TEST(Polyline2DTests, LengthBoundsClosedAndEvaluationWork) {
    const Polyline2D polyline({Point2(0.0, 0.0), Point2(3.0, 4.0), Point2(0.0, 0.0)});

    EXPECT_TRUE(polyline.isClosed());
    EXPECT_DOUBLE_EQ(polyline.length(), 10.0);
    EXPECT_TRUE(polyline.bounds().max().isApprox(Vec2(3.0, 4.0)));
    EXPECT_TRUE(polyline.evaluate(0.5).isApprox(Point2(3.0, 4.0)));
}

TEST(Polyline2DTests, EmptyPolylineThrowsOnEvaluate) {
    EXPECT_THROW(Polyline2D().evaluate(0.0), std::runtime_error);
}
