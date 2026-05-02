#pragma once

#include "mnx/core/ids/BodyId.h"
#include "mnx/core/utils/Status.h"
#include "mnx/modeling/Body.h"

#include <string>
#include <vector>

namespace mnx {

class FeatureResult {
public:
    FeatureResult() = default;
    explicit FeatureResult(Status status);

    [[nodiscard]] static FeatureResult success();
    [[nodiscard]] static FeatureResult failure(Status status);

    [[nodiscard]] bool isOk() const;
    [[nodiscard]] const Status& status() const;

    void addDiagnostic(std::string diagnostic);
    [[nodiscard]] const std::vector<std::string>& diagnostics() const;

    void addBodyId(BodyId id);
    [[nodiscard]] const std::vector<BodyId>& bodyIds() const;
    void addBody(Body body);
    [[nodiscard]] const std::vector<Body>& bodies() const;

private:
    Status status_;
    std::vector<std::string> diagnostics_;
    std::vector<BodyId> bodyIds_;
    std::vector<Body> bodies_;
};

} // namespace mnx
