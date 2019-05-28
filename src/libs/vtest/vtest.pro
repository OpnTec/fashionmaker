#-------------------------------------------------
#
# Project created by QtCreator 2017-06-01T10:07:06
#
#-------------------------------------------------

# File with common stuff for whole project
include(../../../common.pri)

QT += testlib widgets printsupport

# Name of library
TARGET = vtest

# We want to create a library
TEMPLATE = lib

CONFIG += staticlib # Making static library

# Since Q5.12 available support for C++17
equals(QT_MAJOR_VERSION, 5):greaterThan(QT_MINOR_VERSION, 11) {
    CONFIG += c++17
} else {
    CONFIG += c++14
}

# Use out-of-source builds (shadow builds)
CONFIG -= debug_and_release debug_and_release_target

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Since Qt 5.4.0 the source code location is recorded only in debug builds.
# We need this information also in release builds. For this need define QT_MESSAGELOGCONTEXT.
DEFINES += QT_MESSAGELOGCONTEXT

include(vtest.pri)

# This is static library so no need in "make install"

# directory for executable file
DESTDIR = bin

# files created moc
MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj

# Set using ccache. Function enable_ccache() defined in common.pri.
$$enable_ccache()

include(warnings.pri)

CONFIG(debug, debug|release){
    # Debug mode
    #Calculate latest tag distance and build revision only in release mode. Change number each time requare
    #recompilation precompiled headers file.
    DEFINES += "LATEST_TAG_DISTANCE=0"
}else{
    # Release mode
    !*msvc*:CONFIG += silent
    DEFINES += V_NO_ASSERT
    !unix:*g++*{
        QMAKE_CXXFLAGS += -fno-omit-frame-pointer # Need for exchndl.dll
    }

    noDebugSymbols{ # For enable run qmake with CONFIG+=noDebugSymbols
        # do nothing
    } else {
        !macx:!*msvc*{
            # Turn on debug symbols in release mode on Unix systems.
            # On Mac OS X temporarily disabled. TODO: find way how to strip binary file.
            QMAKE_CXXFLAGS_RELEASE += -g -gdwarf-3
            QMAKE_CFLAGS_RELEASE += -g -gdwarf-3
            QMAKE_LFLAGS_RELEASE =
        }
    }

    macx{
        HG = /usr/local/bin/hg # Can't defeat PATH variable on Mac OS.
    }else {
        HG = hg # All other platforms all OK.
    }
    #latest tag distance number for using in version
    HG_DISTANCE=$$system($${HG} log -r. --template '{latesttagdistance}')
    isEmpty(HG_DISTANCE){
        HG_DISTANCE = 0 # if we can't find local revision left 0.
    }
    message("Latest tag distance:" $${HG_DISTANCE})
    DEFINES += "LATEST_TAG_DISTANCE=$${HG_DISTANCE}" # Make available latest tag distance number in sources.
}

include (../libs.pri)
