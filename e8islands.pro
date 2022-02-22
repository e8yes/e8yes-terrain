TEMPLATE = subdirs

SUBDIRS = \
    third_party/uuid/uuid.pro \
    third_party/vma/vma.pro \
    common/islands_common.pro \
    content/islands_content.pro \
    renderer/islands_renderer.pro \
    procedural/islands_procedural.pro \
    editor/islands_editor_main.pro

CONFIG += ordered
