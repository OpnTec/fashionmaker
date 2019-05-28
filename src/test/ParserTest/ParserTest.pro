#-------------------------------------------------
#
# Project created by QtCreator 2014-05-21T20:43:38
#
#-------------------------------------------------

# Build QMuParser tests.

# File with common stuff for whole project
include(../../../common.pri)

# We use many core functions.
QT       += core

# Consol application doesn't need gui.
QT       -= gui

# Name of binary file.
TARGET = ParserTest

# Console application.
CONFIG   += console

# Since Q5.12 available support for C++17
equals(QT_MAJOR_VERSION, 5):greaterThan(QT_MINOR_VERSION, 11) {
    CONFIG += c++17
} else {
    CONFIG += c++14
}

# CONFIG += testcase adds a  'make check' which is great. But by default it also
# adds a 'make install' that installs the test cases, which we do not want.
# Can configure it not to do that with 'no_testcase_installs'
CONFIG += testcase no_testcase_installs

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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
    main.cpp

*msvc*:SOURCES += stable.cpp

# Set using ccache. Function enable_ccache() defined in common.pri.
$$enable_ccache()

include(warnings.pri)

CONFIG(release, debug|release){
    # Release mode
    !*msvc*:CONFIG += silent
    DEFINES += QT_NO_DEBUG_OUTPUT

    noDebugSymbols{ # For enable run qmake with CONFIG+=noDebugSymbols
        # do nothing
    } else {
        # Turn on debug symbols in release mode on Unix systems.
        # On Mac OS X temporarily disabled. Need find way how to strip binary file.
        !macx:!*msvc*:QMAKE_CXXFLAGS_RELEASE += -g -gdwarf-3
    }
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/qmuparser/bin/ -lqmuparser2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/qmuparser/bin/ -lqmuparser2
else:unix: LIBS += -L$$OUT_PWD/../../libs/qmuparser/bin/ -lqmuparser

INCLUDEPATH += $$PWD/../../libs/qmuparser
DEPENDPATH += $$PWD/../../libs/qmuparser

noDebugSymbols{ # For enable run qmake with CONFIG+=noDebugSymbols
    # do nothing
} else {
    noStripDebugSymbols { # For enable run qmake with CONFIG+=noStripDebugSymbols
        # do nothing
    } else {
        # Strip after you link all libaries.
        CONFIG(release, debug|release){
            unix:!macx{
                # Strip debug symbols.
                QMAKE_POST_LINK += objcopy --only-keep-debug $(TARGET) $(TARGET).debug &&
                QMAKE_POST_LINK += strip --strip-debug --strip-unneeded $(TARGET) &&
                QMAKE_POST_LINK += objcopy --add-gnu-debuglink $(TARGET).debug $(TARGET)

                QMAKE_DISTCLEAN += bin/${TARGET}.dbg
            }
        }
    }
}
