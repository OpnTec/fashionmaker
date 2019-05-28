#-------------------------------------------------
#
# Project created by QtCreator 2014-12-10T17:53:37
#
#-------------------------------------------------

# File with common stuff for whole project
include(../../../common.pri)

# Library work with xml.
QT       += xml xmlpatterns printsupport concurrent

# We don't need gui library.
QT       -= gui

# Name of library
TARGET = ifc # Internal Format Converter

# We want create library
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

include(ifc.pri)

# This is static library so no need in "make install"

# directory for executable file
DESTDIR = bin

# files created moc
MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj

# Directory for files created rcc
RCC_DIR = rcc

# Resource files. This files will be included in binary.
RESOURCES += \
    schema.qrc  # Schemas for validation xml files.

# Set using ccache. Function enable_ccache() defined in common.pri.
$$enable_ccache()

include(warnings.pri)

CONFIG(release, debug|release){
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
}

include (../libs.pri)
