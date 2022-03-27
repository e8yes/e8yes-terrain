CONFIG -= qt
CONFIG += c++17

TEMPLATE = lib
TARGET = renderer

DEFINES += ISLANDS_RENDERER_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -DBOOST_LOG_DYN_LINK
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native -g
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

INCLUDEPATH += ../

SOURCES += \
    drawable_instance.cc \
    pipeline_common.cc \
    pipeline_depth_map.cc \
    pipeline_depth_map_visualizer.cc \
    pipeline_light_inputs.cc \
    pipeline_output.cc \
    pipeline_post_processor.cc \
    pipeline_solid_color.cc \
    projection.cc \
    proto/renderer.pb.cc \
    query_fn.cc \
    render_pass.cc \
    renderer.cc \
    renderer_depth.cc \
    renderer_solid_color.cc \
    vram_geometry.cc

HEADERS += \
    drawable_instance.h \
    pipeline_common.h \
    pipeline_depth_map.h \
    pipeline_depth_map_visualizer.h \
    pipeline_light_inputs.h \
    pipeline_output.h \
    pipeline_post_processor.h \
    pipeline_solid_color.h \
    projection.h \
    proto/renderer.pb.h \
    query_fn.h \
    render_pass.h \
    renderer.h \
    renderer_depth.h \
    renderer_solid_color.h \
    vram_geometry.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
