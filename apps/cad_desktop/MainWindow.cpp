#include "MainWindow.h"

#include "AssemblyTreeWidget.h"
#include "FeaturePropertyPanel.h"
#include "FeatureTreeWidget.h"
#include "MateEditorPanel.h"

#include <QAction>
#include <QActionGroup>
#include <QDockWidget>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QKeySequence>
#include <QStatusBar>
#include <QTabWidget>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

#include "mnx/sketch/gui/SketchView.h"
#include "mnx/rendering/SceneView.h"

namespace mnx {

namespace {

void configureApplicationShortcut(QAction* action) {
    action->setShortcutContext(Qt::ApplicationShortcut);
}

} // namespace

MainWindow::MainWindow(AppController& controller, QWidget* parent)
    : QMainWindow(parent), controller_(controller) {
    createActions();
    createMenus();
    createToolBars();
    createDocks();
    createCentralView();

    statusLabel_ = new QLabel(this);
    statusBar()->addPermanentWidget(statusLabel_);
    controller_.sketchController().setStatusCallback([this](std::string statusText) {
        statusBar()->showMessage(QString::fromStdString(statusText));
    });

    setMinimumSize(960, 640);
    refreshUiState();
}

void MainWindow::createActions() {
    newAction_ = new QAction(tr("&New"), this);
    openAction_ = new QAction(tr("&Open"), this);
    saveAction_ = new QAction(tr("&Save"), this);
    saveAsAction_ = new QAction(tr("Save &As"), this);
    undoAction_ = new QAction(tr("&Undo"), this);
    redoAction_ = new QAction(tr("&Redo"), this);
    selectToolAction_ = new QAction(tr("Select"), this);
    lineToolAction_ = new QAction(tr("Line"), this);
    circleToolAction_ = new QAction(tr("Circle"), this);
    rectangleToolAction_ = new QAction(tr("Rectangle"), this);

    newAction_->setShortcuts(QKeySequence::New);
    openAction_->setShortcuts(QKeySequence::Open);
    saveAction_->setShortcuts(QKeySequence::Save);
    saveAsAction_->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
    undoAction_->setShortcuts(QKeySequence::Undo);
    redoAction_->setShortcuts(QKeySequence::Redo);

    QList<QKeySequence> redoShortcuts = redoAction_->shortcuts();
    redoShortcuts.append(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Z));
    redoAction_->setShortcuts(redoShortcuts);

    selectToolAction_->setShortcut(QKeySequence(Qt::Key_Escape));
    lineToolAction_->setShortcut(QKeySequence(Qt::Key_L));
    circleToolAction_->setShortcut(QKeySequence(Qt::Key_C));
    rectangleToolAction_->setShortcut(QKeySequence(Qt::Key_R));

    for(QAction* action :
        {newAction_, openAction_, saveAction_, saveAsAction_, undoAction_, redoAction_, selectToolAction_,
         lineToolAction_, circleToolAction_, rectangleToolAction_}) {
        configureApplicationShortcut(action);
    }

    sketchToolActions_ = new QActionGroup(this);
    for(QAction* action : {selectToolAction_, lineToolAction_, circleToolAction_, rectangleToolAction_}) {
        action->setCheckable(true);
        sketchToolActions_->addAction(action);
    }
    selectToolAction_->setChecked(true);

    connect(newAction_, &QAction::triggered, this, [this]() {
        controller_.newProject();
        refreshUiState();
    });
    connect(openAction_, &QAction::triggered, this, [this]() {
        controller_.openProject();
        refreshUiState();
    });
    connect(saveAction_, &QAction::triggered, this, [this]() {
        controller_.saveProject();
        refreshUiState();
    });
    connect(saveAsAction_, &QAction::triggered, this, [this]() {
        controller_.saveProjectAs();
        refreshUiState();
    });
    connect(undoAction_, &QAction::triggered, this, [this]() {
        controller_.sketchController().undo();
        refreshUiState();
    });
    connect(redoAction_, &QAction::triggered, this, [this]() {
        controller_.sketchController().redo();
        refreshUiState();
    });
    connect(selectToolAction_, &QAction::triggered, this, [this]() {
        setSketchTool(SketchToolKind::Select);
    });
    connect(lineToolAction_, &QAction::triggered, this, [this]() {
        setSketchTool(SketchToolKind::Line);
    });
    connect(circleToolAction_, &QAction::triggered, this, [this]() {
        setSketchTool(SketchToolKind::Circle);
    });
    connect(rectangleToolAction_, &QAction::triggered, this, [this]() {
        setSketchTool(SketchToolKind::Rectangle);
    });
}

void MainWindow::createMenus() {
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction_);
    fileMenu->addAction(openAction_);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction_);
    fileMenu->addAction(saveAsAction_);

    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAction_);
    editMenu->addAction(redoAction_);
}

