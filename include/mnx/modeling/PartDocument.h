#pragma once

#include "mnx/constraints/Constraint.h"
#include "mnx/modeling/FeatureGraph.h"

#include <memory>
#include <span>
#include <string>
#include <vector>

namespace mnx {

class PartDocument {
public:
    explicit PartDocument(std::string name = "Untitled");

    [[nodiscard]] const std::string& name() const;
    void setName(std::string name);

    [[nodiscard]] FeatureGraph& featureGraph();
    [[nodiscard]] const FeatureGraph& featureGraph() const;
    void addConstraint(std::unique_ptr<Constraint> constraint);
    void clearConstraints();
    [[nodiscard]] std::span<const std::unique_ptr<Constraint>> constraints() const;

private:
    std::string name_;
    FeatureGraph featureGraph_;
    std::vector<std::unique_ptr<Constraint>> constraints_;
};

} // namespace mnx
