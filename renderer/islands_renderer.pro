QT += widgets

TEMPLATE = lib
DEFINES += ISLANDS_RENDERER_LIBRARY

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
    context.cc \
    drawable.cc \
    projection.cc \
    renderer.cc \
    scene.cc \
    vma/vk_mem_alloc.cc \
    vram.cc

HEADERS += \
    context.h \
    drawable.h \
    projection.h \
    renderer.h \
    scene.h \
    vram.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
