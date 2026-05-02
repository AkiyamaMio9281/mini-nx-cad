#include "TestSupport.h"

#include "mnx/modeling/ExtrudeFeature.h"
#include "mnx/modeling/FeatureGraph.h"
#include "mnx/modeling/RebuildEngine.h"
#include "mnx/modeling/SketchFeature.h"
#include "mnx/sketch/SketchLine.h"

#include <memory>

using namespace mnx;

namespace {

[[nodiscard]] Sketch makeRectangleSketch(double width = 2.0, double height = 1.0) {
    Sketch sketch;
    (void)sketch.addLine(Point2(0.0, 0.0), Point2(width, 0.0));
    (void)sketch.addLine(Point2(width, 0.0), Point2(width, height));
    (void)sketch.addLine(Point2(width, height), Point2(0.0, height));
    (void)sketch.addLine(Point2(0.0, height), Point2(0.0, 0.0));
    return sketch;
}

[[nodiscard]] Sketch makeCircleSketch(double radius = 1.0) {
    Sketch sketch;
    (void)sketch.addCircle(Point2(0.0, 0.0), radius);
    return sketch;
}

[[nodiscard]] Sketch makeOpenSketch() {
    Sketch sketch;
    (void)sketch.addLine(Point2(0.0, 0.0), Point2(1.0, 0.0));
    (void)sketch.addLine(Point2(1.0, 0.0), Point2(1.0, 1.0));
    return sketch;
}

[[nodiscard]] Sketch makeConcaveSketch() {
    Sketch sketch;
    (void)sketch.addLine(Point2(0.0, 0.0), Point2(2.0, 0.0));
    (void)sketch.addLine(Point2(2.0, 0.0), Point2(2.0, 2.0));
    (void)sketch.addLine(Point2(2.0, 2.0), Point2(1.0, 1.0));
    (void)sketch.addLine(Point2(1.0, 1.0), Point2(0.0, 2.0));
    (void)sketch.addLine(Point2(0.0, 2.0), Point2(0.0, 0.0));
    return sketch;
}

[[nodiscard]] ExtrudeFeature* findExtrude(FeatureGraph& graph, FeatureId id) {
    return dynamic_cast<ExtrudeFeature*>(graph.findFeature(id));
}

} // namespace

TEST(ExtrudeFeatureTests, RectangleProfileExtrudesSuccessfully) {
    FeatureGraph graph;
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(1), "Rectangle Sketch", makeRectangleSketch())));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<ExtrudeFeature>(FeatureId(2), "Extrude", FeatureId(1), 3.0)));

    const RebuildResult result = RebuildEngine().rebuildAll(graph);
    const ExtrudeFeature* extrude = findExtrude(graph, FeatureId(2));

    EXPECT_TRUE(result.isOk());
    EXPECT_TRUE(extrude != nullptr);
    EXPECT_TRUE(extrude->body().has_value());
    EXPECT_EQ(extrude->displayMesh()->vertexCount(), static_cast<std::size_t>(8));
    EXPECT_EQ(extrude->displayMesh()->triangleCount(), static_cast<std::size_t>(12));
    EXPECT_NEAR(extrude->body()->volume(), 6.0, 1.0e-9);
    EXPECT_NEAR(extrude->body()->bounds().size().z(), 3.0, 1.0e-9);
}

TEST(ExtrudeFeatureTests, CircleProfileExtrudesSuccessfully) {
    FeatureGraph graph;
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(1), "Circle Sketch", makeCircleSketch(2.0))));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<ExtrudeFeature>(FeatureId(2), "Cylinder", FeatureId(1), 5.0)));

    const RebuildResult result = RebuildEngine().rebuildAll(graph);
    const ExtrudeFeature* extrude = findExtrude(graph, FeatureId(2));

    EXPECT_TRUE(result.isOk());
    EXPECT_TRUE(extrude != nullptr);
    EXPECT_TRUE(extrude->body().has_value());
    EXPECT_EQ(extrude->displayMesh()->vertexCount(), static_cast<std::size_t>(64));
    EXPECT_EQ(extrude->displayMesh()->triangleCount(), static_cast<std::size_t>(124));
    EXPECT_NEAR(extrude->body()->volume(), 20.0 * 3.14159265358979323846, 1.0e-9);
}

TEST(ExtrudeFeatureTests, OpenProfileIsRejected) {
    FeatureGraph graph;
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(1), "Open Sketch", makeOpenSketch())));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<ExtrudeFeature>(FeatureId(2), "Extrude", FeatureId(1), 1.0)));

    const RebuildResult result = RebuildEngine().rebuildAll(graph);

    EXPECT_FALSE(result.isOk());
    EXPECT_TRUE(result.failedFeatureId.has_value());
    EXPECT_EQ(*result.failedFeatureId, FeatureId(2));
    EXPECT_TRUE(graph.isDirty(FeatureId(2)));
}

