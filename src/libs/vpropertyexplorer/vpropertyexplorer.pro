#-------------------------------------------------
#
# Project created by QtCreator 2014-08-26T14:18:08
#
#-------------------------------------------------

# File with common stuff for whole project
include(../../../common.pri)

# Library use widgets
QT       += core widgets

# We don't need gui library.
QT       -= gui

# Name of library
TARGET = vpropertyexplorer

# We want create library
TEMPLATE = lib

# Since Q5.12 available support for C++17
equals(QT_MAJOR_VERSION, 5):greaterThan(QT_MINOR_VERSION, 11) {
    CONFIG += c++17
} else {
    CONFIG += c++14
}

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

DEFINES += VPROPERTYEXPLORER_LIBRARY

# directory for executable file
DESTDIR = bin

# files created moc
MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj

# Allow MAC OS X to find library inside a bundle
macx:QMAKE_SONAME_PREFIX = @rpath

include(vpropertyexplorer.pri)

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

include(warnings.pri)

CONFIG(release, debug|release){
    # Release mode
    !*msvc*:CONFIG += silent

    !unix:*g++*{
        QMAKE_CXXFLAGS += -fno-omit-frame-pointer # Need for exchndl.dll
    }

    checkWarnings{
        unix:include(warnings.pri)
    }

    noStripDebugSymbols {
        # do nothing
    } else {
        !macx:!*msvc*{
            noDebugSymbols{ # For enable run qmake with CONFIG+=noDebugSymbols
                # Strip after you link all libaries.
                QMAKE_POST_LINK += objcopy --strip-debug bin/${TARGET}
            } else {
                # Strip after you link all libaries.
                # Turn on debug symbols in release mode on Unix systems.
                # On Mac OS X temporarily disabled. TODO: find way how to strip binary file.
                QMAKE_CXXFLAGS_RELEASE += -g -gdwarf-3
                QMAKE_CFLAGS_RELEASE += -g -gdwarf-3
                QMAKE_LFLAGS_RELEASE =

                # Strip debug symbols.
                QMAKE_POST_LINK += objcopy --only-keep-debug bin/${TARGET} bin/${TARGET}.dbg &&
                QMAKE_POST_LINK += objcopy --strip-debug bin/${TARGET} &&
                QMAKE_POST_LINK += objcopy --add-gnu-debuglink="bin/${TARGET}.dbg" bin/${TARGET}

                QMAKE_DISTCLEAN += bin/${TARGET}.dbg
            }
        }
    }
}

include (../libs.pri)
