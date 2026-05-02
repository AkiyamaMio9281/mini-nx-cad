#include "AppController.h"

#include "mnx/modeling/ExtrudeFeature.h"
#include "mnx/modeling/RevolveFeature.h"
#include "mnx/modeling/SketchFeature.h"
#include "mnx/persistence/PersistenceUtils.h"
#include "mnx/rendering/BodyRenderer.h"

#include <memory>
#include <utility>
#include <vector>

namespace mnx {

AppController::AppController() {
    newProject();
}

void AppController::newProject() {
    currentProjectName_ = "Untitled";
    currentProjectPath_ = "mini_nx_project.json";
    partDocument_ = std::make_unique<PartDocument>(currentProjectName_);
    assemblyDocument_ = AssemblyDocument("Assembly");
    selectedFeatureId_ = FeatureId::invalid();
    selectedComponentId_ = ComponentId::invalid();
    auto sketchFeature = std::make_unique<SketchFeature>(FeatureId(1), "Sketch 1");
    const Result<FeatureId> insertedFeature = partDocument_->featureGraph().insertFeature(std::move(sketchFeature));
    if(insertedFeature) {
        selectedFeatureId_ = insertedFeature.value();
    }
    rebuildRenderScene();
    setStatus("New project ready");
}

void AppController::openProject() {
    const Status status = openProjectFrom(currentProjectPath_);
    if(!status) {
        setStatus("Open failed: " + status.message());
    }
}

void AppController::saveProject() {
    const Status status = saveProjectTo(currentProjectPath_);
    if(!status) {
        setStatus("Save failed: " + status.message());
    }
}

void AppController::saveProjectAs() {
    currentProjectPath_ = currentProjectName_ + ".mnx.json";
    const Status status = saveProjectTo(currentProjectPath_);
    if(!status) {
        setStatus("Save as failed: " + status.message());
    }
}

Status AppController::openProjectFrom(const std::filesystem::path& path) {
    Result<PartDocument> loaded = persistence::PersistenceUtils::loadProject(path);
    if(!loaded) {
        return loaded.status();
    }

    partDocument_ = std::make_unique<PartDocument>(std::move(loaded.value()));
    assemblyDocument_ = AssemblyDocument("Assembly");
    currentProjectName_ = partDocument_->name();
    currentProjectPath_ = path;

    selectedFeatureId_ = FeatureId::invalid();
    selectedComponentId_ = ComponentId::invalid();
    const std::vector<FeatureId> featureIds = partDocument_->featureGraph().featureIds();
    if(!featureIds.empty()) {
        selectedFeatureId_ = featureIds.front();
    }
    rebuildRenderScene();
    setStatus("Opened " + path.string());
    return Status::ok();
}

Status AppController::saveProjectTo(const std::filesystem::path& path) {
    if(partDocument_ == nullptr) {
        return Status::failedPrecondition("No active project to save");
    }
    Status status = persistence::PersistenceUtils::saveProject(*partDocument_, path);
    if(!status) {
        return status;
    }
    currentProjectPath_ = path;
    currentProjectName_ = partDocument_->name();
    setStatus("Saved " + path.string());
    return Status::ok();
}

const std::string& AppController::currentProjectName() const {
    return currentProjectName_;
}

const std::string& AppController::statusMessage() const {
    return statusMessage_;
}

SketchController& AppController::sketchController() {
    return sketchController_;
}

const SketchController& AppController::sketchController() const {
    return sketchController_;
}

PartDocument& AppController::partDocument() {
    return *partDocument_;
}

const PartDocument& AppController::partDocument() const {
    return *partDocument_;
}

AssemblyDocument& AppController::assemblyDocument() {
    return assemblyDocument_;
}

const AssemblyDocument& AppController::assemblyDocument() const {
    return assemblyDocument_;
}

const Feature* AppController::selectedFeature() const {
    if(partDocument_ == nullptr || !selectedFeatureId_.isValid()) {
        return nullptr;
    }
    return partDocument_->featureGraph().findFeature(selectedFeatureId_);
}

const ComponentInstance* AppController::selectedComponent() const {
    if(!selectedComponentId_.isValid()) {
        return nullptr;
    }
    return assemblyDocument_.findComponent(selectedComponentId_);
}

rendering::RenderScene& AppController::renderScene() {
    return renderScene_;
}

const rendering::RenderScene& AppController::renderScene() const {
    return renderScene_;
}

void AppController::selectFeature(FeatureId id) {
    if(partDocument_ == nullptr || partDocument_->featureGraph().findFeature(id) == nullptr) {
        selectedFeatureId_ = FeatureId::invalid();
        setStatus("No feature selected");
        return;
    }
    selectedFeatureId_ = id;
    const Feature* feature = selectedFeature();
    setStatus(feature == nullptr ? "No feature selected" : "Selected " + feature->name());
}

void AppController::selectComponent(ComponentId id) {
    if(assemblyDocument_.findComponent(id) == nullptr) {
        selectedComponentId_ = ComponentId::invalid();
        setStatus("No component selected");
        return;
    }
    selectedComponentId_ = id;
    const ComponentInstance* component = selectedComponent();
    setStatus(component == nullptr ? "No component selected" : "Selected " + component->name());
}

void AppController::rebuildRenderScene() {
    renderScene_.clear();
    if(partDocument_ == nullptr) {
        return;
    }

    const rendering::BodyRenderer renderer;
    for(const Feature* feature : partDocument_->featureGraph().features()) {
        if(feature == nullptr) {
            continue;
        }
        if(const auto* extrude = dynamic_cast<const ExtrudeFeature*>(feature);
           extrude != nullptr && extrude->body().has_value()) {
            renderer.populateScene(renderScene_, *extrude->body(), extrude->name());
        } else if(const auto* revolve = dynamic_cast<const RevolveFeature*>(feature);
                  revolve != nullptr && revolve->body().has_value()) {
            renderer.populateScene(renderScene_, *revolve->body(), revolve->name());
        }
    }

    for(const ComponentInstance& component : assemblyDocument_.components()) {
        std::shared_ptr<const PartDocument> part = component.partDocument();
        if(!part) {
            continue;
        }
        Result<Transform3D> world = assemblyDocument_.worldTransform(component.id());
        if(!world) {
            continue;
        }
        for(const Feature* feature : part->featureGraph().features()) {
            if(feature == nullptr) {
                continue;
            }
            if(const auto* extrude = dynamic_cast<const ExtrudeFeature*>(feature);
               extrude != nullptr && extrude->body().has_value()) {
                rendering::RenderItem item = renderer.makeRenderItem(*extrude->body(), component.name() + " / " + extrude->name());
                item.setTransform(world.value());
                (void)renderScene_.addItem(std::move(item));
            } else if(const auto* revolve = dynamic_cast<const RevolveFeature*>(feature);
                      revolve != nullptr && revolve->body().has_value()) {
                rendering::RenderItem item = renderer.makeRenderItem(*revolve->body(), component.name() + " / " + revolve->name());
                item.setTransform(world.value());
                (void)renderScene_.addItem(std::move(item));
            }
        }
    }
}

void AppController::setStatus(std::string message) {
    statusMessage_ = std::move(message);
}

} // namespace mnx
