CONFIG -= qt
CONFIG += c++17

TEMPLATE = lib
TARGET = game

DEFINES += ISLANDS_GAME_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -DBOOST_LOG_DYN_LINK
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native -g
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

INCLUDEPATH += ../

SOURCES += \
    display.cc \
    game.cc \
    storyline.cc \
    task.cc
HEADERS += \
    display.h \
    game.h \
    storyline.h \
    task.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
