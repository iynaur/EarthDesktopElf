#include <QtWidgets>
#include <QtOpenGL/QtOpenGL>
//#include <QtOpenGL/glut>
#include <QtOpenGL/qgl.h>


#ifdef OPENGL_IS_A_FRAMEWORK
# include <OpenGL/glu.h>
#else
# include <GL/glu.h>
#endif
#include "earth.h"
#include "common.h"

#include <QDebug>

Earth::Earth(QWidget *parent, bool fs)
    : QOpenGLWidget(parent)
    , fullscene(fs)
    , speed(2)
    , isShowSlider(true)
{
    setIdentity(cloud_matrix_);
    setupWindowStyle();
    setupTrayIcon();
    setupSpeedSlider();
    QTimer* timer = new QTimer(this);
    timer->setInterval(30);
    connect(timer, &QTimer::timeout, this, static_cast<void(Earth::*)()>(&Earth::update));
    //timer->start();
}

void Earth::initializeGL()
{
    initializeOpenGLFunctions();

    loadGLTextures();

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0 ,0.0, 0.0, 0.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void Earth::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawSphere();
    //update();
}

void Earth::resizeGL(int width, int height)
{
    if (height == 0) {
        height = 1;
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Earth::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_F11:
        fullscene = !fullscene;
        if (fullscene) {
            showFullScreen();
        } else {
            showNormal();
            setGeometry(100, 100, 800, 600);
        }
        break;
    case Qt::Key_Escape:
        close();
        break;
    }
}

void Earth::closeEvent(QCloseEvent *)
{
    qApp->quit();
}

void Earth::mousePressEvent(QMouseEvent *event)
{
    x_ = event->x();
    y_ = event->y();

    trackball_.start(x_, y_);
}

void Earth::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();
    auto modifiers = event -> modifiers();
    auto buttons = event -> buttons();
    float transform[MATRIX_SIZE];

    int dx = (x - x_);
    int dy = (y - y_);
    if (dx == 0 && dy == 0)
      return;
    trackball_.update(x, y);
    if (modifiers & CTRL)
      getTranslateMatrix(dx, dy, transform);
    else if (modifiers & ALT)
      getZTranslateMatrix(dy, transform);
    else if (modifiers & SHFT)
      getScaleMatrix(dy, transform);
    else
      trackball_.getRotationMatrix(transform);

    ::multMatrix(cloud_matrix_, transform, cloud_matrix_);
//    glMultMatrixf(cloud_matrix_);
    update();

    x_ = x;
    y_ = y;
}

const float scale_factor_ = 1.14;
const float translate_factor_ = 0.001f;

void
Earth::getTranslateMatrix (int dx, int dy, float* matrix)
{
  setIdentity(matrix);
  float scale = 1.0f / 1;
  matrix[12] = float(dx) * translate_factor_ * scale;
  matrix[13] = float(-dy) * translate_factor_ * scale;
}

void
Earth::getZTranslateMatrix (int dy, float* matrix)
{
  setIdentity(matrix);
  matrix[14] = float(dy) * translate_factor_ / 1;
}

void
Earth::getScaleMatrix (int dy, float* matrix)
{
  setIdentity(matrix);
  float scale = dy > 0 ? scale_factor_ : 1.0 / scale_factor_;
  for (unsigned int i = 0; i < MATRIX_SIZE-1; i+=MATRIX_SIZE_DIM+1)
    matrix[i] = scale;
}


void Earth::mouseReleaseEvent(QMouseEvent *event)
{

}

void Earth::drawSphere()
{
    glLoadIdentity();

    static float rotatedSpeed = 0;

    GLUquadricObj *qobj = gluNewQuadric();

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glEnable(GL_TEXTURE_2D);
    gluQuadricTexture(qobj, GL_TRUE);

    glTranslated(0.0, 0.0, -6.0);
    //glRotated(-90, 1.0, 0.0, 0.0);
    //glRotated(rotatedSpeed, 0.0, 0.0, 1.0);
    glMultMatrixf(cloud_matrix_);
    glColor3f(1.0, 1.0, 1.0);

    gluSphere(qobj, 1.6, 60, 60);

    glDisable(GL_TEXTURE_2D);

    //rotatedSpeed += speed/10;
}

void Earth::loadGLTextures()
{
    QImage *image = new QImage;
    image->load(":/resources/earth.bmp");
    QImage tex = QGLWidget::convertToGLFormat(*image);
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Earth::setupWindowStyle()
{
//    setWindowFlags(windowFlags()
//                   |Qt::FramelessWindowHint
//                   |Qt::X11BypassWindowManagerHint
//                   |Qt::WindowStaysOnTopHint
//                   |Qt::Tool);
//    setAttribute(Qt::WA_TranslucentBackground);
//    setWindowState(Qt::WindowNoState | Qt::WindowFullScreen);
//    setFocusPolicy(Qt::NoFocus);
//    setWindowOpacity(1.0);

    setWindowIcon(QIcon(":/resources/earth.ico"));
    setGeometry(qApp->desktop()->width() / 8 * 7, qApp->desktop()->height() / 12
                , 200, 200);
}

void Earth::setupTrayIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/resources/earth.ico"));
    trayIcon->setToolTip(tr("your earth"));

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason))
            , this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));

    createTrayIconAction();
    createTrayIconMenu();

    trayIcon->show();
}

void Earth::createTrayIconMenu()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(settingAction);
    trayIconMenu->addAction(feedbackAction);
    trayIconMenu->addAction(helpAction);
    trayIconMenu->addAction(aboutAction);
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);
}

void Earth::createTrayIconAction()
{
    settingAction = new QAction(tr("setting"), this);
    feedbackAction = new QAction(tr("feedback"), this);
    helpAction = new QAction(tr("help"), this);
    aboutAction = new QAction(tr("about"), this);
    quitAction = new QAction(tr("quit"), this);

    connect(quitAction, SIGNAL(triggered(bool)), this, SLOT(close()));
}

void Earth::setupSpeedSlider()
{
    speedSlider = new QSlider(this);
    speedSlider->setOrientation(Qt::Horizontal);
    speedSlider->setRange(0, 100);
    speedSlider->setValue(speed * 10.0);

    speedSlider->setParent(qApp->desktop());
    speedSlider->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    speedSlider->setGeometry(50
                             ,20
                             , 100, 20);

    speedSlider->installEventFilter(this);
    speedSlider->show();

    connect(speedSlider, SIGNAL(valueChanged(int)), this, SLOT(setSpeed(int)));
}

void Earth::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(1) {
        isShowSlider = !isShowSlider;
        if (isShowSlider) {
            speedSlider->show();
            speedSlider->activateWindow();
        } else {
            //speedSlider->hide();
        }
    }
}

void Earth::setSpeed(int value)
{
    speed = value / 10.0;
}

bool Earth::eventFilter(QObject *watched, QEvent *event)
{
    if ((watched == speedSlider && event->type() == QEvent::FocusOut)) {
        //speedSlider->hide();
    }

    return QWidget::eventFilter(watched, event);
}