TEST(ExtrudeFeatureTests, ConcaveProfileIsRejectedUntilRobustTriangulationExists) {
    FeatureGraph graph;
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(1), "Concave Sketch", makeConcaveSketch())));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<ExtrudeFeature>(FeatureId(2), "Extrude", FeatureId(1), 1.0)));

    const RebuildResult result = RebuildEngine().rebuildAll(graph);

    EXPECT_FALSE(result.isOk());
    EXPECT_TRUE(result.failedFeatureId.has_value());
    EXPECT_EQ(*result.failedFeatureId, FeatureId(2));
}

TEST(ExtrudeFeatureTests, NonPositiveDistanceIsRejected) {
    FeatureGraph graph;
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(1), "Rectangle Sketch", makeRectangleSketch())));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<ExtrudeFeature>(FeatureId(2), "Extrude", FeatureId(1), -1.0)));

    const RebuildResult result = RebuildEngine().rebuildAll(graph);

    EXPECT_FALSE(result.isOk());
    EXPECT_TRUE(result.failedFeatureId.has_value());
    EXPECT_EQ(*result.failedFeatureId, FeatureId(2));
}

TEST(ExtrudeFeatureTests, InvalidSketchDependencyIsRejectedWithFailure) {
    FeatureGraph graph;
    EXPECT_TRUE(graph.insertFeature(std::make_unique<ExtrudeFeature>(FeatureId(2), "Extrude", FeatureId::invalid(), 1.0)));

    const RebuildResult result = RebuildEngine().rebuildAll(graph);

    EXPECT_FALSE(result.isOk());
    EXPECT_TRUE(result.failedFeatureId.has_value());
    EXPECT_EQ(*result.failedFeatureId, FeatureId(2));
}

TEST(ExtrudeFeatureTests, DistanceParameterChangeTriggersRebuildAndChangesBody) {
    FeatureGraph graph;
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(1), "Rectangle Sketch", makeRectangleSketch())));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<ExtrudeFeature>(FeatureId(2), "Extrude", FeatureId(1), 1.0)));

    EXPECT_TRUE(RebuildEngine().rebuildAll(graph).isOk());
    ExtrudeFeature* extrude = findExtrude(graph, FeatureId(2));
    EXPECT_TRUE(extrude != nullptr);
    EXPECT_NEAR(extrude->body()->volume(), 2.0, 1.0e-9);

    extrude->setDistance(4.0);
    EXPECT_TRUE(graph.markDirty(FeatureId(2)));
    const RebuildResult result = RebuildEngine().rebuildDirty(graph);

    EXPECT_TRUE(result.isOk());
    EXPECT_EQ(result.rebuiltFeatureIds.size(), static_cast<std::size_t>(1));
    EXPECT_EQ(result.rebuiltFeatureIds[0], FeatureId(2));
    EXPECT_NEAR(extrude->body()->volume(), 8.0, 1.0e-9);
    EXPECT_NEAR(extrude->body()->bounds().size().z(), 4.0, 1.0e-9);
}

TEST(ExtrudeFeatureTests, SketchDimensionUpdateRebuildsExtrudeResult) {
    FeatureGraph graph;
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(1), "Rectangle Sketch", makeRectangleSketch())));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<ExtrudeFeature>(FeatureId(2), "Extrude", FeatureId(1), 3.0)));

    EXPECT_TRUE(RebuildEngine().rebuildAll(graph).isOk());
    auto* sketchFeature = dynamic_cast<SketchFeature*>(graph.findFeature(FeatureId(1)));
    ExtrudeFeature* extrude = findExtrude(graph, FeatureId(2));
    EXPECT_TRUE(sketchFeature != nullptr);
    EXPECT_TRUE(extrude != nullptr);
    EXPECT_NEAR(extrude->body()->volume(), 6.0, 1.0e-9);

    Sketch& sketch = sketchFeature->sketch();
    auto* bottom = dynamic_cast<SketchLine*>(sketch.findEntity(EntityId(1)));
    auto* right = dynamic_cast<SketchLine*>(sketch.findEntity(EntityId(2)));
    auto* top = dynamic_cast<SketchLine*>(sketch.findEntity(EntityId(3)));
    EXPECT_TRUE(bottom != nullptr);
    EXPECT_TRUE(right != nullptr);
    EXPECT_TRUE(top != nullptr);
    bottom->setEnd(Point2(3.0, 0.0));
    right->setStart(Point2(3.0, 0.0));
    right->setEnd(Point2(3.0, 1.0));
    top->setStart(Point2(3.0, 1.0));

    EXPECT_TRUE(graph.markDirty(FeatureId(1)));
    const RebuildResult result = RebuildEngine().rebuildDirty(graph);

    EXPECT_TRUE(result.isOk());
    EXPECT_EQ(result.rebuiltFeatureIds.size(), static_cast<std::size_t>(2));
    EXPECT_NEAR(extrude->body()->volume(), 9.0, 1.0e-9);
    EXPECT_NEAR(extrude->body()->bounds().size().x(), 3.0, 1.0e-9);
}
