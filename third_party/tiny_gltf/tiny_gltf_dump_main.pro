CONFIG -= qt
CONFIG += console
CONFIG += c++17

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

INCLUDEPATH += ../../

SOURCES += \
    dump_main.cc

HEADERS += \
    tiny_gltf.h \
    stb_image.h \
    stb_image_write.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/./release/ -ltiny_gltf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/./debug/ -ltiny_gltf
else:unix: LIBS += -L$$OUT_PWD/./ -ltiny_gltf

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../stb_image/release/ -lstb_image
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../stb_image/debug/ -lstb_image
else:unix: LIBS += -L$$OUT_PWD/../stb_image/ -lstb_image

INCLUDEPATH += $$PWD/../stb_image
DEPENDPATH += $$PWD/../stb_image
