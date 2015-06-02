QT += core gui opengl

TARGET = SphereCubed

TEMPLATE = app

SOURCES += \
    box.cpp \
    camera.cpp \
    cube.cpp \
    engine.cpp \
    frustum.cpp \
    game.cpp \
    level.cpp \
    light.cpp \
    machine.cpp \
    main.cpp \
    menu.cpp \
    plane.cpp \
    player.cpp \
    sphere.cpp \
    window.cpp \
    world.cpp

HEADERS += \
    box.h \
    camera.h \
    cube.h \
    engine.h \
    frustum.h \
    game.h \
    level.h \
    light.h \
    machine.h \
    menu.h \
    plane.h \
    player.h \
    settings.h \
    sphere.h \
    trace.h \
    window.h \
    world.h

DISTFILES += \
    SphereCubed.png \
    Images/Concrete.png \
    Images/Died.png \
    Images/Finish.png \
    Images/Finished.png \
    Images/Menu.png \
    Images/Over.png \
    Images/Standard.png \
    Images/Start.png \
    Images/Won.png \
    Levels/Level1.lvl \
    Shaders/menushader.frag \
    Shaders/menushader.vert \
    Shaders/playshader.frag \
    Shaders/playshader.vert

RESOURCES += \
    icons.qrc \
    images.qrc \
    levels.qrc \
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
