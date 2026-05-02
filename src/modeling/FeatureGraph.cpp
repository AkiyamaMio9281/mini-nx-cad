#include "mnx/modeling/FeatureGraph.h"

#include <algorithm>
#include <utility>

namespace mnx {

Result<FeatureId> FeatureGraph::insertFeature(std::unique_ptr<Feature> feature) {
    if(!feature || !feature->id().isValid()) {
        return Result<FeatureId>::failure(Status::invalidArgument("Cannot insert an invalid feature"));
    }
    if(findFeature(feature->id()) != nullptr) {
        return Result<FeatureId>::failure(Status::failedPrecondition("Feature id already exists"));
    }
    for(FeatureId dependencyId : feature->dependencies()) {
        if(findFeature(dependencyId) == nullptr) {
            return Result<FeatureId>::failure(Status::failedPrecondition("Feature dependency is missing"));
        }
    }

    const FeatureId id = feature->id();
    features_.push_back(std::move(feature));
    dirty_.insert(id);
    return id;
}

Status FeatureGraph::addDependency(FeatureId featureId, FeatureId dependencyId) {
    Feature* feature = findFeature(featureId);
    if(feature == nullptr) {
        return Status::notFound("Dependent feature was not found");
    }
    if(findFeature(dependencyId) == nullptr) {
        return Status::notFound("Dependency feature was not found");
    }

    const auto featureIndex = indexOf(featureId);
    const auto dependencyIndex = indexOf(dependencyId);
    if(featureIndex.has_value() && dependencyIndex.has_value() && *dependencyIndex >= *featureIndex) {
        return Status::failedPrecondition("Feature dependencies must refer to earlier features");
    }

    Status status = feature->addDependency(dependencyId);
    if(!status) {
        return status;
    }
    return markDirty(featureId);
}

Status FeatureGraph::markDirty(FeatureId id) {
    if(findFeature(id) == nullptr) {
        return Status::notFound("Feature was not found");
    }
    dirty_.insert(id);
    markDependentsDirty(id);
    return Status::ok();
}

void FeatureGraph::markClean(FeatureId id) {
    dirty_.erase(id);
}

void FeatureGraph::markAllDirty() {
    for(const auto& feature : features_) {
        dirty_.insert(feature->id());
    }
}

void FeatureGraph::markAllClean() {
    dirty_.clear();
}

Feature* FeatureGraph::findFeature(FeatureId id) {
    const auto iterator = std::find_if(features_.begin(), features_.end(), [id](const auto& feature) {
        return feature->id() == id;
    });
    return iterator == features_.end() ? nullptr : iterator->get();
}

const Feature* FeatureGraph::findFeature(FeatureId id) const {
    const auto iterator = std::find_if(features_.begin(), features_.end(), [id](const auto& feature) {
        return feature->id() == id;
    });
    return iterator == features_.end() ? nullptr : iterator->get();
}

std::vector<Feature*> FeatureGraph::features() {
    std::vector<Feature*> result;
    result.reserve(features_.size());
    for(auto& feature : features_) {
        result.push_back(feature.get());
    }
    return result;
}

std::vector<const Feature*> FeatureGraph::features() const {
    std::vector<const Feature*> result;
    result.reserve(features_.size());
    for(const auto& feature : features_) {
        result.push_back(feature.get());
    }
    return result;
}

std::vector<FeatureId> FeatureGraph::featureIds() const {
    std::vector<FeatureId> result;
    result.reserve(features_.size());
    for(const auto& feature : features_) {
        result.push_back(feature->id());
    }
    return result;
}

std::vector<FeatureId> FeatureGraph::dirtyFeatureIds() const {
    std::vector<FeatureId> result;
    for(const auto& feature : features_) {
        if(isDirty(feature->id())) {
            result.push_back(feature->id());
        }
    }
    return result;
}

std::vector<FeatureId> FeatureGraph::dependentsOf(FeatureId id) const {
    std::vector<FeatureId> result;
    for(const auto& feature : features_) {
        if(feature->dependsOn(id)) {
            result.push_back(feature->id());
        }
    }
    return result;
}

bool FeatureGraph::isDirty(FeatureId id) const {
    return dirty_.find(id) != dirty_.end();
}

std::size_t FeatureGraph::featureCount() const {
    return features_.size();
}

std::optional<std::size_t> FeatureGraph::firstDirtyIndex() const {
    for(std::size_t index = 0; index < features_.size(); ++index) {
        if(isDirty(features_[index]->id())) {
            return index;
        }
    }
    return std::nullopt;
}

std::optional<std::size_t> FeatureGraph::indexOf(FeatureId id) const {
    for(std::size_t index = 0; index < features_.size(); ++index) {
        if(features_[index]->id() == id) {
            return index;
        }
    }
    return std::nullopt;
}

void FeatureGraph::markDependentsDirty(FeatureId id) {
    for(FeatureId dependentId : dependentsOf(id)) {
        if(dirty_.insert(dependentId).second) {
            markDependentsDirty(dependentId);
        }
    }
}

} // namespace mnx
