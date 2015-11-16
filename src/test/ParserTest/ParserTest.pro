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

# Console application, we use C++11 standard.
CONFIG   += console c++11

# CONFIG += testcase adds a  'make check' which is great. But by default it also
# adds a 'make install' that installs the test cases, which we do not want.
# Can configure it not to do that with 'no_testcase_installs'
CONFIG += testcase no_testcase_installs

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

# Set using ccache. Function enable_ccache() defined in common.pri.
$$enable_ccache()

CONFIG(debug, debug|release){
    # Debug mode
    unix {
        #Turn on compilers warnings.
        *-g++{
            QMAKE_CXXFLAGS += \
                $$GCC_DEBUG_CXXFLAGS # See common.pri for more details.

            noAddressSanitizer{ # For enable run qmake with CONFIG+=noAddressSanitizer
                # do nothing
            } else {
                #gcc’s 4.8.0 Address Sanitizer
                #http://blog.qt.digia.com/blog/2013/04/17/using-gccs-4-8-0-address-sanitizer-with-qt/
                QMAKE_CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer
                QMAKE_CFLAGS += -fsanitize=address -fno-omit-frame-pointer
                QMAKE_LFLAGS += -fsanitize=address
            }
        }
        clang*{
        QMAKE_CXXFLAGS += \
            $$CLANG_DEBUG_CXXFLAGS # See common.pri for more details.

        # -isystem key works only for headers. In some cases it's not enough. But we can't delete this warnings and
        # want them in global list. Compromise decision delete them from local list.
        QMAKE_CXXFLAGS -= \
            -Wmissing-prototypes
        }
        *-icc-*{
            QMAKE_CXXFLAGS += \
                $$ICC_DEBUG_CXXFLAGS
        }
    } else {
        *-g++{
        QMAKE_CXXFLAGS += $$GCC_DEBUG_CXXFLAGS # See common.pri for more details.
        }
    }

}else{
    # Release mode
    !win32-msvc*:CONFIG += silent
    DEFINES += QT_NO_DEBUG_OUTPUT

    noDebugSymbols{ # For enable run qmake with CONFIG+=noDebugSymbols
        # do nothing
    } else {
        # Turn on debug symbols in release mode on Unix systems.
        # On Mac OS X temporarily disabled. Need find way how to strip binary file.
        !macx:!win32-msvc*:QMAKE_CXXFLAGS_RELEASE += -g -gdwarf-3
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
