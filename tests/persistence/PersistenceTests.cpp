#include "TestSupport.h"

#include "mnx/constraints/DistanceConstraint.h"
#include "mnx/constraints/HorizontalConstraint.h"
#include "mnx/modeling/ExtrudeFeature.h"
#include "mnx/modeling/PartDocument.h"
#include "mnx/modeling/RebuildEngine.h"
#include "mnx/modeling/SketchFeature.h"
#include "mnx/persistence/PersistenceUtils.h"
#include "mnx/persistence/ProjectDeserializer.h"
#include "mnx/persistence/ProjectSerializer.h"
#include "mnx/sketch/Sketch.h"

#include <filesystem>
#include <memory>

using namespace mnx;
using namespace mnx::persistence;

namespace {

[[nodiscard]] PartDocument makeFeatureDocument() {
    PartDocument document("RoundTripPart");

    Sketch sketch;
    SketchLine& bottom = sketch.addLine(Point2(0.0, 0.0), Point2(4.0, 0.0));
    SketchLine& right = sketch.addLine(Point2(4.0, 0.0), Point2(4.0, 2.0));
    (void)sketch.addLine(Point2(4.0, 2.0), Point2(0.0, 2.0));
    (void)sketch.addLine(Point2(0.0, 2.0), Point2(0.0, 0.0));
    sketch.addConstraintId(ConstraintId(1));
    sketch.addConstraintId(ConstraintId(2));

    auto sketchFeature = std::make_unique<SketchFeature>(FeatureId(1), "Profile Sketch", std::move(sketch));
    EXPECT_TRUE(document.featureGraph().insertFeature(std::move(sketchFeature)).isOk());

    auto extrude = std::make_unique<ExtrudeFeature>(FeatureId(2), "Pad", FeatureId(1), 3.0);
    EXPECT_TRUE(document.featureGraph().insertFeature(std::move(extrude)).isOk());

    document.addConstraint(std::make_unique<HorizontalConstraint>(
        ConstraintId(1),
        GeometryRef::whole(bottom.id())
    ));
    document.addConstraint(std::make_unique<DistanceConstraint>(
        ConstraintId(2),
        GeometryRef::startPoint(bottom.id()),
        GeometryRef::endPoint(right.id()),
        4.0
    ));
    return document;
}

} // namespace

TEST(PersistenceTests, RoundTripsPartWithSketchConstraintsAndFeatureHistory) {
    PartDocument document = makeFeatureDocument();

    const ProjectSerializer serializer;
    const std::string json = serializer.serialize(document);
    const ProjectDeserializer deserializer;
    Result<PartDocument> loaded = deserializer.deserialize(json);

    EXPECT_TRUE(loaded.isOk());
    EXPECT_EQ(loaded.value().name(), std::string("RoundTripPart"));
    EXPECT_EQ(loaded.value().featureGraph().featureCount(), static_cast<std::size_t>(2));
    EXPECT_EQ(loaded.value().constraints().size(), static_cast<std::size_t>(2));

    RebuildEngine engine;
    RebuildResult rebuild = engine.rebuildAll(loaded.value().featureGraph());
    EXPECT_TRUE(rebuild.isOk());

    const auto* extrude = dynamic_cast<const ExtrudeFeature*>(loaded.value().featureGraph().findFeature(FeatureId(2)));
    EXPECT_TRUE(extrude != nullptr);
    EXPECT_TRUE(extrude->body().has_value());
    EXPECT_TRUE(extrude->displayMesh().has_value());
    EXPECT_TRUE(extrude->displayMesh()->vertices().size() > 0U);
}

TEST(PersistenceTests, RejectsUnsupportedSchemaVersion) {
    const std::string json = R"({"schemaVersion":999,"document":{"name":"Bad","features":[]}})";

    Result<PartDocument> loaded = ProjectDeserializer().deserialize(json);

    EXPECT_FALSE(loaded.isOk());
}

TEST(PersistenceTests, RejectsMissingRequiredFields) {
    const std::string json = R"({"schemaVersion":1,"document":{"name":"Bad"}})";

    Result<PartDocument> loaded = ProjectDeserializer().deserialize(json);

    EXPECT_FALSE(loaded.isOk());
}

TEST(PersistenceTests, RejectsInvalidConstraintReferenceShape) {
    const std::string json = R"({
        "schemaVersion": 1,
        "document": {
            "name": "Bad Constraint",
            "features": [],
            "constraints": [
                {"id": 1, "type": 0, "driving": true, "label": "", "refs": [{"entityId": 1, "kind": 0}]}
            ]
        }
    })";

    Result<PartDocument> loaded = ProjectDeserializer().deserialize(json);

    EXPECT_FALSE(loaded.isOk());
}

TEST(PersistenceTests, PersistenceUtilsWritesAndReadsProjectFile) {
    PartDocument document = makeFeatureDocument();
    const std::filesystem::path path = std::filesystem::current_path() / "persistence_roundtrip.mnx.json";

    Status saveStatus = PersistenceUtils::saveProject(document, path);
    EXPECT_TRUE(saveStatus.isOk());

    Result<PartDocument> loaded = PersistenceUtils::loadProject(path);
    EXPECT_TRUE(loaded.isOk());
    EXPECT_EQ(loaded.value().featureGraph().featureCount(), static_cast<std::size_t>(2));
    EXPECT_EQ(loaded.value().constraints().size(), static_cast<std::size_t>(2));

    std::filesystem::remove(path);
}
