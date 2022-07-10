CONFIG -= qt
CONFIG += c++17

TEMPLATE = lib
TARGET = renderer

DEFINES += ISLANDS_RENDERER_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -DBOOST_LOG_DYN_LINK
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

INCLUDEPATH += ../

macx {
    INCLUDEPATH += /usr/include
    INCLUDEPATH += /usr/local/include

    QMAKE_LFLAGS += -L/usr/lib
    QMAKE_LFLAGS += -L/usr/local/lib
}

SOURCES += \
    basic/attachment.cc \
    basic/fixed_function.cc \
    basic/frame_buffer.cc \
    basic/mipmap.cc \
    basic/pipeline.cc \
    basic/projection.cc \
    basic/render_pass.cc \
    basic/sampler.cc \
    basic/shader.cc \
    basic/uniform_buffer.cc \
    basic/uniform_layout.cc \
    basic/vertex_input.cc \
    lighting/direct_illuminator.cc \
    output/cached_pipeline.cc \
    output/common_output.cc \
    output/pipeline_output.cc \
    output/pipeline_stage.cc \
    output/promise.cc \
    pass/configurator.cc \
    pass/rasterize.cc \
    pipeline/fill_color.cc \
    pipeline/project_depth.cc \
    pipeline/project_surface.cc \
    postprocessor/depth_projection_visualizer.cc \
    postprocessor/exposure.cc \
    postprocessor/fxaa.cc \
    postprocessor/post_processor.cc \
    postprocessor/radiance.cc \
    postprocessor/surface_projection_visualizer.cc \
    postprocessor/tone_map.cc \
    proto/renderer.pb.cc \
    query/collection.cc \
    query/drawable_instance.cc \
    query/light_source.cc \
    query/query_fn.cc \
    renderer.cc \
    renderer_depth.cc \
    renderer_radiance.cc \
    renderer_solid_color.cc \
    renderer_surface_projection.cc \
    transfer/context.cc \
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
    basic/mipmap.h \
    basic/pipeline.h \
    basic/projection.h \
    basic/render_pass.h \
    basic/sampler.h \
    basic/shader.h \
    basic/uniform_buffer.h \
    basic/uniform_layout.h \
    basic/vertex_input.h \
    lighting/direct_illuminator.h \
    output/cached_pipeline.h \
    output/common_output.h \
    output/pipeline_output.h \
    output/pipeline_stage.h \
    output/promise.h \
    pass/configurator.h \
    pass/rasterize.h \
    pipeline/fill_color.h \
    pipeline/project_depth.h \
    pipeline/project_surface.h \
    postprocessor/depth_projection_visualizer.h \
    postprocessor/exposure.h \
    postprocessor/fxaa.h \
    postprocessor/post_processor.h \
    postprocessor/radiance.h \
    postprocessor/surface_projection_visualizer.h \
    postprocessor/tone_map.h \
    proto/renderer.pb.h \
    query/collection.h \
    query/drawable_instance.h \
    query/light_source.h \
    query/query_fn.h \
    renderer.h \
    renderer_depth.h \
    renderer_radiance.h \
    renderer_solid_color.h \
    renderer_surface_projection.h \
    transfer/context.h \
    transfer/descriptor_set.h \
    transfer/descriptor_set_texture.h \
    transfer/texture_group.h \
    transfer/vram.h \
    transfer/vram_geometry.h \
    transfer/vram_texture.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../content/release/ -lcontent
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../content/debug/ -lcontent
else:unix: LIBS += -L$$OUT_PWD/../content/ -lcontent

INCLUDEPATH += $$PWD/../content
DEPENDPATH += $$PWD/../content

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../resource/release/ -lresource
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../resource/debug/ -lresource
else:unix: LIBS += -L$$OUT_PWD/../resource/ -lresource

INCLUDEPATH += $$PWD/../resource
DEPENDPATH += $$PWD/../resource

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../common/ -lcommon

INCLUDEPATH += $$PWD/../common
DEPENDPATH += $$PWD/../common

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../third_party/vma/release/ -lvma
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../third_party/vma/debug/ -lvma
else:unix: LIBS += -L$$OUT_PWD/../third_party/vma/ -lvma

INCLUDEPATH += $$PWD/../third_party/vma
DEPENDPATH += $$PWD/../third_party/vma

LIBS += -lprotobuf
LIBS += -lvulkan

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
