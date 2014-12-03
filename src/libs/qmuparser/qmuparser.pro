#-------------------------------------------------
#
# Project created by QtCreator 2014-04-25T12:01:49
#
#-------------------------------------------------

# File with common stuff for whole project
include(../../../Valentina.pri)

# We don't need gui library.
QT       -= gui

# Name of library
TARGET = qmuparser

# We want create library
TEMPLATE = lib

# Use out-of-source builds (shadow builds)
CONFIG -= debug_and_release debug_and_release_target

# We use C++11 standard
CONFIG += c++11

DEFINES += QMUPARSER_LIBRARY

# directory for executable file
DESTDIR = bin

# files created moc
MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj

include(qmuparser.pri)

VERSION = 2.2.4

# Set "make install" command for Unix-like systems.
unix:!macx{
    isEmpty(PREFIX){
        contains(QMAKE_HOST.arch, x86_64) {
            PREFIX = $$DEFAULT_PREFIX/lib64
        } else {
            PREFIX = $$DEFAULT_PREFIX/lib
        }
        target.path = $$PREFIX
    } else {
        contains(QMAKE_HOST.arch, x86_64) {
            target.path = $$PREFIX/lib64
        } else {
            target.path = $$PREFIX/lib
        }
    }

    INSTALLS += target
}

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
            # Key -isystem disable checking errors in system headers.
            -isystem "$${OUT_PWD}/$${MOC_DIR}" \
            $$GCC_DEBUG_CXXFLAGS # See Valentina.pri for more details.
        }
        clang*{
        QMAKE_CXXFLAGS += \
            # Key -isystem disable checking errors in system headers.
            -isystem "$${OUT_PWD}/$${MOC_DIR}" \
            $$CLANG_DEBUG_CXXFLAGS # See Valentina.pri for more details.
        }
    } else {
        *-g++{
            QMAKE_CXXFLAGS += $$GCC_DEBUG_CXXFLAGS # See Valentina.pri for more details.
        }
    }

}else{
    # Release mode

    !unix:*-g++{
        QMAKE_CXXFLAGS += -fno-omit-frame-pointer # Need for exchndl.dll
    }

    !macx:!win32-msvc*{
        # Turn on debug symbols in release mode on Unix systems.
        # On Mac OS X temporarily disabled. TODO: find way how to strip binary file.
        QMAKE_CXXFLAGS_RELEASE += -g -gdwarf-3
        QMAKE_CFLAGS_RELEASE += -g -gdwarf-3
        QMAKE_LFLAGS_RELEASE =

        # Strip debug symbols.
        QMAKE_POST_LINK += objcopy --only-keep-debug bin/${TARGET} bin/${TARGET}.dbg &&
        QMAKE_POST_LINK += objcopy --strip-debug bin/${TARGET} &&
        QMAKE_POST_LINK += objcopy --add-gnu-debuglink="bin/${TARGET}.dbg" bin/${TARGET}
    }
}
