#pragma once

#include "mnx/core/ids/FeatureId.h"
#include "mnx/modeling/FeatureResult.h"
#include "mnx/modeling/FeatureType.h"

#include <span>
#include <string>
#include <vector>

namespace mnx {

class FeatureContext;

class Feature {
public:
    Feature(FeatureId id, FeatureType type, std::string name);
    virtual ~Feature() = default;

    [[nodiscard]] FeatureId id() const;
    [[nodiscard]] FeatureType type() const;
    [[nodiscard]] const std::string& name() const;
    void setName(std::string name);

    [[nodiscard]] std::span<const FeatureId> dependencies() const;
    [[nodiscard]] bool dependsOn(FeatureId id) const;
    [[nodiscard]] Status addDependency(FeatureId id);
    void clearDependencies();

    [[nodiscard]] virtual FeatureResult execute(FeatureContext& context) = 0;

private:
    FeatureId id_;
    FeatureType type_;
    std::string name_;
    std::vector<FeatureId> dependencies_;
};

} // namespace mnx
