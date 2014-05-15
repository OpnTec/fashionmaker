#-------------------------------------------------
#
# Project created by QtCreator 2014-04-25T12:01:49
#
#-------------------------------------------------

QT       -= gui

TARGET = qmuparser
TEMPLATE = lib

CONFIG -= debug_and_release debug_and_release_target
CONFIG += c++11

DEFINES += QMUPARSER_LIBRARY

# directory for executable file
DESTDIR = bin

# files created moc
MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj

# files created rcc
#RCC_DIR = rcc

SOURCES += \
    qmuparser.cpp \
    qmuparsertokenreader.cpp \
    qmuparsererror.cpp \
    qmuparsercallback.cpp \
    qmuparserbytecode.cpp \
    qmuparserbase.cpp \
    qmuparsertest.cpp \
    stable.cpp

HEADERS += \
    qmuparser.h\
    qmuparser_global.h \
    qmuparsertokenreader.h \
    qmuparsertoken.h \
    qmuparserfixes.h \
    qmuparsererror.h \
    qmuparserdef.h \
    qmuparsercallback.h \
    qmuparserbytecode.h \
    qmuparserbase.h \
    qmuparsertest.h \
    stable.h

VERSION = 2.2.4

unix {
    target.path = /usr/lib
    INSTALLS += target

    QMAKE_CXX = ccache g++
}

CONFIG += precompile_header
# Precompiled headers (PCH)
PRECOMPILED_HEADER = stable.h
win32-msvc* {
    PRECOMPILED_SOURCE = stable.cpp
}

CONFIG(debug, debug|release){
    # Debug
    unix {
        *-g++{
        QMAKE_CXXFLAGS += -isystem "/usr/include/qt5" -isystem "/usr/include/qt5/QtCore" -isystem "$${MOC_DIR}" \
                          #-isystem "$${RCC_DIR}" \
                          -O0 -Wall -Wextra -pedantic -Weffc++ -Woverloaded-virtual -Wctor-dtor-privacy \
                          -Wnon-virtual-dtor -Wold-style-cast -Wconversion -Winit-self -Wstack-protector \
                          -Wunreachable-code -Wcast-align -Wcast-qual -Wdisabled-optimization -Wfloat-equal \
                          -Wformat=2 -Wimport \
                          -Winvalid-pch -Wunsafe-loop-optimizations -Wlong-long -Wmissing-format-attribute \
                          -Wmissing-include-dirs -Wpacked -Wredundant-decls -Winline \
                          -Wswitch-default -Wswitch-enum -Wuninitialized -Wvariadic-macros \
                          -Wlogical-op -Wnoexcept -Wmissing-noreturn -Wpointer-arith\
                          -Wstrict-null-sentinel -Wstrict-overflow=5 -Wundef -Wno-unused -gdwarf-3 \
                          -ftrapv
        }
    } else {
        *-g++{#Don't use additional GCC keys on Windows system.
        QMAKE_CXXFLAGS += -O0 -Wall -Wextra -pedantic
        }
    }
}else{
    # Release
    *-g++{
    QMAKE_CXXFLAGS += -O2
    }

    DEFINES += QT_NO_DEBUG_OUTPUT
}

# Remove generated files at cleaning
QMAKE_DISTCLEAN += \
    $${DESTDIR}/* \
    $${OBJECTS_DIR}/* \
    #$${RCC_DIR}/* \
    $${MOC_DIR}/*

