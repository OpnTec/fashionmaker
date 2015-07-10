#-------------------------------------------------
#
# Project created by QtCreator 2015-06-16T18:05:56
#
#-------------------------------------------------

# File with common stuff for whole project
include(../../../common.pri)

QT       += widgets xml

# Name of the library
TARGET = vtools

# We want create library
TEMPLATE = lib

CONFIG += \
    staticlib \# Making static library
    c++11 # We use C++11 standard

# Use out-of-source builds (shadow builds)
CONFIG -= debug_and_release debug_and_release_target

# Since Qt 5.4.0 the source code location is recorded only in debug builds.
# We need this information also in release builds. For this need define QT_MESSAGELOGCONTEXT.
DEFINES += QT_MESSAGELOGCONTEXT

include(vtools.pri)

# This is static library so no need in "make install"

# directory for executable file
DESTDIR = bin

# files created moc
MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj

# Directory for files created rcc
#RCC_DIR = rcc

# Directory for files created uic
UI_DIR = uic

# Set using ccache. Function enable_ccache() defined in common.pri.
$$enable_ccache()

# Set precompiled headers. Function set_PCH() defined in common.pri.
$$set_PCH()

INCLUDEPATH += $$PWD/../vpatterndb

CONFIG(debug, debug|release){
    # Debug mode
    unix {
        #Turn on compilers warnings.
        *-g++{
            QMAKE_CXXFLAGS += \
                # Key -isystem disable checking errors in system headers.
                -isystem "$${OUT_PWD}/$${UI_DIR}" \
                -isystem "$${OUT_PWD}/$${MOC_DIR}" \
#                -isystem "$${OUT_PWD}/$${RCC_DIR}" \
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
            # Key -isystem disable checking errors in system headers.
            -isystem "$${OUT_PWD}/$${MOC_DIR}" \
            $$CLANG_DEBUG_CXXFLAGS # See common.pri for more details.
        }
    } else {
        *-g++{
            QMAKE_CXXFLAGS += $$GCC_DEBUG_CXXFLAGS # See common.pri for more details.
        }
    }

}else{
    # Release mode
    DEFINES += V_NO_ASSERT
    !unix:*-g++{
        QMAKE_CXXFLAGS += -fno-omit-frame-pointer # Need for exchndl.dll
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
