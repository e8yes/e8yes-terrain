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
    basic/pipeline_common.cc \
    basic/projection.cc \
    output/pipeline_output.cc \
    pass/render_pass.cc \
    pipeline/pipeline_depth_map.cc \
    pipeline/pipeline_light_inputs.cc \
    pipeline/pipeline_solid_color.cc \
    postprocessor/pipeline_depth_map_visualizer.cc \
    postprocessor/pipeline_light_inputs_visualizer.cc \
    postprocessor/pipeline_post_processor.cc \
    proto/renderer.pb.cc \
    query/drawable_instance.cc \
    query/query_fn.cc \
    renderer.cc \
    renderer_depth.cc \
    renderer_light_inputs.cc \
    renderer_solid_color.cc \
    transfer/descriptor_set.cc \
    transfer/descriptor_set_texture.cc \
    transfer/texture_group.cc \
    transfer/vram.cc \
    transfer/vram_geometry.cc \
    transfer/vram_texture.cc

HEADERS += \
    basic/pipeline_common.h \
    basic/projection.h \
    output/pipeline_output.h \
    pass/render_pass.h \
    pipeline/pipeline_depth_map.h \
    pipeline/pipeline_light_inputs.h \
    pipeline/pipeline_solid_color.h \
    postprocessor/pipeline_depth_map_visualizer.h \
    postprocessor/pipeline_light_inputs_visualizer.h \
    postprocessor/pipeline_post_processor.h \
    proto/renderer.pb.h \
    query/drawable_instance.h \
    query/query_fn.h \
    renderer.h \
    renderer_depth.h \
    renderer_light_inputs.h \
    renderer_solid_color.h \
    transfer/descriptor_set.h \
    transfer/descriptor_set_texture.h \
    transfer/texture_group.h \
    transfer/vram.h \
    transfer/vram_geometry.h \
    transfer/vram_texture.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    proto/renderer.proto
