#include "mnx/assembly/AssemblyDocument.h"

#include <algorithm>
#include <utility>

namespace mnx {

AssemblyDocument::AssemblyDocument(std::string name) : name_(std::move(name)) {}

const std::string& AssemblyDocument::name() const {
    return name_;
}

void AssemblyDocument::setName(std::string name) {
    name_ = std::move(name);
}

Result<ComponentId> AssemblyDocument::insertComponent(
    std::shared_ptr<const PartDocument> partDocument,
    std::string name,
    Transform3D localTransform,
    std::optional<ComponentId> parentId
) {
    if(!partDocument) {
        return Result<ComponentId>::failure(Status::invalidArgument("Assembly component requires a part document"));
    }
    if(parentId.has_value() && findComponent(*parentId) == nullptr) {
        return Result<ComponentId>::failure(Status::notFound("Assembly component parent was not found"));
    }

    const ComponentId id = nextComponentId();
    return insertComponentWithId(id, std::move(partDocument), std::move(name), localTransform, parentId);
}

Result<ComponentId> AssemblyDocument::insertComponentWithId(
    ComponentId id,
    std::shared_ptr<const PartDocument> partDocument,
    std::string name,
    Transform3D localTransform,
    std::optional<ComponentId> parentId
) {
    if(!id.isValid()) {
        return Result<ComponentId>::failure(Status::invalidArgument("Assembly component id is invalid"));
    }
    if(findComponent(id) != nullptr) {
        return Result<ComponentId>::failure(Status::failedPrecondition("Assembly component id already exists"));
    }
    if(!partDocument) {
        return Result<ComponentId>::failure(Status::invalidArgument("Assembly component requires a part document"));
    }
    if(parentId.has_value()) {
        if(findComponent(*parentId) == nullptr) {
            return Result<ComponentId>::failure(Status::notFound("Assembly component parent was not found"));
        }
        if(wouldCreateCycle(id, *parentId)) {
            return Result<ComponentId>::failure(Status::failedPrecondition("Assembly component parent would create a cycle"));
        }
    }
    components_.emplace_back(id, std::move(name), std::move(partDocument), TransformNode(localTransform, parentId));
    advanceNextComponentIdPast(id);
    return id;
}

Status AssemblyDocument::removeComponent(ComponentId id) {
    const auto iterator = std::find_if(components_.begin(), components_.end(), [id](const ComponentInstance& component) {
        return component.id() == id;
    });
    if(iterator == components_.end()) {
        return Status::notFound("Assembly component was not found");
    }
    components_.erase(iterator);
    mates_.erase(
        std::remove_if(mates_.begin(), mates_.end(), [id](const MateConstraint& mate) {
            return mate.firstComponentId() == id || mate.secondComponentId() == id;
        }),
        mates_.end()
    );
    for(ComponentInstance& component : components_) {
        if(component.transformNode().parentId() == id) {
            component.transformNode().setParentId(std::nullopt);
        }
    }
    return Status::ok();
}

ComponentInstance* AssemblyDocument::findComponent(ComponentId id) {
    const auto iterator = std::find_if(components_.begin(), components_.end(), [id](const ComponentInstance& component) {
        return component.id() == id;
    });
    return iterator == components_.end() ? nullptr : &*iterator;
}

const ComponentInstance* AssemblyDocument::findComponent(ComponentId id) const {
    const auto iterator = std::find_if(components_.begin(), components_.end(), [id](const ComponentInstance& component) {
        return component.id() == id;
    });
    return iterator == components_.end() ? nullptr : &*iterator;
}

std::span<ComponentInstance> AssemblyDocument::components() {
    return components_;
}

std::span<const ComponentInstance> AssemblyDocument::components() const {
    return components_;
}

Status AssemblyDocument::setLocalTransform(ComponentId id, const Transform3D& transform) {
    ComponentInstance* component = findComponent(id);
    if(component == nullptr) {
        return Status::notFound("Assembly component was not found");
    }
    component->transformNode().setLocalTransform(transform);
    return Status::ok();
}

Status AssemblyDocument::setWorldTransform(ComponentId id, const Transform3D& transform) {
    ComponentInstance* component = findComponent(id);
    if(component == nullptr) {
        return Status::notFound("Assembly component was not found");
    }
    if(component->transformNode().parentId().has_value()) {
        Result<Transform3D> parentWorld = worldTransform(*component->transformNode().parentId());
        if(!parentWorld) {
            return parentWorld.status();
        }
        component->transformNode().setLocalTransform(parentWorld.value().inverse() * transform);
    } else {
        component->transformNode().setLocalTransform(transform);
    }
    return Status::ok();
}

Result<Transform3D> AssemblyDocument::worldTransform(ComponentId id) const {
    const ComponentInstance* component = findComponent(id);
    if(component == nullptr) {
        return Result<Transform3D>::failure(Status::notFound("Assembly component was not found"));
    }
    Transform3D result = component->transformNode().localTransform();
    if(component->transformNode().parentId().has_value()) {
        Result<Transform3D> parent = worldTransform(*component->transformNode().parentId());
        if(!parent) {
            return Result<Transform3D>::failure(parent.status());
        }
        result = parent.value() * result;
    }
    return result;
}

Status AssemblyDocument::addMate(MateConstraint mate) {
    if(!mate.id().isValid()) {
        return Status::invalidArgument("Mate id is invalid");
    }
    if(findMate(mate.id()) != nullptr) {
        return Status::failedPrecondition("Mate id already exists");
    }
    if(findComponent(mate.firstComponentId()) == nullptr) {
        return Status::notFound("Mate first component was not found");
    }
    if(mate.type() != MateType::Fixed && findComponent(mate.secondComponentId()) == nullptr) {
        return Status::notFound("Mate second component was not found");
    }
    if(mate.type() == MateType::Distance && mate.distance() < 0.0) {
        return Status::invalidArgument("Distance mate value must be non-negative");
    }
    mates_.push_back(std::move(mate));
    return Status::ok();
}

Status AssemblyDocument::removeMate(ConstraintId id) {
    const auto iterator = std::find_if(mates_.begin(), mates_.end(), [id](const MateConstraint& mate) {
        return mate.id() == id;
    });
    if(iterator == mates_.end()) {
        return Status::notFound("Mate was not found");
    }
    mates_.erase(iterator);
    return Status::ok();
}

MateConstraint* AssemblyDocument::findMate(ConstraintId id) {
    const auto iterator = std::find_if(mates_.begin(), mates_.end(), [id](const MateConstraint& mate) {
        return mate.id() == id;
    });
    return iterator == mates_.end() ? nullptr : &*iterator;
}

const MateConstraint* AssemblyDocument::findMate(ConstraintId id) const {
    const auto iterator = std::find_if(mates_.begin(), mates_.end(), [id](const MateConstraint& mate) {
        return mate.id() == id;
    });
    return iterator == mates_.end() ? nullptr : &*iterator;
}

std::span<const MateConstraint> AssemblyDocument::mates() const {
    return mates_;
}

ComponentId AssemblyDocument::nextComponentId() {
    return ComponentId(nextComponentId_++);
}

void AssemblyDocument::advanceNextComponentIdPast(ComponentId id) {
    if(id.isValid() && id.value() >= nextComponentId_) {
        nextComponentId_ = id.value() + 1;
    }
}

bool AssemblyDocument::wouldCreateCycle(ComponentId childId, ComponentId parentId) const {
    ComponentId current = parentId;
    while(current.isValid()) {
        if(current == childId) {
            return true;
        }
        const ComponentInstance* parent = findComponent(current);
        if(parent == nullptr || !parent->transformNode().parentId().has_value()) {
            return false;
        }
        current = *parent->transformNode().parentId();
    }
    return false;
}

} // namespace mnx
