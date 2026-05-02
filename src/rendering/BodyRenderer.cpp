#include "mnx/rendering/BodyRenderer.h"

#include <string>

namespace mnx::rendering {

RenderItem BodyRenderer::makeRenderItem(const mnx::Body& body, std::string name) const {
    if(name.empty()) {
        name = "Body " + std::to_string(body.id().value());
    }
    return RenderItem(body.id(), std::move(name), Mesh::fromDisplayMesh(body.displayMesh()));
}

void BodyRenderer::populateScene(RenderScene& scene, const mnx::Body& body, std::string name) const {
    (void)scene.addItem(makeRenderItem(body, std::move(name)));
}

} // namespace mnx::rendering
