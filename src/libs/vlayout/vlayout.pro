#-------------------------------------------------
#
# Project created by QtCreator 2015-01-02T15:39:01
#
#-------------------------------------------------

# File with common stuff for whole project
include(../../../common.pri)

QT += core gui widgets printsupport

# Name of library
TARGET = vlayout

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

include(vlayout.pri)

# This is static library so no need in "make install"

# directory for executable file
DESTDIR = bin

# files created moc
MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj

# Directory for files created rcc
RCC_DIR = rcc

# Set using ccache. Function enable_ccache() defined in common.pri.
$$enable_ccache()

CONFIG(debug, debug|release){
    # Debug mode
    unix {
        #Turn on compilers warnings.
        *-g++{
            QMAKE_CXXFLAGS += \
                # Key -isystem disable checking errors in system headers.
                -isystem "$${OUT_PWD}/$${MOC_DIR}" \
                -isystem "$${OUT_PWD}/$${RCC_DIR}" \
                $$GCC_DEBUG_CXXFLAGS # See common.pri for more details.

            # -isystem key works only for headers. In some cases it's not enough. But we can't delete these warnings and
            # want them in the global list. Compromise decision is to delete them from the local list.
            QMAKE_CXXFLAGS -= \
                -Wlong-long \

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
                -isystem "$${OUT_PWD}/$${RCC_DIR}" \
                $$CLANG_DEBUG_CXXFLAGS # See common.pri for more details.

            # -isystem key works only for headers. In some cases it's not enough. But we can't delete this warnings and
            # want them in global list. Compromise decision delete them from local list.
            QMAKE_CXXFLAGS -= \
                -Wundefined-reinterpret-cast \
                -Wmissing-prototypes # rcc folder
        }
        *-icc-*{
            QMAKE_CXXFLAGS += \
                -isystem "$${OUT_PWD}/$${MOC_DIR}" \
                -isystem "$${OUT_PWD}/$${RCC_DIR}" \
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
    DEFINES += V_NO_ASSERT
    !unix:*-g++{
        QMAKE_CXXFLAGS += -fno-omit-frame-pointer # Need for exchndl.dll
    }

    QMAKE_CXXFLAGS -= -O2 # Disable default optimization level
    QMAKE_CXXFLAGS += -O3 # For vlayout library enable speed optimizations

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

RESOURCES += \
    share/icons.qrc
