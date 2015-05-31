QT += core gui opengl

TARGET = SphereCubed

TEMPLATE = app

SOURCES += \
    machine.cpp \
    main.cpp \
    window.cpp

HEADERS += \
    machine.h \
    settings.h \
    trace.h \
    window.h

DISTFILES += \
    SphereCubed.png

RESOURCES += \
    icons.qrc

CONFIG(debug, debug|release) {
  message("Debug")
  CONFIG  += debug
  DEFINES += DEBUG
}else{
  message("Release")
  DEFINES += QT_NO_DEBUG_OUTPUT
}
