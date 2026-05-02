#include "TestSupport.h"

#include "mnx/assembly/AssemblyDocument.h"
#include "mnx/assembly/AssemblyQueries.h"
#include "mnx/assembly/AssemblySolver.h"
#include "mnx/modeling/ExtrudeFeature.h"
#include "mnx/modeling/PartDocument.h"
#include "mnx/modeling/RebuildEngine.h"
#include "mnx/modeling/SketchFeature.h"
#include "mnx/persistence/ProjectDeserializer.h"
#include "mnx/persistence/ProjectSerializer.h"

#include <memory>
#include <string>

using namespace mnx;
using namespace mnx::persistence;

namespace {

[[nodiscard]] std::shared_ptr<const PartDocument> makeBoxPart(const std::string& name) {
    auto document = std::make_shared<PartDocument>(name);
    Sketch sketch;
    (void)sketch.addLine(Point2(0.0, 0.0), Point2(1.0, 0.0));
    (void)sketch.addLine(Point2(1.0, 0.0), Point2(1.0, 1.0));
    (void)sketch.addLine(Point2(1.0, 1.0), Point2(0.0, 1.0));
    (void)sketch.addLine(Point2(0.0, 1.0), Point2(0.0, 0.0));

    auto sketchFeature = std::make_unique<SketchFeature>(FeatureId(1), "Sketch", std::move(sketch));
    EXPECT_TRUE(document->featureGraph().insertFeature(std::move(sketchFeature)).isOk());
    auto extrude = std::make_unique<ExtrudeFeature>(FeatureId(2), "Box", FeatureId(1), 1.0);
    EXPECT_TRUE(document->featureGraph().insertFeature(std::move(extrude)).isOk());
    EXPECT_TRUE(RebuildEngine().rebuildAll(document->featureGraph()).isOk());
    return document;
}

[[nodiscard]] Vec3 originOf(const AssemblyDocument& assembly, ComponentId id) {
    Result<Transform3D> transform = assembly.worldTransform(id);
    EXPECT_TRUE(transform.isOk());
    return transform.value().applyToPoint(Vec3());
}

} // namespace

TEST(AssemblyTests, InsertsMultipleComponentInstances) {
    AssemblyDocument assembly("Asm");
    auto partA = makeBoxPart("A");
    auto partB = makeBoxPart("B");

    Result<ComponentId> first = assembly.insertComponent(partA, "A:1");
    Result<ComponentId> second = assembly.insertComponent(partB, "B:1", Transform3D::translation(Vec3(2.0, 0.0, 0.0)));

    EXPECT_TRUE(first.isOk());
    EXPECT_TRUE(second.isOk());
    EXPECT_EQ(assembly.components().size(), static_cast<std::size_t>(2));
    EXPECT_EQ(AssemblyQueries::rootComponents(assembly).size(), static_cast<std::size_t>(2));
    EXPECT_TRUE(AssemblyQueries::bounds(assembly).isValid());
}

TEST(AssemblyTests, SupportsMultipleInstancesOfSamePart) {
    AssemblyDocument assembly("Asm");
    auto sharedPart = makeBoxPart("Shared");

    Result<ComponentId> first = assembly.insertComponent(sharedPart, "Shared:1");
    Result<ComponentId> second = assembly.insertComponent(sharedPart, "Shared:2", Transform3D::translation(Vec3(5.0, 0.0, 0.0)));

    EXPECT_TRUE(first.isOk());
    EXPECT_TRUE(second.isOk());
    EXPECT_EQ(assembly.findComponent(first.value())->partDocument().get(), assembly.findComponent(second.value())->partDocument().get());
}

TEST(AssemblyTests, FixedMateSetsWorldTransform) {
    AssemblyDocument assembly("Asm");
    auto part = makeBoxPart("Part");
    ComponentId id = assembly.insertComponent(part, "Part:1", Transform3D::translation(Vec3(9.0, 0.0, 0.0))).value();
    EXPECT_TRUE(assembly.addMate(MateConstraint::fixed(ConstraintId(1), id, Transform3D::translation(Vec3(2.0, 3.0, 4.0)))).isOk());

    AssemblySolveResult result = AssemblySolver().solve(assembly);

    EXPECT_TRUE(result.isOk());
    EXPECT_TRUE(originOf(assembly, id).isApprox(Vec3(2.0, 3.0, 4.0)));
}

