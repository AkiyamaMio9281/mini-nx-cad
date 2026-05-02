#include "TestSupport.h"

#include "mnx/core/math/Mat3.h"
#include "mnx/core/math/Mat4.h"

using namespace mnx;

TEST(MatrixTests, Mat3ComposesAndTransformsPoints) {
    const Mat3 transform = Mat3::translation(Vec2(2.0, 3.0)) * Mat3::scaling(2.0, 4.0);

    EXPECT_TRUE(transform.transformPoint(Vec2(1.0, 1.0)).isApprox(Vec2(4.0, 7.0)));
    EXPECT_TRUE(transform.transformVector(Vec2(1.0, 1.0)).isApprox(Vec2(2.0, 4.0)));
}

TEST(MatrixTests, Mat4ComposesAndTransformsPoints) {
    const Mat4 transform = Mat4::translation(Vec3(1.0, 2.0, 3.0)) * Mat4::scaling(2.0, 3.0, 4.0);

    EXPECT_TRUE(transform.transformPoint(Vec3(1.0, 1.0, 1.0)).isApprox(Vec3(3.0, 5.0, 7.0)));
    EXPECT_TRUE(transform.transformVector(Vec3(1.0, 1.0, 1.0)).isApprox(Vec3(2.0, 3.0, 4.0)));
}
