#include "mnx/assembly/ComponentInstance.h"

#include <utility>

namespace mnx {

ComponentInstance::ComponentInstance(
    ComponentId id,
    std::string name,
    std::shared_ptr<const PartDocument> partDocument,
    TransformNode transformNode
)
    : id_(id),
      name_(std::move(name)),
      sourceDocumentName_(partDocument ? partDocument->name() : std::string()),
      partDocument_(std::move(partDocument)),
      transformNode_(std::move(transformNode)) {
    if(name_.empty()) {
        name_ = sourceDocumentName_.empty() ? "Component " + std::to_string(id_.value()) : sourceDocumentName_;
    }
}

ComponentId ComponentInstance::id() const {
    return id_;
}

const std::string& ComponentInstance::name() const {
    return name_;
}

void ComponentInstance::setName(std::string name) {
    name_ = std::move(name);
}

std::shared_ptr<const PartDocument> ComponentInstance::partDocument() const {
    return partDocument_;
}

void ComponentInstance::setPartDocument(std::shared_ptr<const PartDocument> partDocument) {
    partDocument_ = std::move(partDocument);
    sourceDocumentName_ = partDocument_ ? partDocument_->name() : std::string();
}

const std::string& ComponentInstance::sourceDocumentName() const {
    return sourceDocumentName_;
}

const TransformNode& ComponentInstance::transformNode() const {
    return transformNode_;
}

TransformNode& ComponentInstance::transformNode() {
    return transformNode_;
}

void ComponentInstance::setTransformNode(TransformNode node) {
    transformNode_ = std::move(node);
}

} // namespace mnx
