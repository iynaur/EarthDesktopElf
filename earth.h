#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QSystemTrayIcon>

#include "trackball.h"

QT_BEGIN_NAMESPACE
class QMenu;
class QAction;
class QSlider;
QT_END_NAMESPACE

class Earth : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Earth(QWidget *parent = 0, bool fs = false);

    void getTranslateMatrix(int dx, int dy, float *matrix);
    void getZTranslateMatrix(int dy, float *matrix);
    void getScaleMatrix(int dy, float *matrix);
protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int, int) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *) Q_DECL_OVERRIDE;

    void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;

    /// mouse press control
    void
    mousePressEvent (QMouseEvent *event) override;

    /// mouse move control
    void
    mouseMoveEvent (QMouseEvent *event) override;

    /// mouse release control
    void
    mouseReleaseEvent (QMouseEvent *event) override;

private:
    void drawSphere();
    void loadGLTextures();

    void setupWindowStyle();
    void setupTrayIcon();
    void createTrayIconMenu();
    void createTrayIconAction();
    void setupSpeedSlider();

private slots:
    void trayActivated(QSystemTrayIcon::ActivationReason);
    void setSpeed(int);
    bool eventFilter(QObject *, QEvent *) Q_DECL_OVERRIDE;

private:
    bool fullscene;
    GLuint texture[1];

    float speed;
    bool isShowSlider;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *settingAction;
    QAction *feedbackAction;
    QAction *helpAction;
    QAction *aboutAction;
    QAction *quitAction;

    QPoint oldPos;
    QSlider *speedSlider;

    /// the trackball associated with this transform
    TrackBall trackball_;

    /// last recorded mouse positions
    int x_, y_;
    float cloud_matrix_[MATRIX_SIZE];
};

#endif // SOLARSYSTEM_H
