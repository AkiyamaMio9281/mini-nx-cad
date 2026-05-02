#include "../core/TestSupport.h"

#include "mnx/geometry/PlanarSurface.h"

#include <stdexcept>

using namespace mnx;

TEST(PlaneSurfaceTests, PlaneDistancesProjectionAndBasisWork) {
    const Plane plane(Point3(0.0, 0.0, 1.0), Vec3(0.0, 0.0, 2.0));

    EXPECT_DOUBLE_EQ(plane.signedDistance(Point3(0.0, 0.0, 4.0)), 3.0);
    EXPECT_TRUE(plane.projection(Point3(1.0, 2.0, 4.0)).isApprox(Point3(1.0, 2.0, 1.0)));
    EXPECT_TRUE(isZero(plane.basisU().dot(plane.normal()), kLooseTolerance));
    EXPECT_TRUE(isZero(plane.basisV().dot(plane.normal()), kLooseTolerance));
}

TEST(PlaneSurfaceTests, PlanarSurfaceEvaluatesParameters) {
    const PlanarSurface surface(Point3(1.0, 2.0, 3.0), Vec3(2.0, 0.0, 0.0), Vec3(0.0, 3.0, 0.0));

    EXPECT_TRUE(surface.evaluate(2.0, 3.0).isApprox(Point3(5.0, 11.0, 3.0)));
    EXPECT_TRUE(surface.normal(0.0, 0.0).isApprox(Vec3(0.0, 0.0, 1.0)));
    EXPECT_FALSE(surface.bounds().isValid());
}

TEST(PlaneSurfaceTests, DegeneratePlanesAndSurfacesThrow) {
    EXPECT_THROW(Plane(Point3(), Vec3()), std::invalid_argument);
    EXPECT_THROW(PlanarSurface(Point3(), Vec3(1.0, 0.0, 0.0), Vec3(2.0, 0.0, 0.0)), std::invalid_argument);
}
