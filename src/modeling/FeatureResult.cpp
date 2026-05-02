#include "mnx/modeling/FeatureResult.h"

#include <utility>

namespace mnx {

FeatureResult::FeatureResult(Status status) : status_(std::move(status)) {}

FeatureResult FeatureResult::success() {
    return FeatureResult(Status::ok());
}

FeatureResult FeatureResult::failure(Status status) {
    return FeatureResult(std::move(status));
}

bool FeatureResult::isOk() const {
    return status_.isOk();
}

const Status& FeatureResult::status() const {
    return status_;
}

void FeatureResult::addDiagnostic(std::string diagnostic) {
    diagnostics_.push_back(std::move(diagnostic));
}

const std::vector<std::string>& FeatureResult::diagnostics() const {
    return diagnostics_;
}

void FeatureResult::addBodyId(BodyId id) {
    if(id.isValid()) {
        bodyIds_.push_back(id);
    }
}

const std::vector<BodyId>& FeatureResult::bodyIds() const {
    return bodyIds_;
}

void FeatureResult::addBody(Body body) {
    addBodyId(body.id());
    bodies_.push_back(std::move(body));
}

const std::vector<Body>& FeatureResult::bodies() const {
    return bodies_;
}

} // namespace mnx
