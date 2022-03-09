TEMPLATE = subdirs

SUBDIRS = \
    third_party/tiny_gltf/tiny_gltf.pro \
    third_party/tiny_gltf/tiny_gltf_dump_main.pro \
    third_party/uuid/uuid.pro \
    third_party/vma/vma.pro \
    common/islands_common.pro \
    content/islands_content.pro \
    renderer/islands_renderer.pro \
    editor/islands_editor_main.pro

CONFIG += ordered
