CONFIG -= qt

TARGET = tiny_gltf
TEMPLATE = lib

DEFINES += THIRD_PARTY_TINY_GLTF_LIBRARY

CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native -g
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../../

SOURCES += \
    tiny_gltf.cc

HEADERS += \
    tiny_gltf.h \
    stb_image.h \
    stb_image_write.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
