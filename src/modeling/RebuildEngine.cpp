#include "mnx/modeling/RebuildEngine.h"

#include "mnx/modeling/Feature.h"

#include <utility>

namespace mnx {

RebuildResult RebuildEngine::rebuildAll(FeatureGraph& graph) const {
    graph.markAllDirty();
    return rebuildFromIndex(graph, 0);
}

RebuildResult RebuildEngine::rebuildDirty(FeatureGraph& graph) const {
    const std::optional<std::size_t> startIndex = graph.firstDirtyIndex();
    if(!startIndex.has_value()) {
        return {Status::ok(), {}, {}, std::nullopt};
    }
    return rebuildFromIndex(graph, *startIndex);
}

RebuildResult RebuildEngine::rebuildFromIndex(FeatureGraph& graph, std::size_t startIndex) const {
    RebuildResult rebuildResult {Status::ok(), {}, {}, std::nullopt};
    FeatureContext context;
    const std::vector<Feature*> features = graph.features();

    for(std::size_t index = 0; index < startIndex && index < features.size(); ++index) {
        Feature* feature = features[index];
        if(feature == nullptr) {
            continue;
        }
        FeatureResult featureResult = feature->execute(context);
        if(!featureResult.isOk()) {
            rebuildResult.status = featureResult.status();
            rebuildResult.failedFeatureId = feature->id();
            rebuildResult.diagnostics.push_back("Rebuild context setup failed before first dirty feature");
            return rebuildResult;
        }
    }

    for(std::size_t index = startIndex; index < features.size(); ++index) {
        Feature* feature = features[index];
        if(feature == nullptr) {
            continue;
        }

        FeatureResult featureResult = feature->execute(context);
        rebuildResult.rebuiltFeatureIds.push_back(feature->id());
        for(const std::string& diagnostic : featureResult.diagnostics()) {
            rebuildResult.diagnostics.push_back(diagnostic);
        }

        if(!featureResult.isOk()) {
            rebuildResult.status = featureResult.status();
            rebuildResult.failedFeatureId = feature->id();
            (void)graph.markDirty(feature->id());
            for(std::size_t dirtyIndex = index + 1; dirtyIndex < features.size(); ++dirtyIndex) {
                (void)graph.markDirty(features[dirtyIndex]->id());
            }
            rebuildResult.diagnostics.push_back("Rebuild stopped after feature failure");
            return rebuildResult;
        }

        graph.markClean(feature->id());
    }

    for(const std::string& diagnostic : context.diagnostics()) {
        rebuildResult.diagnostics.push_back(diagnostic);
    }
    return rebuildResult;
}

} // namespace mnx
