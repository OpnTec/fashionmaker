#-------------------------------------------------
#
# Project created by QtCreator 2013-06-18T12:36:43
#
#-------------------------------------------------

# Use out-of-source builds (shadow builds)

QT       += core gui widgets xml svg

TEMPLATE = app

DEBUG_TARGET = Valentinad
RELEASE_TARGET = Valentina

CONFIG -= debug_and_release debug_and_release_target
CONFIG += c++11 precompile_header

#DEFINES += ...

# Precompiled headers (PCH)
PRECOMPILED_HEADER = src/stable.h
win32-msvc* {
    PRECOMPILED_SOURCE = src/stable.cpp
}

# directory for executable file
DESTDIR = bin

# files created moc
MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj

# files created rcc
RCC_DIR = rcc

# files created uic
UI_DIR = uic

include(src/container/container.pri)
include(src/dialogs/dialogs.pri)
include(src/exception/exception.pri)
include(src/geometry/geometry.pri)
include(src/tools/tools.pri)
include(src/widgets/widgets.pri)
include(src/xml/xml.pri)

SOURCES += src/main.cpp \
        src/mainwindow.cpp \
        src/tablewindow.cpp \
        src/stable.cpp

HEADERS  += src/mainwindow.h \
    src/options.h \
    src/tablewindow.h \
    src/stable.h \
    src/version.h

FORMS    += src/mainwindow.ui \
    src/tablewindow.ui

RESOURCES += \
    share/resources/icon.qrc \
    share/resources/cursor.qrc

TRANSLATIONS += share/translations/valentina_ru.ts \
                share/translations/valentina_uk.ts

CONFIG(debug, debug|release){
    # Debug
    QMAKE_CXX = ccache g++
    TARGET = $$DEBUG_TARGET

    QMAKE_CXXFLAGS += -isystem "/usr/include/qt5" -isystem "/usr/include/qt5/QtWidgets" \
                      -isystem "/usr/include/qt5/QtXml" -isystem "/usr/include/qt5/QtGui" \
                      -isystem "/usr/include/qt5/QtCore" -isystem "$$OUT_PWD/uic" -isystem "$$OUT_PWD/moc/" \
                      -isystem "$$OUT_PWD/rcc/" \
                      -Og -Wall -Wextra -pedantic -Weffc++ -Woverloaded-virtual -Wctor-dtor-privacy \
                      -Wnon-virtual-dtor -Wold-style-cast -Wconversion -Winit-self \
                      -Wunreachable-code -Wcast-align -Wcast-qual -Wdisabled-optimization -Wfloat-equal \
                      -Wformat  -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k \
                      -Winvalid-pch -Wunsafe-loop-optimizations -Wlong-long -Wmissing-format-attribute \
                      -Wmissing-include-dirs -Wpacked -Wredundant-decls \
                      -Wswitch-default -Wswitch-enum -Wuninitialized -Wunused-parameter -Wvariadic-macros \
                      -Wlogical-op -Wnoexcept \
                      -Wstrict-null-sentinel -Wstrict-overflow=5 -Wundef -Wno-unused -gdwarf-3
}else{
    # Release
    TARGET = $$RELEASE_TARGET

    QMAKE_CXXFLAGS += -O1

    DEFINES += QT_NO_DEBUG_OUTPUT

    QMAKE_EXTRA_COMPILERS += lrelease
    lrelease.input         = TRANSLATIONS
    lrelease.output        = ${QMAKE_FILE_BASE}.qm
    lrelease.commands      = $$[QT_INSTALL_BINS]/lrelease ${QMAKE_FILE_IN} -qm "bin/"${QMAKE_FILE_BASE}.qm
    lrelease.CONFIG       += no_link target_predeps
}

message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])
message(Qt resources can be found in the following locations:)
message(Documentation: $$[QT_INSTALL_DOCS])
message(Header files: $$[QT_INSTALL_HEADERS])
message(Libraries: $$[QT_INSTALL_LIBS])
message(Binary files (executables): $$[QT_INSTALL_BINS])
message(Plugins: $$[QT_INSTALL_PLUGINS])
message(Data files: $$[QT_INSTALL_DATA])
message(Translation files: $$[QT_INSTALL_TRANSLATIONS])
message(Settings: $$[QT_INSTALL_SETTINGS])
message(Examples: $$[QT_INSTALL_EXAMPLES])
