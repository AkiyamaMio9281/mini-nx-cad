#pragma once

#include "mnx/assembly/ComponentInstance.h"
#include "mnx/assembly/MateConstraint.h"
#include "mnx/core/utils/Result.h"

#include <memory>
#include <span>
#include <string>
#include <vector>

namespace mnx {

class AssemblyDocument {
public:
    explicit AssemblyDocument(std::string name = "Assembly");

    [[nodiscard]] const std::string& name() const;
    void setName(std::string name);

    [[nodiscard]] Result<ComponentId> insertComponent(
        std::shared_ptr<const PartDocument> partDocument,
        std::string name = {},
        Transform3D localTransform = Transform3D::identity(),
        std::optional<ComponentId> parentId = std::nullopt
    );
    [[nodiscard]] Result<ComponentId> insertComponentWithId(
        ComponentId id,
        std::shared_ptr<const PartDocument> partDocument,
        std::string name = {},
        Transform3D localTransform = Transform3D::identity(),
        std::optional<ComponentId> parentId = std::nullopt
    );
    [[nodiscard]] Status removeComponent(ComponentId id);

    [[nodiscard]] ComponentInstance* findComponent(ComponentId id);
    [[nodiscard]] const ComponentInstance* findComponent(ComponentId id) const;
    [[nodiscard]] std::span<ComponentInstance> components();
    [[nodiscard]] std::span<const ComponentInstance> components() const;

    [[nodiscard]] Status setLocalTransform(ComponentId id, const Transform3D& transform);
    [[nodiscard]] Status setWorldTransform(ComponentId id, const Transform3D& transform);
    [[nodiscard]] Result<Transform3D> worldTransform(ComponentId id) const;

    [[nodiscard]] Status addMate(MateConstraint mate);
    [[nodiscard]] Status removeMate(ConstraintId id);
    [[nodiscard]] MateConstraint* findMate(ConstraintId id);
    [[nodiscard]] const MateConstraint* findMate(ConstraintId id) const;
    [[nodiscard]] std::span<const MateConstraint> mates() const;

private:
    [[nodiscard]] ComponentId nextComponentId();
    void advanceNextComponentIdPast(ComponentId id);
    [[nodiscard]] bool wouldCreateCycle(ComponentId childId, ComponentId parentId) const;

    std::string name_;
    std::vector<ComponentInstance> components_;
    std::vector<MateConstraint> mates_;
    ComponentId::ValueType nextComponentId_ = 1;
};

} // namespace mnx
