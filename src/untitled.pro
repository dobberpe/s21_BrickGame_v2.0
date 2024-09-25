QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

!isEmpty(LIBRARY) {
    LIBRARY_PATH = build/$$LIBRARY.a
    LIBS += -L. -l:$$LIBRARY_PATH
    TARGET = build/$$LIBRARY_dsk
} else {
    error("Please specify a library name using 'qmake LIBRARY=<name>'")
}

SOURCES += \
    gui/desktop/controller.cpp \
    gui/desktop/gamewindow.cpp \
    gui/desktop/main.cpp

HEADERS += \
    gui/desktop/controller.h \
    gui/desktop/gamewindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
