#include "mnx/rendering/SceneView.h"

#include "mnx/rendering/Picking.h"

#include <QMouseEvent>
#include <QWheelEvent>

#include <algorithm>

namespace mnx::rendering {

SceneView::SceneView(QWidget* parent) : QOpenGLWidget(parent) {
    setMouseTracking(true);
}

RenderScene& SceneView::scene() {
    return scene_;
}

const RenderScene& SceneView::scene() const {
    return scene_;
}

Camera& SceneView::camera() {
    return camera_;
}

const Camera& SceneView::camera() const {
    return camera_;
}

void SceneView::setScene(RenderScene scene) {
    scene_ = std::move(scene);
    update();
}

void SceneView::initializeGL() {
    glClearColor(0.08F, 0.09F, 0.10F, 1.0F);
    glEnable(GL_DEPTH_TEST);
}

void SceneView::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
}

void SceneView::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const double aspect = height() > 0 ? static_cast<double>(width()) / static_cast<double>(height()) : 1.0;
    const Mat4 projection = camera_.projectionMatrix(aspect);
    glLoadMatrixd(projection.values().data());

    glMatrixMode(GL_MODELVIEW);
    const Mat4 view = camera_.viewMatrix();
    glLoadMatrixd(view.values().data());

    for(const RenderItem& item : scene_.items()) {
        if(item.isVisible()) {
            drawItem(item);
        }
    }
}

void SceneView::mousePressEvent(QMouseEvent* event) {
    lastMousePosition_ = event->pos();
    if(event->button() == Qt::LeftButton) {
        const std::optional<PickResult> pick = Picking::pickBody(scene_, rayFromMouse(event->pos()));
        if(pick.has_value()) {
            (void)scene_.selectBody(pick->bodyId);
        } else {
            scene_.clearSelection();
        }
        update();
    }
}

void SceneView::mouseMoveEvent(QMouseEvent* event) {
    const QPoint delta = event->pos() - lastMousePosition_;
    lastMousePosition_ = event->pos();

    if(event->buttons() & Qt::RightButton) {
        camera_.orbit(static_cast<double>(delta.x()) * 0.01, static_cast<double>(delta.y()) * 0.01);
        update();
    } else if(event->buttons() & Qt::MiddleButton) {
        const double scale = camera_.distance() * 0.0015;
        camera_.pan(-static_cast<double>(delta.x()) * scale, static_cast<double>(delta.y()) * scale);
        update();
    }
}

void SceneView::wheelEvent(QWheelEvent* event) {
    const double factor = event->angleDelta().y() > 0 ? 0.9 : 1.1;
    camera_.zoom(factor);
    update();
}

void SceneView::drawItem(const RenderItem& item) {
    if(item.isSelected()) {
        glColor3f(1.0F, 0.72F, 0.20F);
    } else if(item.isHighlighted()) {
        glColor3f(0.45F, 0.72F, 1.0F);
    } else {
        glColor3f(0.62F, 0.68F, 0.74F);
    }

    glBegin(GL_TRIANGLES);
    for(const MeshTriangle& triangle : item.mesh().triangles()) {
        for(std::size_t vertexIndex : triangle.vertexIndices) {
            const Point3& point = item.mesh().vertices()[vertexIndex].position;
            glVertex3d(point.x(), point.y(), point.z());
        }
    }
    glEnd();
}

Ray SceneView::rayFromMouse(const QPoint& position) const {
    const double nx = width() > 0 ? (2.0 * static_cast<double>(position.x()) / static_cast<double>(width())) - 1.0 : 0.0;
    const double ny =
        height() > 0 ? 1.0 - (2.0 * static_cast<double>(position.y()) / static_cast<double>(height())) : 0.0;
    const double aspect = height() > 0 ? static_cast<double>(width()) / static_cast<double>(height()) : 1.0;
    return camera_.screenRay(nx, ny, aspect);
}

} // namespace mnx::rendering
