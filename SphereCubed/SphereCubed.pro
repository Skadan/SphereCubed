QT += core gui opengl

TARGET = SphereCubed

TEMPLATE = app

SOURCES += \
    main.cpp \
    window.cpp

HEADERS += \
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
