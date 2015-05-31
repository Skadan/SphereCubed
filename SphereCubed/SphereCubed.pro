QT += core gui opengl

TARGET = SphereCubed

TEMPLATE = app

SOURCES += \
    box.cpp \
    camera.cpp \
    engine.cpp \
    frustum.cpp \
    game.cpp \
    machine.cpp \
    main.cpp \
    menu.cpp \
    plane.cpp \
    window.cpp \
    world.cpp

HEADERS += \
    box.h \
    camera.h \
    engine.h \
    frustum.h \
    game.h \
    machine.h \
    menu.h \
    plane.h \
    settings.h \
    trace.h \
    window.h \
    world.h

DISTFILES += \
    SphereCubed.png \
    Images/Died.png \
    Images/Finished.png \
    Images/Menu.png \
    Images/Over.png \
    Images/Won.png \
    Shaders/menushader.frag \
    Shaders/menushader.vert

RESOURCES += \
    icons.qrc \
    images.qrc \
    shaders.qrc

QMAKE_CXXFLAGS += -std=c++11

CONFIG(debug, debug|release) {
  message("Debug")
  CONFIG  += debug
  DEFINES += DEBUG
}else{
  message("Release")
  DEFINES += QT_NO_DEBUG_OUTPUT
}
