#include "TestSupport.h"

#include "mnx/modeling/FeatureGraph.h"
#include "mnx/modeling/RebuildEngine.h"
#include "mnx/modeling/RevolveFeature.h"
#include "mnx/modeling/SketchFeature.h"

#include <memory>
#include <numbers>

using namespace mnx;

namespace {

[[nodiscard]] Sketch makeRevolveRectangleSketch() {
    Sketch sketch;
    (void)sketch.addLine(Point2(1.0, 0.0), Point2(2.0, 0.0));
    (void)sketch.addLine(Point2(2.0, 0.0), Point2(2.0, 1.0));
    (void)sketch.addLine(Point2(2.0, 1.0), Point2(1.0, 1.0));
    (void)sketch.addLine(Point2(1.0, 1.0), Point2(1.0, 0.0));
    return sketch;
}

[[nodiscard]] Sketch makeOpenSketch() {
    Sketch sketch;
    (void)sketch.addLine(Point2(1.0, 0.0), Point2(2.0, 0.0));
    (void)sketch.addLine(Point2(2.0, 0.0), Point2(2.0, 1.0));
    return sketch;
}

[[nodiscard]] RevolveAxis2D verticalAxis() {
    return RevolveAxis2D(Point2(0.0, 0.0), Vec2(0.0, 1.0));
}

[[nodiscard]] RevolveFeature* findRevolve(FeatureGraph& graph, FeatureId id) {
    return dynamic_cast<RevolveFeature*>(graph.findFeature(id));
}

} // namespace

TEST(RevolveFeatureTests, SimpleProfileRevolvesSuccessfully) {
    FeatureGraph graph;
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(1), "Profile", makeRevolveRectangleSketch())));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<RevolveFeature>(
        FeatureId(2),
        "Revolve",
        FeatureId(1),
        verticalAxis(),
        2.0 * std::numbers::pi
    )));

    const RebuildResult result = RebuildEngine().rebuildAll(graph);
    const RevolveFeature* revolve = findRevolve(graph, FeatureId(2));

    EXPECT_TRUE(result.isOk());
    EXPECT_TRUE(revolve != nullptr);
    EXPECT_TRUE(revolve->body().has_value());
    EXPECT_EQ(revolve->displayMesh()->vertexCount(), static_cast<std::size_t>(132));
    EXPECT_EQ(revolve->displayMesh()->triangleCount(), static_cast<std::size_t>(256));
    EXPECT_NEAR(revolve->body()->bounds().size().x(), 1.0, 1.0e-9);
    EXPECT_NEAR(revolve->body()->bounds().size().y(), 4.0, 1.0e-9);
    EXPECT_NEAR(revolve->body()->bounds().size().z(), 4.0, 1.0e-9);
}

TEST(RevolveFeatureTests, InvalidProfileIsRejected) {
    FeatureGraph graph;
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(1), "Open Profile", makeOpenSketch())));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<RevolveFeature>(
        FeatureId(2),
        "Revolve",
        FeatureId(1),
        verticalAxis(),
        std::numbers::pi
    )));

    const RebuildResult result = RebuildEngine().rebuildAll(graph);

    EXPECT_FALSE(result.isOk());
    EXPECT_TRUE(result.failedFeatureId.has_value());
    EXPECT_EQ(*result.failedFeatureId, FeatureId(2));
}

TEST(RevolveFeatureTests, InvalidAxisIsRejected) {
    FeatureGraph graph;
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(1), "Profile", makeRevolveRectangleSketch())));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<RevolveFeature>(
        FeatureId(2),
        "Revolve",
        FeatureId(1),
        RevolveAxis2D(Point2(0.0, 0.0), Vec2(0.0, 0.0)),
        std::numbers::pi
    )));

    const RebuildResult result = RebuildEngine().rebuildAll(graph);

    EXPECT_FALSE(result.isOk());
    EXPECT_TRUE(result.failedFeatureId.has_value());
    EXPECT_EQ(*result.failedFeatureId, FeatureId(2));
}

TEST(RevolveFeatureTests, AxisCrossingProfileIsRejected) {
    FeatureGraph graph;
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(1), "Profile", makeRevolveRectangleSketch())));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<RevolveFeature>(
        FeatureId(2),
        "Revolve",
        FeatureId(1),
        RevolveAxis2D(Point2(1.5, 0.0), Vec2(0.0, 1.0)),
        std::numbers::pi
    )));

    const RebuildResult result = RebuildEngine().rebuildAll(graph);

    EXPECT_FALSE(result.isOk());
    EXPECT_TRUE(result.failedFeatureId.has_value());
    EXPECT_EQ(*result.failedFeatureId, FeatureId(2));
}

TEST(RevolveFeatureTests, AngleChangeTriggersRebuildAndChangesMesh) {
    FeatureGraph graph;
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(1), "Profile", makeRevolveRectangleSketch())));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<RevolveFeature>(
        FeatureId(2),
        "Revolve",
        FeatureId(1),
        verticalAxis(),
        2.0 * std::numbers::pi
    )));

    EXPECT_TRUE(RebuildEngine().rebuildAll(graph).isOk());
    RevolveFeature* revolve = findRevolve(graph, FeatureId(2));
    EXPECT_TRUE(revolve != nullptr);
    EXPECT_EQ(revolve->displayMesh()->triangleCount(), static_cast<std::size_t>(256));
    const double fullTurnZSize = revolve->body()->bounds().size().z();

    revolve->setAngleRadians(std::numbers::pi);
    EXPECT_TRUE(graph.markDirty(FeatureId(2)));
    const RebuildResult result = RebuildEngine().rebuildDirty(graph);

    EXPECT_TRUE(result.isOk());
    EXPECT_EQ(result.rebuiltFeatureIds.size(), static_cast<std::size_t>(1));
    EXPECT_EQ(result.rebuiltFeatureIds[0], FeatureId(2));
    EXPECT_EQ(revolve->displayMesh()->triangleCount(), static_cast<std::size_t>(260));
    EXPECT_TRUE(revolve->body()->bounds().size().z() < fullTurnZSize);
}
