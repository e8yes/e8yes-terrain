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
    drawable.cc \
    entity.cc \
    proto/drawable.pb.cc \
    proto/entity.pb.cc \
    proto/physical_shape.pb.cc \
    proto/primitive.pb.cc \
    proto/scene_object.pb.cc \
    proto/scene.pb.cc \
    scene.cc \
    scene_linear.cc

HEADERS += \
    drawable.h \
    entity.h \
    proto/drawable.pb.h \
    proto/entity.pb.h \
    proto/physical_shape.pb.h \
    proto/primitive.pb.h \
    proto/scene_object.pb.h \
    proto/scene.pb.h \
    scene.h \
    scene_linear.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
