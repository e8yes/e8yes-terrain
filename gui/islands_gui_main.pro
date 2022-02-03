QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -DBOOST_LOG_DYN_LINK
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native -g
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

INCLUDEPATH += ../

SOURCES += \
    islands_editor_window.cc \
    main.cc

HEADERS += \
    islands_editor_window.h

FORMS += \
    islands_editor_window.ui

TRANSLATIONS += \
    e8terrain_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Dependencies
# Islands generator
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../generator/release/ -lislands_generator
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../generator/debug/ -lislands_generator
else:unix: LIBS += -L$$OUT_PWD/../generator/ -lislands_generator

INCLUDEPATH += $$PWD/../generator
DEPENDPATH += $$PWD/../generator

# Islands renderer
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../renderer/release/ -lislands_renderer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../renderer/debug/ -lislands_renderer
else:unix: LIBS += -L$$OUT_PWD/../renderer/ -lislands_renderer

INCLUDEPATH += $$PWD/../renderer
DEPENDPATH += $$PWD/../renderer

# Islands common
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lislands_common
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lislands_common
else:unix: LIBS += -L$$OUT_PWD/../common/ -lislands_common

INCLUDEPATH += $$PWD/../common
DEPENDPATH += $$PWD/../common
