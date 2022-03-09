CONFIG -= qt

CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native -g
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../../

SOURCES += \
    main.cc

HEADERS += \
    tiny_gltf.h \
    stb_image.h \
    stb_image_write.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/./release/ -ltiny_gltf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/./debug/ -ltiny_gltf
else:unix: LIBS += -L$$OUT_PWD/./ -ltiny_gltf

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
