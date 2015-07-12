#-------------------------------------------------
#
# Project created by QtCreator 2015-06-15T14:07:14
#
#-------------------------------------------------

# File with common stuff for whole project
include(../../../common.pri)

QT += widgets

# Name of library
TARGET = vmisc

# We want to create a library
TEMPLATE = lib

CONFIG += \
    staticlib \# Making static library
    c++11 # We use C++11 standard

# Use out-of-source builds (shadow builds)
CONFIG -= debug_and_release debug_and_release_target

# Since Qt 5.4.0 the source code location is recorded only in debug builds.
# We need this information also in release builds. For this need define QT_MESSAGELOGCONTEXT.
DEFINES += QT_MESSAGELOGCONTEXT

include(vmisc.pri)

# Resource files. This files will be included in binary.
RESOURCES += \
    share/resources/theme.qrc \ # Windows theme icons.

# This is static library so no need in "make install"

# directory for executable file
DESTDIR = bin

# files created moc
MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj

# Set using ccache. Function enable_ccache() defined in common.pri.
$$enable_ccache()

# Set precompiled headers. Function set_PCH() defined in common.pri.
$$set_PCH()

CONFIG(debug, debug|release){
    # Debug mode
    unix {
        #Turn on compilers warnings.
        *-g++{
            QMAKE_CXXFLAGS += \
                # Key -isystem disable checking errors in system headers.
                -isystem "$${OUT_PWD}/$${MOC_DIR}" \
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

        # -isystem key works only for headers. In some cases it's not enough. But we can't delete these warnings and
        # want them in global list. Compromise decision delete them from local list.
        QMAKE_CXXFLAGS -= \
            -Wmissing-prototypes
        }
    } else {
        *-g++{
            QMAKE_CXXFLAGS += $$GCC_DEBUG_CXXFLAGS # See common.pri for more details.
        }
    }

    #Calculate latest tag distance and build revision only in release mode. Change number each time requare
    #recompilation precompiled headers file.
    DEFINES += "LATEST_TAG_DISTANCE=0"
    DEFINES += "BUILD_REVISION=\\\"unknown\\\""
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

    #build revision number for using in version
    unix {
        HG_HESH=$$system("$${HG} log -r. --template '{node|short}'")
    } else {
        # Use escape character before "|" on Windows
        HG_HESH=$$system($${HG} log -r. --template "{node^|short}")
    }
    isEmpty(HG_HESH){
        HG_HESH = "unknown" # if we can't find build revision left unknown.
    }
    message("Build revision:" $${HG_HESH})
    DEFINES += "BUILD_REVISION=\\\"$${HG_HESH}\\\"" # Make available build revision number in sources.
}
