QT += core gui opengl

TARGET = SphereCubed

TEMPLATE = app

SOURCES += \
    camera.cpp \
    engine.cpp \
    game.cpp \
    machine.cpp \
    main.cpp \
    menu.cpp \
    window.cpp \
    world.cpp

HEADERS += \
    camera.h \
    engine.h \
    game.h \
    machine.h \
    menu.h \
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

CONFIG(debug, debug|release) {
  message("Debug")
  CONFIG  += debug
  DEFINES += DEBUG
}else{
  message("Release")
  DEFINES += QT_NO_DEBUG_OUTPUT
}
