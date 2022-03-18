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
    geometry.cc \
    gltf.cc \
    procedural_basic.cc \
    procedural_object.cc \
    proto/camera.pb.cc \
    proto/entity.pb.cc \
    proto/geometry.pb.cc \
    proto/physical_shape.pb.cc \
    proto/primitive.pb.cc \
    proto/procedural_object.pb.cc \
    proto/procedural_shape.pb.cc \
    proto/renderer.pb.cc \
    proto/scene_object.pb.cc \
    proto/scene.pb.cc \
    scene.cc \
    scene_entity.cc \
    scene_object.cc \
    structure.cc \
    structure_linear.cc

HEADERS += \
    camera.h \
    common.h \
    geometry.h \
    gltf.h \
    procedural_basic.h \
    procedural_object.h \
    proto/camera.pb.h \
    proto/entity.pb.h \
    proto/geometry.pb.h \
    proto/physical_shape.pb.h \
    proto/primitive.pb.h \
    proto/procedural_object.pb.h \
    proto/procedural_shape.pb.h \
    proto/renderer.pb.h \
    proto/scene_object.pb.h \
    proto/scene.pb.h \
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
