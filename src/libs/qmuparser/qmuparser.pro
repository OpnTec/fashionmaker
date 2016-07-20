#-------------------------------------------------
#
# Project created by QtCreator 2014-04-25T12:01:49
#
#-------------------------------------------------

# File with common stuff for whole project
include(../../../common.pri)

# We don't need gui library.
QT       -= gui

# Name of library
TARGET = qmuparser

# We want create a library
TEMPLATE = lib

# Use out-of-source builds (shadow builds)
CONFIG -= debug_and_release debug_and_release_target

# We use C++11 standard
CONFIG += c++11

# Since Qt 5.4.0 the source code location is recorded only in debug builds.
# We need this information also in release builds. For this need define QT_MESSAGELOGCONTEXT.
DEFINES += QT_MESSAGELOGCONTEXT

DEFINES += QMUPARSER_LIBRARY

# directory for executable file
DESTDIR = bin

# files created moc
MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj

include(qmuparser.pri)

VERSION = 2.4.1

# Set "make install" command for Unix-like systems.
unix:!macx{
    isEmpty(PREFIX_LIB){
        isEmpty(PREFIX){
            PR_LIB = $$DEFAULT_PREFIX
        } else {
            PR_LIB = $$PREFIX
        }
        contains(QMAKE_HOST.arch, x86_64) {
            PREFIX_LIB = $$PR_LIB/lib64
        } else {
            PREFIX_LIB = $$PR_LIB/lib
        }
    }
    target.path = $$PREFIX_LIB
    INSTALLS += target
}

# Set using ccache. Function enable_ccache() defined in common.pri.
$$enable_ccache()

CONFIG(debug, debug|release){
    # Debug mode
    unix {
        include(warnings.pri)
    } else {
        *-g++{
            QMAKE_CXXFLAGS += $$GCC_DEBUG_CXXFLAGS # See common.pri for more details.
        }
    }

}else{
    # Release mode
    !win32-msvc*:CONFIG += silent

    !unix:*-g++{
        QMAKE_CXXFLAGS += -fno-omit-frame-pointer # Need for exchndl.dll
    }

    checkWarnings{
        unix:include(warnings.pri)
    }

    noStripDebugSymbols {
        # do nothing
    } else {
        !macx:!win32-msvc*{
            noDebugSymbols{ # For enable run qmake with CONFIG+=noDebugSymbols
                # do nothing
            } else {
                # Turn on debug symbols in release mode on Unix systems.
                # On Mac OS X temporarily disabled. TODO: find way how to strip binary file.
                QMAKE_CXXFLAGS_RELEASE += -g -gdwarf-3
                QMAKE_CFLAGS_RELEASE += -g -gdwarf-3
                QMAKE_LFLAGS_RELEASE =

                noStripDebugSymbols { # For enable run qmake with CONFIG+=noStripDebugSymbols
                    # do nothing
                } else {
                    # Strip debug symbols.
                    QMAKE_POST_LINK += objcopy --only-keep-debug bin/${TARGET} bin/${TARGET}.dbg &&
                    QMAKE_POST_LINK += objcopy --strip-debug bin/${TARGET} &&
                    QMAKE_POST_LINK += objcopy --add-gnu-debuglink="bin/${TARGET}.dbg" bin/${TARGET}
                }

                QMAKE_DISTCLEAN += bin/${TARGET}.dbg
            }
        }
    }
}
