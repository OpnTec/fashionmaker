#-------------------------------------------------
#
# Project created by QtCreator 2014-08-26T14:18:08
#
#-------------------------------------------------

# File with common stuff for whole project
include(../../../Valentina.pri)

# Library use widgets
QT       += widgets

# We don't need gui library.
QT       -= gui

# Name of library
TARGET = vpropertyexplorer

# We want create library
TEMPLATE = lib

# We use C++11 standard
CONFIG += c++11

DEFINES += VPROPERTYEXPLORER_LIBRARY

# directory for executable file
DESTDIR = bin

# files created moc
MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj

include(vpropertyexplorer.pri)

# Set "make install" command for Unix-like systems.
unix:!macx{
    isEmpty(PREFIX_LIB){
        contains(QMAKE_HOST.arch, x86_64) {
            PREFIX_LIB = $$DEFAULT_PREFIX/lib64
        } else {
            PREFIX_LIB = $$DEFAULT_PREFIX/lib
        }
    }
    target.path = $$PREFIX_LIB
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

        #gcc’s 4.8.0 Address Sanitizer
        #http://blog.qt.digia.com/blog/2013/04/17/using-gccs-4-8-0-address-sanitizer-with-qt/
        QMAKE_CFLAGS+=-fsanitize=address -fno-omit-frame-pointer
        QMAKE_LFLAGS+=-fsanitize=address
        }
        clang*{
        QMAKE_CXXFLAGS += \
            # Key -isystem disable checking errors in system headers.
            -isystem "$${OUT_PWD}/$${MOC_DIR}" \
            $$CLANG_DEBUG_CXXFLAGS # See Valentina.pri for more details.
        }
    } else {
        *-g++{
        QMAKE_CXXFLAGS += $$CLANG_DEBUG_CXXFLAGS # See Valentina.pri for more details.
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
