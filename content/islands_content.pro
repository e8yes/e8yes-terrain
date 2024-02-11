CONFIG -= qt
CONFIG += c++17

TEMPLATE = lib
TARGET = content

DEFINES += ISLANDS_CONTENT_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -DBOOST_LOG_DYN_LINK
QMAKE_CXXFLAGS_DEBUG += -Og
QMAKE_CXXFLAGS_DEBUG += -fsanitize=address
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto=auto -march=native
QMAKE_LFLAGS_DEBUG += -fsanitize=address
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto=auto -march=native

INCLUDEPATH += ../

macx {
    INCLUDEPATH += /usr/include
    INCLUDEPATH += /usr/local/include

    QMAKE_LFLAGS += -L/usr/lib
    QMAKE_LFLAGS += -L/usr/local/lib
}

SOURCES += \
    camera.cc \
    common.cc \
    loader_gltf.cc \
    loader_gltf_geometry.cc \
    loader_gltf_material.cc \
    procedural_basic.cc \
    procedural_object.cc \
    proto/bbox.pb.cc \
    proto/camera.pb.cc \
    proto/entity.pb.cc \
    proto/light_source.pb.cc \
    proto/lod.pb.cc \
    proto/procedural_object.pb.cc \
    proto/procedural_shape.pb.cc \
    proto/scene_object.pb.cc \
    proto/scene.pb.cc \
    proto/transform.pb.cc \
    scene.cc \
    scene_entity.cc \
    scene_object.cc \
    structure.cc \
    structure_linear.cc

HEADERS += \
    camera.h \
    common.h \
    loader_gltf.h \
    loader_gltf_geometry.h \
    loader_gltf_material.h \
    procedural_basic.h \
    procedural_object.h \
    proto/bbox.pb.h \
    proto/camera.pb.h \
    proto/entity.pb.h \
    proto/light_source.pb.h \
    proto/lod.pb.h \
    proto/procedural_object.pb.h \
    proto/procedural_shape.pb.h \
    proto/scene_object.pb.h \
    proto/scene.pb.h \
    proto/transform.pb.h \
    scene.h \
    scene_entity.h \
    scene_object.h \
    structure.h \
    structure_linear.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../resource/release/ -lresource
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../resource/debug/ -lresource
else:unix: LIBS += -L$$OUT_PWD/../resource/ -lresource

INCLUDEPATH += $$PWD/../resource
DEPENDPATH += $$PWD/../resource

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../third_party/tiny_gltf/release/ -ltiny_gltf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../third_party/tiny_gltf/debug/ -ltiny_gltf
else:unix: LIBS += -L$$OUT_PWD/../third_party/tiny_gltf/ -ltiny_gltf

INCLUDEPATH += $$PWD/../third_party/tiny_gltf
DEPENDPATH += $$PWD/../third_party/tiny_gltf

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../third_party/uuid/release/ -luuid4
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../third_party/uuid/debug/ -luuid4
else:unix: LIBS += -L$$OUT_PWD/../third_party/uuid/ -luuid4

INCLUDEPATH += $$PWD/../third_party/uuid
DEPENDPATH += $$PWD/../third_party/uuid

win32 {
    LIBS += -lboost_log-mt
}

unix:!macx {
    LIBS += -lboost_log
}

macx {
    LIBS += -lboost_log-mt
}

LIBS += -lprotobuf

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
