QT += widgets
QT += opengl

RC_FILE = ./resources/earth.rc

LIBS += -lGL  \
        -lGLU \
        -lGLEW \
        -lglut

TEMPLATE = app

SOURCES += main.cpp \
    earth.cpp

HEADERS += \
    earth.h

RESOURCES += \
    resources.qrc
