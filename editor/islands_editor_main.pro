QT += core
QT += gui
QT += widgets

CONFIG += c++17

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -DBOOST_LOG_DYN_LINK
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native -g
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

INCLUDEPATH += ../

macx {
    INCLUDEPATH += /usr/include
    INCLUDEPATH += /usr/local/include

    QMAKE_LFLAGS += -L/usr/lib
    QMAKE_LFLAGS += -L/usr/local/lib
}

SOURCES += \
    basic/component_editor_portal_switcher.cc \
    basic/component_modification_monitor.cc \
    basic/component_status.cc \
    basic/context.cc \
    basic/editor_storyline.cc \
    basic/theme.cc \
    display/component_renderer.cc \
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
    basic/theme.h \
    display/component_renderer.h \
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
    display/renderer_depth_parameters.ui \
    display/renderer_light_inputs_parameters.ui \
    display/renderer_radiance_parameters.ui \
    display/renderer_radiosity_parameters.ui \
    display/renderer_solid_color_parameters.ui \
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
# Islands game
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../game/release/ -lgame
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../game/debug/ -lgame
else:unix: LIBS += -L$$OUT_PWD/../game/ -lgame

INCLUDEPATH += $$PWD/../game
DEPENDPATH += $$PWD/../game

# Islands renderer
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../renderer/release/ -lrenderer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../renderer/debug/ -lrenderer
else:unix: LIBS += -L$$OUT_PWD/../renderer/ -lrenderer

INCLUDEPATH += $$PWD/../renderer
DEPENDPATH += $$PWD/../renderer

# Islands content.
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../content/release/ -lcontent
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../content/debug/ -lcontent
else:unix: LIBS += -L$$OUT_PWD/../content/ -lcontent

INCLUDEPATH += $$PWD/../content
DEPENDPATH += $$PWD/../content

# Islands resource.
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../resource/release/ -lresource
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../resource/debug/ -lresource
else:unix: LIBS += -L$$OUT_PWD/../resource/ -lresource

INCLUDEPATH += $$PWD/../resource
DEPENDPATH += $$PWD/../resource

# Islands common
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../common/ -lcommon

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

# Third party STB image.
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../third_party/stb_image/release/ -lstb_image
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../third_party/stb_image/debug/ -lstb_image
else:unix: LIBS += -L$$OUT_PWD/../third_party/stb_image/ -lstb_image

INCLUDEPATH += $$PWD/../third_party/stb_image
DEPENDPATH += $$PWD/../third_party/stb_image

unix:!macx {
    LIBS += -lboost_log
}

macx {
    LIBS += -lboost_log-mt
}

LIBS += -lprotobuf
LIBS += -lSDL2
LIBS += -lvulkan
