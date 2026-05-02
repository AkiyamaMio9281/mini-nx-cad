#include "../core/TestSupport.h"

#include "mnx/geometry/Point2.h"
#include "mnx/geometry/Point3.h"

using namespace mnx;

TEST(PointTests, PointsAndVectorsHaveDistinctOperations) {
    const Point2 a(1.0, 2.0);
    const Point2 b(4.0, 6.0);
    const Vec2 delta = b - a;

    EXPECT_TRUE(delta.isApprox(Vec2(3.0, 4.0)));
    EXPECT_TRUE((a + delta).isApprox(b));
    EXPECT_TRUE((b - delta).isApprox(a));
}

TEST(PointTests, Point3VectorOperationsWork) {
    const Point3 a(1.0, 2.0, 3.0);
    const Point3 b(2.0, 4.0, 6.0);

    EXPECT_TRUE((b - a).isApprox(Vec3(1.0, 2.0, 3.0)));
    EXPECT_TRUE((a + Vec3(1.0, 2.0, 3.0)).isApprox(b));
}
