#pragma once

#include "mnx/rendering/Camera.h"
#include "mnx/rendering/RenderScene.h"

#include <QOpenGLWidget>
#include <QPoint>

namespace mnx::rendering {

class SceneView final : public QOpenGLWidget {
public:
    explicit SceneView(QWidget* parent = nullptr);

    [[nodiscard]] RenderScene& scene();
    [[nodiscard]] const RenderScene& scene() const;
    [[nodiscard]] Camera& camera();
    [[nodiscard]] const Camera& camera() const;
    void setScene(RenderScene scene);

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void drawItem(const RenderItem& item);
    [[nodiscard]] Ray rayFromMouse(const QPoint& position) const;

    RenderScene scene_;
    Camera camera_;
    QPoint lastMousePosition_;
};

} // namespace mnx::rendering
