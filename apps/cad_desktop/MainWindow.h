#pragma once

#include "AppController.h"

#include <QMainWindow>

class QAction;
class QActionGroup;
class QLabel;
class QTabWidget;

namespace mnx {

class AssemblyTreeWidget;
class FeaturePropertyPanel;
class FeatureTreeWidget;
class MateEditorPanel;
namespace rendering {
class SceneView;
}

class MainWindow final : public QMainWindow {
public:
    explicit MainWindow(AppController& controller, QWidget* parent = nullptr);

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createDocks();
    void createCentralView();
    void setSketchTool(SketchToolKind tool);
    void refreshUiState();

    AppController& controller_;

    QAction* newAction_ = nullptr;
    QAction* openAction_ = nullptr;
    QAction* saveAction_ = nullptr;
    QAction* saveAsAction_ = nullptr;
    QAction* undoAction_ = nullptr;
    QAction* redoAction_ = nullptr;
    QAction* selectToolAction_ = nullptr;
    QAction* lineToolAction_ = nullptr;
    QAction* circleToolAction_ = nullptr;
    QAction* rectangleToolAction_ = nullptr;
    QActionGroup* sketchToolActions_ = nullptr;
    QLabel* statusLabel_ = nullptr;
    QTabWidget* centralTabs_ = nullptr;
    rendering::SceneView* sceneView_ = nullptr;
    FeatureTreeWidget* featureTree_ = nullptr;
    AssemblyTreeWidget* assemblyTree_ = nullptr;
    FeaturePropertyPanel* featurePropertyPanel_ = nullptr;
    MateEditorPanel* mateEditorPanel_ = nullptr;
};

} // namespace mnx
