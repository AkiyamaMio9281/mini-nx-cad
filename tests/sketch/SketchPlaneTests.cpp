#include "../core/TestSupport.h"

#include "mnx/sketch/SketchPlane.h"

#include <stdexcept>

using namespace mnx;

TEST(SketchPlaneTests, MapsBetweenSketchAndWorldCoordinates) {
    const SketchPlane plane(Point3(1.0, 2.0, 3.0), Vec3(1.0, 0.0, 0.0), Vec3(0.0, 1.0, 0.0));

    const Point3 world = plane.toWorld(Point2(4.0, 5.0));
    EXPECT_TRUE(world.isApprox(Point3(5.0, 7.0, 3.0)));
    EXPECT_TRUE(plane.toSketch(world).isApprox(Point2(4.0, 5.0)));
}

TEST(SketchPlaneTests, BuildsFromGeometryPlane) {
    const SketchPlane plane(Plane(Point3(0.0, 0.0, 2.0), Vec3(0.0, 0.0, 1.0)));

    EXPECT_TRUE(plane.toWorld(Point2(0.0, 0.0)).isApprox(Point3(0.0, 0.0, 2.0)));
    EXPECT_TRUE(isZero(plane.uDirection().dot(plane.normal()), kLooseTolerance));
    EXPECT_TRUE(isZero(plane.vDirection().dot(plane.normal()), kLooseTolerance));
}

TEST(SketchPlaneTests, RejectsDegenerateDirections) {
    EXPECT_THROW(SketchPlane(Point3(), Vec3(), Vec3(0.0, 1.0, 0.0)), std::invalid_argument);
    EXPECT_THROW(SketchPlane(Point3(), Vec3(1.0, 0.0, 0.0), Vec3(2.0, 0.0, 0.0)), std::invalid_argument);
}
