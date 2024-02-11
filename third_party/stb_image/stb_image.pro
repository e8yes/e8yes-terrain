CONFIG -= qt
CONFIG += c++17

TEMPLATE = lib
TARGET = stb_image

DEFINES += THIRD_PARTY_STB_IMAGE_LIBRARY
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
    stb_image.cc

HEADERS += \
    stb_image.h \
    stb_image_write.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
