#include "mnx/modeling/PartDocument.h"

#include <utility>

namespace mnx {

PartDocument::PartDocument(std::string name) : name_(std::move(name)) {}

const std::string& PartDocument::name() const {
    return name_;
}

void PartDocument::setName(std::string name) {
    name_ = std::move(name);
}

FeatureGraph& PartDocument::featureGraph() {
    return featureGraph_;
}

const FeatureGraph& PartDocument::featureGraph() const {
    return featureGraph_;
}

void PartDocument::addConstraint(std::unique_ptr<Constraint> constraint) {
    if(constraint) {
        constraints_.push_back(std::move(constraint));
    }
}

void PartDocument::clearConstraints() {
    constraints_.clear();
}

std::span<const std::unique_ptr<Constraint>> PartDocument::constraints() const {
    return constraints_;
}

} // namespace mnx
