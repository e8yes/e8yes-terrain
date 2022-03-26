CONFIG -= qt

TEMPLATE = lib
DEFINES += ISLANDS_CONTENT_LIBRARY

CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -DBOOST_LOG_DYN_LINK
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native -g
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../

SOURCES += \
    camera.cc \
    common.cc \
    loader_gltf.cc \
    procedural_basic.cc \
    procedural_object.cc \
    proto/bbox.pb.cc \
    proto/camera.pb.cc \
    proto/entity.pb.cc \
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
    procedural_basic.h \
    procedural_object.h \
    proto/bbox.pb.h \
    proto/camera.pb.h \
    proto/entity.pb.h \
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

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
