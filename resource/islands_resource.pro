CONFIG -= qt

TEMPLATE = lib
DEFINES += ISLANDS_RESOURCE_LIBRARY

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
    accessor.cc \
    common.cc \
    geometry.cc \
    material.cc \
    proto/geometry.pb.cc \
    proto/light_map.pb.cc \
    proto/material.pb.cc \
    proto/physical_shape.pb.cc \
    proto/primitive.pb.cc \
    proto/table.pb.cc \
    proto/texture.pb.cc \
    ram_geometry.cc \
    ram_material.cc \
    table.cc

HEADERS += \
    accessor.h \
    common.h \
    geometry.h \
    material.h \
    proto/geometry.pb.h \
    proto/light_map.pb.h \
    proto/material.pb.h \
    proto/physical_shape.pb.h \
    proto/primitive.pb.h \
    proto/table.pb.h \
    proto/texture.pb.h \
    ram_geometry.h \
    ram_material.h \
    table.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
