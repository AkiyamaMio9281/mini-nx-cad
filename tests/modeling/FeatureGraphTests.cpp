#include "TestSupport.h"

#include "mnx/modeling/ExtrudeFeature.h"
#include "mnx/modeling/FeatureGraph.h"
#include "mnx/modeling/RebuildEngine.h"
#include "mnx/modeling/SketchFeature.h"

#include <algorithm>
#include <memory>

using namespace mnx;

namespace {

[[nodiscard]] bool contains(const std::vector<FeatureId>& ids, FeatureId id) {
    return std::find(ids.begin(), ids.end(), id) != ids.end();
}

[[nodiscard]] Sketch makeSimpleSketch() {
    Sketch sketch;
    (void)sketch.addLine(Point2(0.0, 0.0), Point2(1.0, 0.0));
    return sketch;
}

} // namespace

TEST(FeatureGraphTests, PreservesFeatureInsertionOrder) {
    FeatureGraph graph;

    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(1), "Sketch 1", makeSimpleSketch())));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(2), "Sketch 2", makeSimpleSketch())));

    const std::vector<FeatureId> ids = graph.featureIds();
    EXPECT_EQ(ids.size(), static_cast<std::size_t>(2));
    EXPECT_EQ(ids[0], FeatureId(1));
    EXPECT_EQ(ids[1], FeatureId(2));
}

TEST(FeatureGraphTests, TracksDependencies) {
    FeatureGraph graph;
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(1), "Sketch", makeSimpleSketch())));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(2), "Derived Sketch", makeSimpleSketch())));

    EXPECT_TRUE(graph.addDependency(FeatureId(2), FeatureId(1)));

    const Feature* feature = graph.findFeature(FeatureId(2));
    EXPECT_TRUE(feature != nullptr);
    EXPECT_TRUE(feature->dependsOn(FeatureId(1)));
    EXPECT_TRUE(contains(graph.dependentsOf(FeatureId(1)), FeatureId(2)));
}

TEST(FeatureGraphTests, MarksDependentsDirty) {
    FeatureGraph graph;
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(1), "Base Sketch", makeSimpleSketch())));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(2), "Dependent Sketch", makeSimpleSketch())));
    EXPECT_TRUE(graph.addDependency(FeatureId(2), FeatureId(1)));
    graph.markAllClean();

    EXPECT_TRUE(graph.markDirty(FeatureId(1)));

    EXPECT_TRUE(graph.isDirty(FeatureId(1)));
    EXPECT_TRUE(graph.isDirty(FeatureId(2)));
}

TEST(FeatureGraphTests, RebuildEngineExecutesSketchFeaturesAndCleansDirtyState) {
    FeatureGraph graph;
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(1), "Sketch", makeSimpleSketch())));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(2), "Sketch 2", makeSimpleSketch())));

    const RebuildEngine engine;
    const RebuildResult result = engine.rebuildDirty(graph);

    EXPECT_TRUE(result.isOk());
    EXPECT_EQ(result.rebuiltFeatureIds.size(), static_cast<std::size_t>(2));
    EXPECT_FALSE(graph.isDirty(FeatureId(1)));
    EXPECT_FALSE(graph.isDirty(FeatureId(2)));
}

TEST(FeatureGraphTests, RebuildDirtyStartsAtFirstDirtyFeature) {
    FeatureGraph graph;
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(1), "Sketch 1", makeSimpleSketch())));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(2), "Sketch 2", makeSimpleSketch())));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(3), "Sketch 3", makeSimpleSketch())));
    graph.markAllClean();
    EXPECT_TRUE(graph.markDirty(FeatureId(2)));

    const RebuildEngine engine;
    const RebuildResult result = engine.rebuildDirty(graph);

    EXPECT_TRUE(result.isOk());
    EXPECT_EQ(result.rebuiltFeatureIds.size(), static_cast<std::size_t>(2));
    EXPECT_EQ(result.rebuiltFeatureIds[0], FeatureId(2));
    EXPECT_EQ(result.rebuiltFeatureIds[1], FeatureId(3));
}

TEST(FeatureGraphTests, RebuildStopsAndPropagatesFailure) {
    FeatureGraph graph;
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(1), "Sketch", makeSimpleSketch())));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<ExtrudeFeature>(FeatureId(2), "Invalid Extrude", FeatureId(1), 2.0)));
    EXPECT_TRUE(graph.insertFeature(std::make_unique<SketchFeature>(FeatureId(3), "After Failure", makeSimpleSketch())));

    const RebuildEngine engine;
    const RebuildResult result = engine.rebuildAll(graph);

    EXPECT_FALSE(result.isOk());
    EXPECT_TRUE(result.failedFeatureId.has_value());
    EXPECT_EQ(*result.failedFeatureId, FeatureId(2));
    EXPECT_TRUE(graph.isDirty(FeatureId(2)));
    EXPECT_TRUE(graph.isDirty(FeatureId(3)));
}
