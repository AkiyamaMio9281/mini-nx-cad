#pragma once

#include "mnx/core/ids/FeatureId.h"
#include "mnx/core/utils/Status.h"
#include "mnx/modeling/FeatureContext.h"
#include "mnx/modeling/FeatureGraph.h"

#include <optional>
#include <string>
#include <vector>

namespace mnx {

struct RebuildResult {
    Status status;
    std::vector<FeatureId> rebuiltFeatureIds;
    std::vector<std::string> diagnostics;
    std::optional<FeatureId> failedFeatureId;

    [[nodiscard]] bool isOk() const {
        return status.isOk();
    }
};

class RebuildEngine {
public:
    [[nodiscard]] RebuildResult rebuildAll(FeatureGraph& graph) const;
    [[nodiscard]] RebuildResult rebuildDirty(FeatureGraph& graph) const;

private:
    [[nodiscard]] RebuildResult rebuildFromIndex(FeatureGraph& graph, std::size_t startIndex) const;
};

} // namespace mnx
