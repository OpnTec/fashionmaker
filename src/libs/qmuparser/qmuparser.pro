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
    isEmpty(PREFIX) {
     PREFIX = /usr/lib
    }
    target.path = $$PREFIX/lib
    INSTALLS += target

    *-g++{
        QMAKE_CC = ccache gcc
        QMAKE_CXX = ccache g++
    }
    clang*{
        QMAKE_CC = ccache clang
        QMAKE_CXX = ccache clang++
    }
}

unix:!macx:!clang*{
    CONFIG += precompile_header
    # Precompiled headers (PCH)
    PRECOMPILED_HEADER = stable.h
    win32-msvc* {
        PRECOMPILED_SOURCE = stable.cpp
    }
}

include(../../../Valentina.pri)

CONFIG(debug, debug|release){
    # Debug
    unix {
        *-g++{
        QMAKE_CXXFLAGS += \
            -isystem "$$[QT_INSTALL_HEADERS]" \
            -isystem "$$[QT_INSTALL_HEADERS]/QtWidgets" \
            -isystem "$$[QT_INSTALL_HEADERS]/QtXml" \
            -isystem "$$[QT_INSTALL_HEADERS]/QtGui" \
            -isystem "$$[QT_INSTALL_HEADERS]/QtXmlPatterns" \
            -isystem "$$[QT_INSTALL_HEADERS]/QtCore" \
            -isystem "$${OUT_PWD}/$${MOC_DIR}" \
            $$GCC_CXXFLAGS
        }
        #Turn on Clang warnings
        clang*{
        QMAKE_CXXFLAGS += \
            -isystem "/usr/include/qt5" \
            -isystem "/usr/include/qt5/QtWidgets" \
            -isystem "/usr/include/qt5/QtXml" \
            -isystem "/usr/include/qt5/QtGui" \
            -isystem "/usr/include/qt5/QtCore" \
            -isystem "/usr/include/qt5/QtXmlPatterns" \
            -isystem "$${OUT_PWD}/$${MOC_DIR}" \
            $$CLANG_CXXFLAGS
        }
    } else {
        *-g++{#Don't use additional GCC keys on Windows system.
        QMAKE_CXXFLAGS += -O0 -Wall -Wextra -pedantic
        }
    }

}else{
    # Release
    DEFINES += QT_NO_DEBUG_OUTPUT

    unix:!macx:QMAKE_CXXFLAGS_RELEASE += -g
    unix:!macx{
        # On Linux
        QMAKE_POST_LINK += objcopy --only-keep-debug $(DESTDIR)/$(TARGET) $(DESTDIR)/$(TARGET).debug &&
        QMAKE_POST_LINK += strip --strip-debug --strip-unneeded $(DESTDIR)/$(TARGET) &&
        QMAKE_POST_LINK += objcopy --add-gnu-debuglink $(DESTDIR)/$(TARGET).debug $(DESTDIR)/$(TARGET)
    }
}
