#include "TestSupport.h"

#include "mnx/core/math/Vec2.h"

using namespace mnx;

TEST(Vec2Tests, ArithmeticAndProductsWork) {
    const Vec2 a(1.0, 2.0);
    const Vec2 b(3.0, -4.0);

    EXPECT_TRUE((a + b).isApprox(Vec2(4.0, -2.0)));
    EXPECT_TRUE((a - b).isApprox(Vec2(-2.0, 6.0)));
    EXPECT_TRUE((2.0 * a).isApprox(Vec2(2.0, 4.0)));
    EXPECT_TRUE((b / 2.0).isApprox(Vec2(1.5, -2.0)));
    EXPECT_DOUBLE_EQ(a.dot(b), -5.0);
    EXPECT_DOUBLE_EQ(a.cross(b), -10.0);
}

TEST(Vec2Tests, NormAndNormalizationAreDeterministic) {
    const Vec2 vector(3.0, 4.0);
    const Vec2 normalized = vector.normalized();

    EXPECT_DOUBLE_EQ(vector.squaredNorm(), 25.0);
    EXPECT_DOUBLE_EQ(vector.norm(), 5.0);
    EXPECT_TRUE(normalized.isApprox(Vec2(0.6, 0.8)));
    EXPECT_TRUE(Vec2().normalized().isApprox(Vec2()));
}