TEST(AssemblyTests, CoincidentAndDistanceMatesMoveSecondComponent) {
    AssemblyDocument coincidentAssembly("Coincident");
    auto part = makeBoxPart("Part");
    ComponentId fixed = coincidentAssembly.insertComponent(part, "Fixed").value();
    ComponentId moving =
        coincidentAssembly.insertComponent(part, "Moving", Transform3D::translation(Vec3(5.0, 0.0, 0.0))).value();
    EXPECT_TRUE(coincidentAssembly.addMate(MateConstraint::coincident(
                    ConstraintId(1),
                    fixed,
                    Vec3(),
                    moving,
                    Vec3()
                ))
                    .isOk());
    EXPECT_TRUE(AssemblySolver().solve(coincidentAssembly).isOk());
    EXPECT_TRUE(originOf(coincidentAssembly, moving).isApprox(Vec3()));

    AssemblyDocument distanceAssembly("Distance");
    ComponentId first = distanceAssembly.insertComponent(part, "First").value();
    ComponentId second =
        distanceAssembly.insertComponent(part, "Second", Transform3D::translation(Vec3(10.0, 0.0, 0.0))).value();
    EXPECT_TRUE(distanceAssembly.addMate(MateConstraint::distance(
                    ConstraintId(2),
                    first,
                    Vec3(),
                    second,
                    Vec3(),
                    3.0
                ))
                    .isOk());
    EXPECT_TRUE(AssemblySolver().solve(distanceAssembly).isOk());
    EXPECT_TRUE(originOf(distanceAssembly, second).isApprox(Vec3(3.0, 0.0, 0.0)));
}

TEST(AssemblyTests, ParallelMateAlignsProjectedDirections) {
    AssemblyDocument assembly("Asm");
    auto part = makeBoxPart("Part");
    ComponentId first = assembly.insertComponent(part, "First").value();
    ComponentId second = assembly.insertComponent(part, "Second", Transform3D::rotationZ(1.5707963267948966)).value();
    EXPECT_TRUE(assembly.addMate(MateConstraint::parallel(
                    ConstraintId(1),
                    first,
                    Vec3(1.0, 0.0, 0.0),
                    second,
                    Vec3(1.0, 0.0, 0.0)
                ))
                    .isOk());

    EXPECT_TRUE(AssemblySolver().solve(assembly).isOk());
    Result<Transform3D> secondWorld = assembly.worldTransform(second);
    EXPECT_TRUE(secondWorld.isOk());
    EXPECT_TRUE(secondWorld.value().applyToVector(Vec3(1.0, 0.0, 0.0)).normalized().isApprox(Vec3(1.0, 0.0, 0.0)));
}

TEST(AssemblyTests, AssemblyPersistenceRoundTripsComponentsAndMates) {
    AssemblyDocument assembly("PersistedAssembly");
    auto part = makeBoxPart("Part");
    ComponentId first = assembly.insertComponent(part, "Part:1").value();
    ComponentId second = assembly.insertComponent(part, "Part:2", Transform3D::translation(Vec3(4.0, 0.0, 0.0))).value();
    EXPECT_TRUE(assembly.addMate(MateConstraint::distance(ConstraintId(1), first, Vec3(), second, Vec3(), 2.5)).isOk());

    const std::string json = ProjectSerializer().serializeAssembly(assembly);
    Result<AssemblyDocument> loaded = ProjectDeserializer().deserializeAssembly(json);

    EXPECT_TRUE(loaded.isOk());
    EXPECT_EQ(loaded.value().name(), std::string("PersistedAssembly"));
    EXPECT_EQ(loaded.value().components().size(), static_cast<std::size_t>(2));
    EXPECT_EQ(loaded.value().mates().size(), static_cast<std::size_t>(1));
    EXPECT_TRUE(AssemblySolver().solve(loaded.value()).isOk());
    EXPECT_TRUE(originOf(loaded.value(), second).isApprox(Vec3(2.5, 0.0, 0.0)));
}
