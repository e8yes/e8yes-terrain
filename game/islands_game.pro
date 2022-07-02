CONFIG -= qt
CONFIG += c++17

TEMPLATE = lib
TARGET = game

DEFINES += ISLANDS_GAME_LIBRARY
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
    display.cc \
    game.cc \
    storyline.cc \
    task.cc

HEADERS += \
    display.h \
    game.h \
    storyline.h \
    task.h

# Islands renderer
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../renderer/release/ -lrenderer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../renderer/debug/ -lrenderer
else:unix: LIBS += -L$$OUT_PWD/../renderer/ -lrenderer

INCLUDEPATH += $$PWD/../renderer
DEPENDPATH += $$PWD/../renderer

# Islands content.
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../content/release/ -lcontent
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../content/debug/ -lcontent
else:unix: LIBS += -L$$OUT_PWD/../content/ -lcontent

INCLUDEPATH += $$PWD/../content
DEPENDPATH += $$PWD/../content

# Islands resource.
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../resource/release/ -lresource
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../resource/debug/ -lresource
else:unix: LIBS += -L$$OUT_PWD/../resource/ -lresource

INCLUDEPATH += $$PWD/../resource
DEPENDPATH += $$PWD/../resource

# Islands common
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../common/ -lcommon

INCLUDEPATH += $$PWD/../common
DEPENDPATH += $$PWD/../common

LIBS += -lprotobuf
LIBS += -lSDL2

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
