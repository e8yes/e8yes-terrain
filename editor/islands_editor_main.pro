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
    basic/component_editor_portal_switcher.cc \
    basic/component_modification_monitor.cc \
    basic/component_status.cc \
    basic/context.cc \
    basic/editor_storyline.cc \
    editor.cc \
    environment/component_ambient.cc \
    environment/component_camera.cc \
    object/component_scene_object_gltf.cc \
    procedural/component_procedural_plane.cc \
    project/component_project_closer.cc \
    project/component_project_creator.cc \
    project/component_project_loader.cc \
    project/component_project_saver.cc \
    scene/component_scene_view.cc \
    main.cc

HEADERS += \
    basic/component_editor_portal_switcher.h \
    basic/component_modification_monitor.h \
    basic/component_status.h \
    basic/context.h \
    basic/editor_storyline.h \
    editor.h \
    environment/component_ambient.h \
    environment/component_camera.h \
    object/component_scene_object_gltf.h \
    procedural/component_procedural_plane.h \
    project/component_project_closer.h \
    project/component_project_creator.h \
    project/component_project_loader.h \
    project/component_project_saver.h \
    scene/component_scene_view.h

FORMS += \
    editor_window.ui \
    procedural/component_procedural_plane.ui \
    project/component_project_creator.ui

TRANSLATIONS += \
    e8terrain_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Dependencies
# Game
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../game/release/ -lislands_renderer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../game/debug/ -lislands_renderer
else:unix: LIBS += -L$$OUT_PWD/../game/ -lislands_game

INCLUDEPATH += $$PWD/../game
DEPENDPATH += $$PWD/../game

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

# Islands resource.
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../resource/release/ -lislands_resource
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../resource/debug/ -lislands_resource
else:unix: LIBS += -L$$OUT_PWD/../resource/ -lislands_resource

INCLUDEPATH += $$PWD/../resource
DEPENDPATH += $$PWD/../resource

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

INCLUDEPATH += $$PWD/../third_party/vma
DEPENDPATH += $$PWD/../third_party/vma

# Third party UUID.
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../third_party/uuid/release/ -lislands_common
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../third_party/uuid/debug/ -lislands_common
else:unix: LIBS += -L$$OUT_PWD/../third_party/uuid/ -luuid4

INCLUDEPATH += $$PWD/../third_party/uuid
DEPENDPATH += $$PWD/../third_party/uuid

# Third party Tiny glTF.
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../third_party/tiny_gltf/release/ -lislands_common
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../third_party/tiny_gltf/debug/ -lislands_common
else:unix: LIBS += -L$$OUT_PWD/../third_party/tiny_gltf/ -ltiny_gltf

INCLUDEPATH += $$PWD/../third_party/tiny_gltf
DEPENDPATH += $$PWD/../third_party/tiny_gltf

LIBS += -lboost_log
LIBS += -lboost_thread
LIBS += -lprotobuf
LIBS += -lSDL2
LIBS += -lvulkan
