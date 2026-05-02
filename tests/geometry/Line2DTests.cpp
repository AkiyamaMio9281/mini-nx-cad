#include "../core/TestSupport.h"

#include "mnx/geometry/Line2D.h"

#include <stdexcept>

using namespace mnx;

TEST(Line2DTests, ProjectsAndMeasuresDistance) {
    const Line2D line(Point2(0.0, 0.0), Vec2(2.0, 0.0));

    EXPECT_TRUE(line.direction().isApprox(Vec2(1.0, 0.0)));
    EXPECT_TRUE(line.projection(Point2(3.0, 4.0)).isApprox(Point2(3.0, 0.0)));
    EXPECT_DOUBLE_EQ(line.distanceTo(Point2(3.0, 4.0)), 4.0);
}

TEST(Line2DTests, RejectsZeroDirection) {
    EXPECT_THROW(Line2D(Point2(), Vec2()), std::invalid_argument);
}