void MainWindow::createToolBars() {
    QToolBar* fileToolBar = addToolBar(tr("File"));
    fileToolBar->setObjectName(QStringLiteral("fileToolBar"));
    fileToolBar->addAction(newAction_);
    fileToolBar->addAction(openAction_);
    fileToolBar->addAction(saveAction_);
    fileToolBar->addSeparator();
    fileToolBar->addAction(undoAction_);
    fileToolBar->addAction(redoAction_);

    QToolBar* sketchToolBar = addToolBar(tr("Sketch"));
    sketchToolBar->setObjectName(QStringLiteral("sketchToolBar"));
    sketchToolBar->addAction(selectToolAction_);
    sketchToolBar->addAction(lineToolAction_);
    sketchToolBar->addAction(circleToolAction_);
    sketchToolBar->addAction(rectangleToolAction_);
}

void MainWindow::createDocks() {
    auto* modelTree = new QDockWidget(tr("Model"), this);
    modelTree->setObjectName(QStringLiteral("modelDock"));
    featureTree_ = new FeatureTreeWidget(modelTree);
    featureTree_->setFeatureGraph(&controller_.partDocument().featureGraph());
    featureTree_->setSelectionCallback([this](FeatureId id) {
        controller_.selectFeature(id);
        refreshUiState();
    });
    modelTree->setWidget(featureTree_);
    addDockWidget(Qt::LeftDockWidgetArea, modelTree);

    auto* assemblyTreeDock = new QDockWidget(tr("Assembly"), this);
    assemblyTreeDock->setObjectName(QStringLiteral("assemblyDock"));
    assemblyTree_ = new AssemblyTreeWidget(assemblyTreeDock);
    assemblyTree_->setAssemblyDocument(&controller_.assemblyDocument());
    assemblyTree_->setSelectionCallback([this](ComponentId id) {
        controller_.selectComponent(id);
        refreshUiState();
    });
    assemblyTreeDock->setWidget(assemblyTree_);
    addDockWidget(Qt::LeftDockWidgetArea, assemblyTreeDock);

    auto* properties = new QDockWidget(tr("Properties"), this);
    properties->setObjectName(QStringLiteral("propertiesDock"));
    featurePropertyPanel_ = new FeaturePropertyPanel(properties);
    properties->setWidget(featurePropertyPanel_);
    addDockWidget(Qt::RightDockWidgetArea, properties);

    auto* mateEditor = new QDockWidget(tr("Mates"), this);
    mateEditor->setObjectName(QStringLiteral("matesDock"));
    mateEditorPanel_ = new MateEditorPanel(mateEditor);
    mateEditorPanel_->setAssembly(&controller_.assemblyDocument(), ComponentId::invalid());
    mateEditor->setWidget(mateEditorPanel_);
    addDockWidget(Qt::RightDockWidgetArea, mateEditor);
}

void MainWindow::createCentralView() {
    centralTabs_ = new QTabWidget(this);
    centralTabs_->addTab(new SketchView(controller_.sketchController(), centralTabs_), tr("Sketch"));
    sceneView_ = new rendering::SceneView(centralTabs_);
    sceneView_->setScene(controller_.renderScene());
    centralTabs_->addTab(sceneView_, tr("3D"));
    setCentralWidget(centralTabs_);
}

void MainWindow::setSketchTool(SketchToolKind tool) {
    controller_.sketchController().setActiveTool(tool);
}

void MainWindow::refreshUiState() {
    const QString projectName = QString::fromStdString(controller_.currentProjectName());
    setWindowTitle(tr("Mini NX - %1").arg(projectName));

    const QString status = QString::fromStdString(controller_.statusMessage());
    statusBar()->showMessage(status);
    if(statusLabel_ != nullptr) {
        statusLabel_->setText(projectName);
    }
    if(featureTree_ != nullptr) {
        featureTree_->setFeatureGraph(&controller_.partDocument().featureGraph());
        featureTree_->refresh();
    }
    if(assemblyTree_ != nullptr) {
        assemblyTree_->setAssemblyDocument(&controller_.assemblyDocument());
        assemblyTree_->refresh();
    }
    if(featurePropertyPanel_ != nullptr) {
        const Feature* selectedFeature = controller_.selectedFeature();
        const bool dirty =
            selectedFeature != nullptr && controller_.partDocument().featureGraph().isDirty(selectedFeature->id());
        featurePropertyPanel_->setFeature(selectedFeature, dirty);
    }
    if(mateEditorPanel_ != nullptr) {
        const ComponentInstance* selectedComponent = controller_.selectedComponent();
        mateEditorPanel_->setAssembly(
            &controller_.assemblyDocument(),
            selectedComponent == nullptr ? ComponentId::invalid() : selectedComponent->id()
        );
    }
    if(sceneView_ != nullptr) {
        controller_.rebuildRenderScene();
        sceneView_->setScene(controller_.renderScene());
    }
}

} // namespace mnx
