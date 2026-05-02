#include "TestSupport.h"

#include "mnx/core/math/Transform3D.h"

#include <cmath>
#include <stdexcept>

using namespace mnx;

TEST(Transform3DTests, CompositionTransformsPointAndVector) {
    const Transform3D transform =
        Transform3D::translation(Vec3(0.0, 0.0, 2.0)) * Transform3D::rotationZ(std::acos(-1.0) / 2.0);

    EXPECT_TRUE(transform.applyToPoint(Vec3(1.0, 0.0, 0.0)).isApprox(Vec3(0.0, 1.0, 2.0), kLooseTolerance));
    EXPECT_TRUE(transform.applyToVector(Vec3(1.0, 0.0, 0.0)).isApprox(Vec3(0.0, 1.0, 0.0), kLooseTolerance));
}

TEST(Transform3DTests, InverseRoundTripsPoint) {
    const Transform3D transform =
        Transform3D::translation(Vec3(2.0, -3.0, 4.0)) * Transform3D::scaling(2.0, 3.0, 4.0);
    const Vec3 point(5.0, 6.0, 7.0);

    EXPECT_TRUE(transform.inverse().applyToPoint(transform.applyToPoint(point)).isApprox(point));
}

TEST(Transform3DTests, SingularTransformThrowsOnInverse) {
    EXPECT_THROW(Transform3D::scaling(0.0, 1.0, 1.0).inverse(), std::runtime_error);
}
