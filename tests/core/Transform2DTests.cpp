#include "TestSupport.h"

#include "mnx/core/math/Transform2D.h"

#include <cmath>
#include <stdexcept>

using namespace mnx;

TEST(Transform2DTests, CompositionAppliesRightHandTransformFirst) {
    const Transform2D transform =
        Transform2D::translation(Vec2(2.0, 0.0)) * Transform2D::rotation(std::acos(-1.0) / 2.0);

    EXPECT_TRUE(transform.applyToPoint(Vec2(1.0, 0.0)).isApprox(Vec2(2.0, 1.0), kLooseTolerance));
    EXPECT_TRUE(transform.applyToVector(Vec2(1.0, 0.0)).isApprox(Vec2(0.0, 1.0), kLooseTolerance));
}

TEST(Transform2DTests, InverseRoundTripsPoint) {
    const Transform2D transform =
        Transform2D::translation(Vec2(5.0, -2.0)) * Transform2D::scaling(2.0, 4.0);
    const Vec2 point(3.0, 7.0);

    EXPECT_TRUE(transform.inverse().applyToPoint(transform.applyToPoint(point)).isApprox(point));
}

TEST(Transform2DTests, SingularTransformThrowsOnInverse) {
    EXPECT_THROW(Transform2D::scaling(0.0, 1.0).inverse(), std::runtime_error);
}
