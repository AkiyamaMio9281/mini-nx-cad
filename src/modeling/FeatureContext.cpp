#include "mnx/modeling/FeatureContext.h"

#include <utility>

namespace mnx {

void FeatureContext::clear() {
    executedFeatureIds_.clear();
    bodyIds_.clear();
    bodies_.clear();
    sketches_.clear();
    diagnostics_.clear();
}

void FeatureContext::recordExecutedFeature(FeatureId id) {
    if(id.isValid()) {
        executedFeatureIds_.push_back(id);
    }
}

const std::vector<FeatureId>& FeatureContext::executedFeatureIds() const {
    return executedFeatureIds_;
}

void FeatureContext::recordBody(BodyId id) {
    if(id.isValid()) {
        bodyIds_.push_back(id);
    }
}

const std::vector<BodyId>& FeatureContext::bodyIds() const {
    return bodyIds_;
}

void FeatureContext::recordBody(Body body) {
    recordBody(body.id());
    bodies_.push_back(std::move(body));
}

const std::vector<Body>& FeatureContext::bodies() const {
    return bodies_;
}

void FeatureContext::recordSketch(FeatureId id, const Sketch& sketch) {
    if(!id.isValid()) {
        return;
    }
    for(auto& entry : sketches_) {
        if(entry.first == id) {
            entry.second = &sketch;
            return;
        }
    }
    sketches_.push_back({id, &sketch});
}

const Sketch* FeatureContext::findSketch(FeatureId id) const {
    for(const auto& entry : sketches_) {
        if(entry.first == id) {
            return entry.second;
        }
    }
    return nullptr;
}

void FeatureContext::addDiagnostic(std::string diagnostic) {
    diagnostics_.push_back(std::move(diagnostic));
}

const std::vector<std::string>& FeatureContext::diagnostics() const {
    return diagnostics_;
}

} // namespace mnx
