QT += core gui opengl

TARGET = SphereCubed

TEMPLATE = app

SOURCES += \
    engine.cpp \
    game.cpp \
    machine.cpp \
    main.cpp \
    window.cpp \
    world.cpp

HEADERS += \
    engine.h \
    game.h \
    machine.h \
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
