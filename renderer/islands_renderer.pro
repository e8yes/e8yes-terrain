CONFIG -= qt
CONFIG += c++17

TEMPLATE = lib
TARGET = renderer

DEFINES += ISLANDS_RENDERER_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -DBOOST_LOG_DYN_LINK
QMAKE_CXXFLAGS_DEBUG += -Og
QMAKE_CXXFLAGS_DEBUG += -fsanitize=address
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto=auto -march=native
QMAKE_LFLAGS_DEBUG += -fsanitize=address
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto=auto -march=native

INCLUDEPATH += ../

macx {
    INCLUDEPATH += /usr/include
    INCLUDEPATH += /usr/local/include

    QMAKE_LFLAGS += -L/usr/lib
    QMAKE_LFLAGS += -L/usr/local/lib
}

SOURCES += \
    basic/attachment.cc \
    basic/command_buffer.cc \
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
    dag/dag_context.cc \
    dag/dag_operation.cc \
    dag/frame_resource_allocator.cc \
    dag/graphics_pipeline.cc \
    dag/graphics_pipeline_output.cc \
    dag/graphics_pipeline_output_common.cc \
    lighting/direct_illuminator.cc \
    dag/promise.cc \
    render_pass/configurator.cc \
    render_pass/rasterize.cc \
    space_projection/project_depth.cc \
    space_projection/project_surface.cc \
    space_screen/depth_projection_visualizer.cc \
    space_screen/exposure.cc \
    space_screen/fill_color.cc \
    space_screen/float_map_visualizer.cc \
    space_screen/fxaa.cc \
    space_screen/gaussian_blur.cc \
    space_screen/radiance.cc \
    space_screen/screen_space_processor.cc \
    space_screen/surface_projection_visualizer.cc \
    space_screen/tone_map.cc \
    proto/renderer.pb.cc \
    drawable/collection.cc \
    drawable/drawable_instance.cc \
    drawable/light_source.cc \
    drawable/query_fn.cc \
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
    basic/command_buffer.h \
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
    dag/dag_context.h \
    dag/dag_operation.h \
    dag/frame_resource_allocator.h \
    dag/graphics_pipeline.h \
    dag/graphics_pipeline_output.h \
    dag/graphics_pipeline_output_common.h \
    dag/promise.h \
    lighting/direct_illuminator.h \
    render_pass/configurator.h \
    render_pass/rasterize.h \
    space_projection/project_depth.h \
    space_projection/project_surface.h \
    space_screen/depth_projection_visualizer.h \
    space_screen/exposure.h \
    space_screen/fill_color.h \
    space_screen/float_map_visualizer.h \
    space_screen/fxaa.h \
    space_screen/gaussian_blur.h \
    space_screen/radiance.h \
    space_screen/screen_space_processor.h \
    space_screen/surface_projection_visualizer.h \
    space_screen/tone_map.h \
    proto/renderer.pb.h \
    drawable/collection.h \
    drawable/drawable_instance.h \
    drawable/light_source.h \
    drawable/query_fn.h \
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

DISTFILES += \
    shader/radiance_deferred.frag

