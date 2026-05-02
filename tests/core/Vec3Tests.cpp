#include "TestSupport.h"

#include "mnx/core/math/Vec3.h"

using namespace mnx;

TEST(Vec3Tests, ArithmeticAndProductsWork) {
    const Vec3 a(1.0, 2.0, 3.0);
    const Vec3 b(4.0, -5.0, 6.0);

    EXPECT_TRUE((a + b).isApprox(Vec3(5.0, -3.0, 9.0)));
    EXPECT_TRUE((a - b).isApprox(Vec3(-3.0, 7.0, -3.0)));
    EXPECT_TRUE((a * 3.0).isApprox(Vec3(3.0, 6.0, 9.0)));
    EXPECT_DOUBLE_EQ(a.dot(b), 12.0);
    EXPECT_TRUE(a.cross(b).isApprox(Vec3(27.0, 6.0, -13.0)));
}

TEST(Vec3Tests, NormAndNormalizationAreDeterministic) {
    const Vec3 vector(0.0, 3.0, 4.0);

    EXPECT_DOUBLE_EQ(vector.squaredNorm(), 25.0);
    EXPECT_DOUBLE_EQ(vector.norm(), 5.0);
    EXPECT_TRUE(vector.normalized().isApprox(Vec3(0.0, 0.6, 0.8)));
    EXPECT_TRUE(Vec3().normalized().isApprox(Vec3()));
}
