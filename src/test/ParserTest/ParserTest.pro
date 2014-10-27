#-------------------------------------------------
#
# Project created by QtCreator 2014-05-21T20:43:38
#
#-------------------------------------------------

# Build QMuParser tests.

# File with common stuff for whole project
include(../../../Valentina.pri)

# We use many core functions.
QT       += core

# Consol application doesn't need gui.
QT       -= gui

# Name of binary file.
TARGET = ParserTest

# Console application, we use C++11 standard.
CONFIG   += console c++11

# Use out-of-source builds (shadow builds)
CONFIG   -= app_bundle debug_and_release debug_and_release_target

# We want create executable file
TEMPLATE = app

# directory for executable file
DESTDIR = bin

# objecs files
OBJECTS_DIR = obj

HEADERS += \
    stable.h

SOURCES += \
    main.cpp \
    stable.cpp

# Set using ccache. Function enable_ccache() defined in Valentina.pri.
$$enable_ccache()

# Set precompiled headers. Function set_PCH() defined in Valentina.pri.
$$set_PCH()

CONFIG(debug, debug|release){
    # Debug mode
    unix {
        #Turn on compilers warnings.
        *-g++{
        QMAKE_CXXFLAGS += \
            $$GCC_DEBUG_CXXFLAGS # See Valentina.pri for more details.
        }
        clang*{
        QMAKE_CXXFLAGS += \
            $$CLANG_DEBUG_CXXFLAGS # See Valentina.pri for more details.
        }
    } else {
        *-g++{
        QMAKE_CXXFLAGS += $$GCC_DEBUG_CXXFLAGS # See Valentina.pri for more details.
        }
    }

}else{
    # Release mode
    DEFINES += QT_NO_DEBUG_OUTPUT

    # Turn on debug symbols in release mode on Unix systems.
    # On Mac OS X temporarily disabled. Need find way how to strip binary file.
    unix:!macx:QMAKE_CXXFLAGS_RELEASE += -g -gdwarf-3
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/qmuparser/bin/ -lqmuparser2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/qmuparser/bin/ -lqmuparser2
else:unix: LIBS += -L$$OUT_PWD/../../libs/qmuparser/bin/ -lqmuparser

INCLUDEPATH += $$PWD/../../libs/qmuparser
DEPENDPATH += $$PWD/../../libs/qmuparser

# Strip after you link all libaries.
CONFIG(release, debug|release){
    unix:!macx{
        # Strip debug symbols.
        QMAKE_POST_LINK += objcopy --only-keep-debug $(TARGET) $(TARGET).debug &&
        QMAKE_POST_LINK += strip --strip-debug --strip-unneeded $(TARGET) &&
        QMAKE_POST_LINK += objcopy --add-gnu-debuglink $(TARGET).debug $(TARGET)
    }
}
