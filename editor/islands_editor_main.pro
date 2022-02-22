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
    component_editor_context.cc \
    component_environment.cc \
    component_scene.cc \
    main.cc \
    window_display.cc \
    window_editor.cc

HEADERS += \
    component_editor_context.h \
    component_environment.h \
    component_scene.h \
    window_display.h \
    window_editor.h

FORMS += \
    window_editor.ui

TRANSLATIONS += \
    e8terrain_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Dependencies
# Islands procedural generator
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../procedural/release/ -lislands_procedural
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../procedural/debug/ -lislands_procedural
else:unix: LIBS += -L$$OUT_PWD/../procedural/ -lislands_procedural

INCLUDEPATH += $$PWD/../procedural
DEPENDPATH += $$PWD/../procedural

# Islands renderer
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../renderer/release/ -lislands_renderer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../renderer/debug/ -lislands_renderer
else:unix: LIBS += -L$$OUT_PWD/../renderer/ -lislands_renderer

INCLUDEPATH += $$PWD/../renderer
DEPENDPATH += $$PWD/../renderer

# Islands content.
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../content/release/ -lislands_content
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../content/debug/ -lislands_content
else:unix: LIBS += -L$$OUT_PWD/../content/ -lislands_content

INCLUDEPATH += $$PWD/../content
DEPENDPATH += $$PWD/../content

# Islands common
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lislands_common
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lislands_common
else:unix: LIBS += -L$$OUT_PWD/../common/ -lislands_common

INCLUDEPATH += $$PWD/../common
DEPENDPATH += $$PWD/../common

# Third party VMA.
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../third_party/vma/release/ -lislands_common
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../third_party/vma/debug/ -lislands_common
else:unix: LIBS += -L$$OUT_PWD/../third_party/vma/ -lvma

INCLUDEPATH += $$PWD/../common
DEPENDPATH += $$PWD/../third_party/vma

# Third party UUID.
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../third_party/uuid/release/ -lislands_common
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../third_party/uuid/debug/ -lislands_common
else:unix: LIBS += -L$$OUT_PWD/../third_party/uuid/ -luuid4

INCLUDEPATH += $$PWD/../common
DEPENDPATH += $$PWD/../third_party/uuid

LIBS += -lboost_log
LIBS += -lboost_thread
LIBS += -lSDL2
LIBS += -lvulkan
