#pragma once

#include "mnx/core/utils/Result.h"
#include "mnx/modeling/Feature.h"

#include <memory>
#include <optional>
#include <unordered_set>
#include <vector>

namespace mnx {

class FeatureGraph {
public:
    [[nodiscard]] Result<FeatureId> insertFeature(std::unique_ptr<Feature> feature);
    [[nodiscard]] Status addDependency(FeatureId featureId, FeatureId dependencyId);
    [[nodiscard]] Status markDirty(FeatureId id);
    void markClean(FeatureId id);
    void markAllDirty();
    void markAllClean();

    [[nodiscard]] Feature* findFeature(FeatureId id);
    [[nodiscard]] const Feature* findFeature(FeatureId id) const;
    [[nodiscard]] std::vector<Feature*> features();
    [[nodiscard]] std::vector<const Feature*> features() const;
    [[nodiscard]] std::vector<FeatureId> featureIds() const;
    [[nodiscard]] std::vector<FeatureId> dirtyFeatureIds() const;
    [[nodiscard]] std::vector<FeatureId> dependentsOf(FeatureId id) const;

    [[nodiscard]] bool isDirty(FeatureId id) const;
    [[nodiscard]] std::size_t featureCount() const;
    [[nodiscard]] std::optional<std::size_t> firstDirtyIndex() const;

private:
    [[nodiscard]] std::optional<std::size_t> indexOf(FeatureId id) const;
    void markDependentsDirty(FeatureId id);

    std::vector<std::unique_ptr<Feature>> features_;
    std::unordered_set<FeatureId> dirty_;
};

} // namespace mnx
