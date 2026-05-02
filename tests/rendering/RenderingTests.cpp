#include "TestSupport.h"

#include "mnx/modeling/Body.h"
#include "mnx/rendering/BodyRenderer.h"
#include "mnx/rendering/Camera.h"
#include "mnx/rendering/Mesh.h"
#include "mnx/rendering/Picking.h"
#include "mnx/rendering/RenderScene.h"

#include <optional>
#include <utility>

using namespace mnx;
using namespace mnx::rendering;

namespace {

[[nodiscard]] DisplayMesh makeBoxLikeMesh() {
    DisplayMesh mesh;
    (void)mesh.addVertex(Point3(0.0, 0.0, 0.0));
    (void)mesh.addVertex(Point3(1.0, 0.0, 0.0));
    (void)mesh.addVertex(Point3(1.0, 1.0, 0.0));
    (void)mesh.addVertex(Point3(0.0, 1.0, 0.0));
    (void)mesh.addVertex(Point3(0.0, 0.0, 1.0));
    (void)mesh.addVertex(Point3(1.0, 0.0, 1.0));
    (void)mesh.addVertex(Point3(1.0, 1.0, 1.0));
    (void)mesh.addVertex(Point3(0.0, 1.0, 1.0));
    mesh.addTriangle(0, 1, 2);
    mesh.addTriangle(0, 2, 3);
    mesh.addTriangle(4, 6, 5);
    mesh.addTriangle(4, 7, 6);
    return mesh;
}

} // namespace

TEST(RenderingTests, CameraOrbitPanAndZoomAreDeterministic) {
    Camera camera;
    const Vec3 initialPosition = camera.position();

    camera.orbit(0.5, 0.25);
    EXPECT_FALSE(camera.position().isApprox(initialPosition));
    camera.pan(2.0, -1.0);
    EXPECT_FALSE(camera.target().isApprox(Vec3()));

    const double distance = camera.distance();
    camera.zoom(0.5);
    EXPECT_NEAR(camera.distance(), distance * 0.5, 1.0e-12);

    const Ray ray = camera.screenRay(0.0, 0.0, 1.0);
    EXPECT_NEAR(ray.direction.norm(), 1.0, 1.0e-12);
}

TEST(RenderingTests, MeshBoundsComeFromVertices) {
    const Mesh mesh = Mesh::fromDisplayMesh(makeBoxLikeMesh());
    const AABB3 bounds = mesh.bounds();

    EXPECT_TRUE(bounds.isValid());
    EXPECT_NEAR(bounds.size().x(), 1.0, 1.0e-12);
    EXPECT_NEAR(bounds.size().y(), 1.0, 1.0e-12);
    EXPECT_NEAR(bounds.size().z(), 1.0, 1.0e-12);
    EXPECT_EQ(mesh.triangleCount(), static_cast<std::size_t>(4));
}

TEST(RenderingTests, BodyRendererPopulatesSceneWithoutOwningDomainBody) {
    Body body(BodyId(7), FeatureId(3), makeBoxLikeMesh(), 1.0);
    RenderScene scene;

    BodyRenderer().populateScene(scene, body, "Box");

    EXPECT_EQ(scene.items().size(), static_cast<std::size_t>(1));
    EXPECT_EQ(scene.items()[0].bodyId(), BodyId(7));
    EXPECT_EQ(scene.items()[0].mesh().vertexCount(), body.displayMesh().vertexCount());
    EXPECT_TRUE(scene.bounds().isValid());
}

TEST(RenderingTests, PickingSelectsNearestBodyByBounds) {
    RenderScene scene;
    Body body(BodyId(1), FeatureId(1), makeBoxLikeMesh(), 1.0);
    BodyRenderer().populateScene(scene, body, "Near");

    RenderItem farItem = BodyRenderer().makeRenderItem(Body(BodyId(2), FeatureId(2), makeBoxLikeMesh(), 1.0), "Far");
    farItem.setTransform(Transform3D::translation(Vec3(0.0, 0.0, 5.0)));
    (void)scene.addItem(std::move(farItem));

    const Ray ray {Vec3(0.5, 0.5, -2.0), Vec3(0.0, 0.0, 1.0)};
    const std::optional<PickResult> pick = Picking::pickBody(scene, ray);

    EXPECT_TRUE(pick.has_value());
    EXPECT_EQ(pick->bodyId, BodyId(1));
    EXPECT_TRUE(scene.selectBody(pick->bodyId));
    EXPECT_TRUE(scene.selectedBodyId().has_value());
    EXPECT_EQ(*scene.selectedBodyId(), BodyId(1));
}
