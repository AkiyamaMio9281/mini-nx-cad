#pragma once

#include "mnx/assembly/TransformNode.h"
#include "mnx/core/ids/ComponentId.h"
#include "mnx/modeling/PartDocument.h"

#include <memory>
#include <string>

namespace mnx {

class ComponentInstance {
public:
    ComponentInstance(
        ComponentId id,
        std::string name,
        std::shared_ptr<const PartDocument> partDocument,
        TransformNode transformNode = {}
    );

    [[nodiscard]] ComponentId id() const;
    [[nodiscard]] const std::string& name() const;
    void setName(std::string name);

    [[nodiscard]] std::shared_ptr<const PartDocument> partDocument() const;
    void setPartDocument(std::shared_ptr<const PartDocument> partDocument);
    [[nodiscard]] const std::string& sourceDocumentName() const;

    [[nodiscard]] const TransformNode& transformNode() const;
    [[nodiscard]] TransformNode& transformNode();
    void setTransformNode(TransformNode node);

private:
    ComponentId id_;
    std::string name_;
    std::string sourceDocumentName_;
    std::shared_ptr<const PartDocument> partDocument_;
    TransformNode transformNode_;
};

} // namespace mnx
