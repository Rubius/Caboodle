#include "glwidget.h"
#include <QSurfaceFormat>
#include <QResizeEvent>

#include <GraphicsToolsModule/gtcamera.h>

#include "glrender.h"
#include "glthread.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , _render(new GLRender())
    , _thread(new GLThread(_render.data(), this))
    , _camera(new GtCamera)
{
    connect(_render.data(), SIGNAL(imageUpdated()), this, SLOT(update()));

    setMouseTracking(true);

    QSurfaceFormat format = this->format();
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    setFormat(format);

    _camera->setProjectionProperties(45.f,1.f,15000.f);

    _camera->setSceneBox(BoundingBox(Point3F(-10000.f,-10000.f, 3.f), Point3F(10000.f,10000.f,10000.f)));
    _camera->setIsometric(false);
    _camera->setPosition({0.f,0.f,100.f}, {0.f,0.f,-1.f}, {0.f,1.f,0.f});

    _render->SetCamera(_camera.data());
}

GLWidget::~GLWidget()
{
    _thread->Stop();
}

void GLWidget::StartRendering()
{
    doneCurrent();
    context()->moveToThread(_thread.data());

    _thread->Start();
}

void GLWidget::paintEvent(QPaintEvent* )
{
}

void GLWidget::resizeEvent(QResizeEvent* e)
{
    _thread->Stop();
    Super::resizeEvent(e);
    StartRendering();

    auto eSize = e->size();
    _camera->resize(eSize.width(), eSize.height());
    _thread->Resize(eSize.width(), eSize.height());
}

void GLWidget::closeEvent(QCloseEvent* event)
{
    _thread->Stop();
    Super::closeEvent(event);
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    Point2I resolutional_screen_pos = event->pos();
    if(event->buttons() == Qt::MiddleButton) {
        _camera->rotate(_lastScreenPosition - resolutional_screen_pos);
    }
    else if(event->buttons() == Qt::RightButton) {
        _camera->rotateRPE(_lastScreenPosition - resolutional_screen_pos);
    }
    else if(event->buttons() != Qt::NoButton){
        Vector3F dist = _lastPlanePosition - _camera->unprojectPlane(resolutional_screen_pos);
        _camera->translate(dist.x(), dist.y());
    }
    _lastScreenPosition = resolutional_screen_pos;
    _lastPlanePosition = _camera->unprojectPlane(resolutional_screen_pos);
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
    _lastScreenPosition = event->pos();
    _lastPlanePosition = _camera->unprojectPlane(_lastScreenPosition);
    if(event->buttons() == Qt::MiddleButton) {
        _camera->setRotationPoint(_lastPlanePosition);
    }
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
    _camera->focusBind(event->pos());
    _camera->zoom(event->delta() > 0);
    _camera->focusRelease();
}

void GLWidget::keyReleaseEvent(QKeyEvent* e)
{
    switch(e->key())
    {
    case Qt::Key_P: _camera->setIsometric(false); break;
    case Qt::Key_I: _camera->setIsometric(true); break;
    default: break;
    };
}
