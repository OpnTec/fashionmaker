#-------------------------------------------------
#
# Project created by QtCreator 2014-05-21T20:43:38
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ParserTest
CONFIG   += console c++11
CONFIG   -= app_bundle debug_and_release debug_and_release_target

TEMPLATE = app

# directory for executable file
DESTDIR = bin

# objecs files
OBJECTS_DIR = obj

SOURCES += main.cpp

unix {
QMAKE_CXX = ccache g++
}

CONFIG(debug, debug|release){
    # Debug
    unix {
        *-g++{
        QMAKE_CXXFLAGS += -isystem "/usr/include/qt5" -isystem "/usr/include/qt5/QtCore" \
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
}

# Remove generated files at cleaning
QMAKE_DISTCLEAN += $${DESTDIR}/* \
                   $${OBJECTS_DIR}/*


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/qmuparser/bin/ -lqmuparser2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/qmuparser/bin/ -lqmuparser2
else:unix: LIBS += -L$$OUT_PWD/../../libs/qmuparser/bin/ -lqmuparser

INCLUDEPATH += $$PWD/../../libs/qmuparser
DEPENDPATH += $$PWD/../../libs/qmuparser
