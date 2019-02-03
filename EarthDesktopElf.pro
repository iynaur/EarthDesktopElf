QT += widgets
QT += opengl

RC_FILE = ./resources/earth.rc

LIBS += -lGL  \
        -lGLU \
        -lGLEW

#pcl
CONFIG += link_pkgconfig
PKGCONFIG += eigen3
LIBS += -L/usr/local/lib
INCLUDEPATH += /usr/include/pcl-1.8

TEMPLATE = app

SOURCES += main.cpp \
    earth.cpp \
    trackball.cpp \
    common.cpp

HEADERS += \
    earth.h  \
    trackball.h

RESOURCES += \
    resources.qrc
