#include "TestSupport.h"

#include "mnx/modeling/RebuildEngine.h"
#include "mnx/persistence/JsonConverters.h"
#include "mnx/persistence/ProjectDeserializer.h"

#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

using namespace mnx;
using namespace mnx::persistence;

namespace {

std::string readText(const std::filesystem::path& path) {
    std::ifstream stream(path);
    EXPECT_TRUE(stream.is_open());
    return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
}

std::filesystem::path repositoryRoot() {
#if defined(MNX_REPOSITORY_ROOT)
    const std::filesystem::path configuredRoot = MNX_REPOSITORY_ROOT;
    if(std::filesystem::exists(configuredRoot / "samples")) {
        return configuredRoot;
    }
#endif

    std::filesystem::path path = std::filesystem::current_path();
    while(!path.empty()) {
        if(std::filesystem::exists(path / "samples") && std::filesystem::exists(path / "tests")) {
            return path;
        }
        const std::filesystem::path parent = path.parent_path();
        if(parent == path) {
            break;
        }
        path = parent;
    }

    return std::filesystem::current_path();
}

std::filesystem::path repoPath(std::initializer_list<const char*> parts) {
    std::filesystem::path path = repositoryRoot();
    for(const char* part : parts) {
        path /= part;
    }
    return path;
}

} // namespace

TEST(SampleAssetTests, PartSamplesLoadAndRebuild) {
    const std::vector<std::filesystem::path> samples {
        repoPath({"samples", "parts", "rectangle_extrude.mnx.json"}),
        repoPath({"samples", "parts", "revolve_pin.mnx.json"}),
    };

    for(const std::filesystem::path& sample : samples) {
        Result<PartDocument> document = ProjectDeserializer().deserialize(readText(sample));
        EXPECT_TRUE(document.isOk());
        EXPECT_TRUE(document.value().featureGraph().featureCount() > 0);

        RebuildResult rebuild = RebuildEngine().rebuildAll(document.value().featureGraph());
        EXPECT_TRUE(rebuild.isOk());
        EXPECT_TRUE(!rebuild.rebuiltFeatureIds.empty());
    }
}

TEST(SampleAssetTests, AssemblySampleLoads) {
    Result<AssemblyDocument> assembly = ProjectDeserializer().deserializeAssembly(readText(repoPath(
        {"samples", "assemblies", "two_box_assembly.mnx_assembly.json"}
    )));

    EXPECT_TRUE(assembly.isOk());
    EXPECT_EQ(assembly.value().components().size(), static_cast<std::size_t>(2));
    EXPECT_EQ(assembly.value().mates().size(), static_cast<std::size_t>(1));
}

TEST(SampleAssetTests, RegressionCaseMetadataParses) {
    const std::vector<std::filesystem::path> cases {
        repoPath({"tests", "integration", "regression_cases", "assembly_distance_mate", "case.json"}),
        repoPath({"tests", "integration", "regression_cases", "extrude_rectangle", "case.json"}),
        repoPath({"tests", "integration", "regression_cases", "open_profile_rejected", "case.json"}),
        repoPath({"tests", "integration", "regression_cases", "persistence_invalid_schema", "case.json"}),
        repoPath({"tests", "integration", "regression_cases", "revolve_axis_clearance", "case.json"}),
        repoPath({"tests", "integration", "regression_cases", "sketch_solver_rectangle", "case.json"}),
        repoPath({"tests", "integration", "regression_cases", "topology_broken_loop", "case.json"}),
    };

    for(const std::filesystem::path& regressionCase : cases) {
        Result<JsonValue> parsed = JsonValue::parse(readText(regressionCase));
        EXPECT_TRUE(parsed.isOk());
        EXPECT_TRUE(parsed.value().isObject());
    }
}
