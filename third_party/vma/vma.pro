CONFIG -= qt
CONFIG += c++17

TEMPLATE = lib
TARGET = vma

DEFINES += THIRD_PARTY_VMA_LIBRARY
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

macx {
    INCLUDEPATH += /usr/include
    INCLUDEPATH += /usr/local/include

    QMAKE_LFLAGS += -L/usr/lib
    QMAKE_LFLAGS += -L/usr/local/lib
}

SOURCES += \
    vk_mem_alloc.cc

HEADERS += \
    vk_mem_alloc.h

LIBS += -lvulkan

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
