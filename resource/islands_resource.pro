CONFIG -= qt
CONFIG += c++17

TEMPLATE = lib
TARGET = resource

DEFINES += ISLANDS_RESOURCE_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -DBOOST_LOG_DYN_LINK
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native -g
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

INCLUDEPATH += ../

SOURCES += \
    accessor.cc \
    buffer.cc \
    buffer_image.cc \
    buffer_index.cc \
    buffer_vertex.cc \
    common.cc \
    geometry.cc \
    light_map.cc \
    material.cc \
    proto/geometry.pb.cc \
    proto/light_map.pb.cc \
    proto/material.pb.cc \
    proto/physical_shape.pb.cc \
    proto/primitive.pb.cc \
    proto/table.pb.cc \
    proto/texture.pb.cc \
    ram_geometry.cc \
    ram_light_map.cc \
    ram_material.cc \
    table.cc

HEADERS += \
    accessor.h \
    buffer.h \
    buffer_image.h \
    buffer_index.h \
    buffer_vertex.h \
    common.h \
    geometry.h \
    light_map.h \
    material.h \
    proto/geometry.pb.h \
    proto/light_map.pb.h \
    proto/material.pb.h \
    proto/physical_shape.pb.h \
    proto/primitive.pb.h \
    proto/table.pb.h \
    proto/texture.pb.h \
    ram_geometry.h \
    ram_light_map.h \
    ram_material.h \
    table.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
