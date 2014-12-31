#-------------------------------------------------
#
# Project created by QtCreator 2014-12-12T14:55:06
#
#-------------------------------------------------

# File with common stuff for whole project
include(../../../Valentina.pri)

# Name of library
TARGET = vobj

# We want create a library
TEMPLATE = lib

CONFIG += \
    staticlib \# Making static library
    c++11 # We use C++11 standard

# Use out-of-source builds (shadow builds)
CONFIG -= debug_and_release debug_and_release_target

# Since Qt 5.4.0 the source code location is recorded only in debug builds.
# We need this information also in release builds. For this need define QT_MESSAGELOGCONTEXT.
DEFINES += QT_MESSAGELOGCONTEXT

include(vobj.pri)

# This is static library so no need in "make install"

# directory for executable file
DESTDIR = bin

# files created moc
MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj

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
    }
}
