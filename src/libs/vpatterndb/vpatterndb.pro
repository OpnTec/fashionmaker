#-------------------------------------------------
#
# Project created by QtCreator 2015-06-11T20:46:46
#
#-------------------------------------------------

# File with common stuff for whole project
include(../../../common.pri)

QT += widgets

# Name of the library
TARGET = vpatterndb

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

include(trmeasurements.pri)
include(vpatterndb.pri)

# This is static library so no need in "make install"

# directory for executable file
DESTDIR = bin

# files created moc
MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj

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

    noDebugSymbols{ # For enable run qmake with CONFIG+=noDebugSymbols
        # do nothing
    } else {
        !macx:!win32-msvc*{
            # Turn on debug symbols in release mode on Unix systems.
            # On Mac OS X temporarily disabled. TODO: find way how to strip binary file.
            QMAKE_CXXFLAGS_RELEASE += -g -gdwarf-3
            QMAKE_CFLAGS_RELEASE += -g -gdwarf-3
            QMAKE_LFLAGS_RELEASE =
        }
    }
}

win32-msvc*{
    #VMisc static library
    INCLUDEPATH += $$PWD/../vmisc
}
