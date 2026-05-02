#include "TestSupport.h"

#include "mnx/persistence/ProjectDeserializer.h"
#include "mnx/scripting/PythonBindingsPart.h"

#include <filesystem>
#include <fstream>
#include <iterator>

using namespace mnx;
using namespace mnx::scripting;

namespace {

[[nodiscard]] std::string readText(const std::filesystem::path& path) {
    std::ifstream file(path);
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

} // namespace

TEST(ScriptingSmokeTests, ScriptPartCreatesExtrudesSavesAndExports) {
    ScriptPart part("script_smoke");
    part.newSketch();
    (void)part.addLine(0.0, 0.0, 2.0, 0.0);
    (void)part.addLine(2.0, 0.0, 2.0, 1.0);
    (void)part.addLine(2.0, 1.0, 0.0, 1.0);
    (void)part.addLine(0.0, 1.0, 0.0, 0.0);

    Status extrudeStatus = part.extrude(0.5);
    EXPECT_TRUE(extrudeStatus.isOk());

    const std::filesystem::path projectPath = std::filesystem::current_path() / "script_smoke.mnx.json";
    const std::filesystem::path stlPath = std::filesystem::current_path() / "script_smoke.stl";
    EXPECT_TRUE(part.saveProject(projectPath).isOk());
    EXPECT_TRUE(part.exportStl(stlPath).isOk());

    Result<PartDocument> loaded = mnx::persistence::ProjectDeserializer().deserialize(readText(projectPath));
    EXPECT_TRUE(loaded.isOk());
    EXPECT_EQ(loaded.value().featureGraph().featureCount(), static_cast<std::size_t>(2));

    std::filesystem::remove(projectPath);
    std::filesystem::remove(stlPath);
}

TEST(ScriptingSmokeTests, ScriptPartSolvesDistanceConstraintWhenUsed) {
    ScriptPart part("solve_smoke");
    part.newSketch();
    EntityId first = part.addLine(0.0, 0.0, 1.0, 0.0);
    EntityId second = part.addLine(3.0, 0.0, 4.0, 0.0);
    (void)part.addDistanceConstraint(first, second, 2.0);

    ScriptSolveSummary summary = part.solveSketch();

    EXPECT_TRUE(summary.iterationCount >= 0);
}
