QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    main.cc \
    renderer/display.cc \
    renderer/renderer.cc \
    terrain_editor_window.cc

HEADERS += \
    renderer/display.h \
    renderer/renderer.h \
    terrain_editor_window.h

FORMS += \
    terrain_editor_window.ui

TRANSLATIONS += \
    e8terrain_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
