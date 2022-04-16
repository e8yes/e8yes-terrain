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
    basic/attachment.cc \
    basic/fixed_function.cc \
    basic/frame_buffer.cc \
    basic/pipeline.cc \
    basic/projection.cc \
    basic/render_pass.cc \
    basic/sampler.cc \
    basic/shader.cc \
    basic/uniform_buffer.cc \
    basic/uniform_layout.cc \
    basic/vertex_input.cc \
    output/pipeline_output.cc \
    output/promise.cc \
    pass/configurator.cc \
    pass/rasterize.cc \
    pipeline/depth_map.cc \
    pipeline/light_inputs.cc \
    pipeline/solid_color.cc \
    postprocessor/depth_map_visualizer.cc \
    postprocessor/light_inputs_visualizer.cc \
    postprocessor/post_processor.cc \
    postprocessor/radiance_directional.cc \
    postprocessor/tone_map.cc \
    proto/renderer.pb.cc \
    query/drawable_instance.cc \
    query/query_fn.cc \
    renderer.cc \
    renderer_depth.cc \
    renderer_light_inputs.cc \
    renderer_radiance.cc \
    renderer_solid_color.cc \
    transfer/descriptor_set.cc \
    transfer/descriptor_set_texture.cc \
    transfer/texture_group.cc \
    transfer/vram.cc \
    transfer/vram_geometry.cc \
    transfer/vram_texture.cc

HEADERS += \
    basic/attachment.h \
    basic/fixed_function.h \
    basic/frame_buffer.h \
    basic/pipeline.h \
    basic/projection.h \
    basic/render_pass.h \
    basic/sampler.h \
    basic/shader.h \
    basic/uniform_buffer.h \
    basic/uniform_layout.h \
    basic/vertex_input.h \
    output/pipeline_output.h \
    output/promise.h \
    pass/configurator.h \
    pass/rasterize.h \
    pipeline/depth_map.h \
    pipeline/light_inputs.h \
    pipeline/solid_color.h \
    postprocessor/depth_map_visualizer.h \
    postprocessor/light_inputs_visualizer.h \
    postprocessor/post_processor.h \
    postprocessor/radiance_directional.h \
    postprocessor/tone_map.h \
    proto/renderer.pb.h \
    query/drawable_instance.h \
    query/query_fn.h \
    renderer.h \
    renderer_depth.h \
    renderer_light_inputs.h \
    renderer_radiance.h \
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
    proto/renderer.proto \
    shader/hdr_aces.frag \
    shader/hdr_clamp.frag
