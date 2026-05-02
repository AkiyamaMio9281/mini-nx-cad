#pragma once

#include "mnx/core/ids/FeatureId.h"
#include "mnx/core/utils/Status.h"
#include "mnx/assembly/AssemblyDocument.h"
#include "mnx/modeling/PartDocument.h"
#include "mnx/rendering/RenderScene.h"
#include "mnx/sketch/gui/SketchController.h"

#include <filesystem>
#include <memory>
#include <string>

namespace mnx {

class AppController {
public:
    AppController();

    void newProject();
    void openProject();
    void saveProject();
    void saveProjectAs();
    [[nodiscard]] Status openProjectFrom(const std::filesystem::path& path);
    [[nodiscard]] Status saveProjectTo(const std::filesystem::path& path);

    [[nodiscard]] const std::string& currentProjectName() const;
    [[nodiscard]] const std::string& statusMessage() const;
    [[nodiscard]] SketchController& sketchController();
    [[nodiscard]] const SketchController& sketchController() const;
    [[nodiscard]] PartDocument& partDocument();
    [[nodiscard]] const PartDocument& partDocument() const;
    [[nodiscard]] AssemblyDocument& assemblyDocument();
    [[nodiscard]] const AssemblyDocument& assemblyDocument() const;
    [[nodiscard]] const Feature* selectedFeature() const;
    [[nodiscard]] const ComponentInstance* selectedComponent() const;
    [[nodiscard]] rendering::RenderScene& renderScene();
    [[nodiscard]] const rendering::RenderScene& renderScene() const;

    void selectFeature(FeatureId id);
    void selectComponent(ComponentId id);
    void rebuildRenderScene();

private:
    void setStatus(std::string message);

    std::string currentProjectName_;
    std::string statusMessage_;
    std::filesystem::path currentProjectPath_;
    SketchController sketchController_;
    std::unique_ptr<PartDocument> partDocument_;
    AssemblyDocument assemblyDocument_;
    FeatureId selectedFeatureId_;
    ComponentId selectedComponentId_;
    rendering::RenderScene renderScene_;
};

} // namespace mnx
