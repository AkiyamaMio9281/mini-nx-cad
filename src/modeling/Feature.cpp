#include "mnx/modeling/Feature.h"

#include <algorithm>
#include <utility>

namespace mnx {

Feature::Feature(FeatureId id, FeatureType type, std::string name)
    : id_(id), type_(type), name_(std::move(name)) {}

FeatureId Feature::id() const {
    return id_;
}

FeatureType Feature::type() const {
    return type_;
}

const std::string& Feature::name() const {
    return name_;
}

void Feature::setName(std::string name) {
    name_ = std::move(name);
}

std::span<const FeatureId> Feature::dependencies() const {
    return dependencies_;
}

bool Feature::dependsOn(FeatureId id) const {
    return std::find(dependencies_.begin(), dependencies_.end(), id) != dependencies_.end();
}

Status Feature::addDependency(FeatureId id) {
    if(!id.isValid()) {
        return Status::invalidArgument("Feature dependency id is invalid");
    }
    if(id == id_) {
        return Status::invalidArgument("Feature cannot depend on itself");
    }
    if(!dependsOn(id)) {
        dependencies_.push_back(id);
    }
    return Status::ok();
}

void Feature::clearDependencies() {
    dependencies_.clear();
}

} // namespace mnx
