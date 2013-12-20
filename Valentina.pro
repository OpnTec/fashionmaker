#-------------------------------------------------
#
# Project created by QtCreator 2013-06-18T12:36:43
#
#-------------------------------------------------

# Use out-of-source builds (shadow builds)

QT       += core gui widgets xml svg

TEMPLATE = app

TARGET = valentina

CONFIG -= debug_and_release debug_and_release_target
CONFIG += c++11

#DEFINES += ...

# directory for executable file
DESTDIR = bin

# files created moc
#MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj

# files created rcc
RCC_DIR = rcc

# files created uic
UI_DIR = uic

include(src/src.pri)
include(src/container/container.pri)
include(src/dialogs/dialogs.pri)
include(src/exception/exception.pri)
include(src/geometry/geometry.pri)
include(src/tools/tools.pri)
include(src/widgets/widgets.pri)
include(src/xml/xml.pri)

RESOURCES += \
    share/resources/icon.qrc \
    share/resources/cursor.qrc

OTHER_FILES += share/resources/valentina.rc \
    share/resources/icon/64x64/icon64x64.ico

TRANSLATIONS += share/translations/valentina_ru.ts \
                share/translations/valentina_uk.ts \
                share/translations/valentina_de.ts
QMAKE_CXX = g++
unix {
QMAKE_CXX = ccache g++
}

CONFIG += precompile_header
# Precompiled headers (PCH)
PRECOMPILED_HEADER = src/stable.h
win32-msvc* {
    PRECOMPILED_SOURCE = src/stable.cpp
}

CONFIG(debug, debug|release){
    # Debug
    *-g++{
    QMAKE_CXXFLAGS += -isystem "/usr/include/qt5" -isystem "/usr/include/qt5/QtWidgets" \
                      -isystem "/usr/include/qt5/QtXml" -isystem "/usr/include/qt5/QtGui" \
                      -isystem "/usr/include/qt5/QtCore" -isystem "$${UI_DIR}" \
# -isystem "$${MOC_DIR}" \
                      -isystem "$${RCC_DIR}" \
                      -Og -Wall -Wextra -pedantic -Weffc++ -Woverloaded-virtual -Wctor-dtor-privacy \
                      -Wnon-virtual-dtor -Wold-style-cast -Wconversion -Winit-self \
                      -Wunreachable-code -Wcast-align -Wcast-qual -Wdisabled-optimization -Wfloat-equal \
                      -Wformat  -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k \
                      -Winvalid-pch -Wunsafe-loop-optimizations -Wlong-long -Wmissing-format-attribute \
                      -Wmissing-include-dirs -Wpacked -Wredundant-decls \
                      -Wswitch-default -Wswitch-enum -Wuninitialized -Wunused-parameter -Wvariadic-macros \
                      -Wlogical-op -Wnoexcept \
                      -Wstrict-null-sentinel -Wstrict-overflow=5 -Wundef -Wno-unused -gdwarf-3
    }
}else{
    # Release
    *-g++{
    QMAKE_CXXFLAGS += -O1
    }

    DEFINES += QT_NO_DEBUG_OUTPUT
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

win32:RC_FILE = share/resources/valentina.rc

# Remove generated files at cleaning
QMAKE_DISTCLEAN += $${DESTDIR}/* \
                   $${OBJECTS_DIR}/* \
                   $${UI_DIR}/* \
#                   $${MOC_DIR}/* \
                   $${RCC_DIR}/*

unix {
#VARIABLES
isEmpty(PREFIX) {
  PREFIX = /usr
}
BINDIR = $$PREFIX/bin
DATADIR =$$PREFIX/share
DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"
#MAKE INSTALL
target.path = $$BINDIR
desktop.path = $$DATADIR/applications/
desktop.files += dist/$${TARGET}.desktop
pixmaps.path = $$DATADIR/pixmaps/
pixmaps.files += dist/$${TARGET}.png
INSTALL_TRANSLATIONS += share/translations/valentina_ru.qm \
                        share/translations/valentina_uk.qm \
                        share/translations/valentina_de.qm \
translations.path = $$DATADIR/$${TARGET}/translations/
translations.files = $$INSTALL_TRANSLATIONS
INSTALLS += target \
    desktop \
    pixmaps \
    translations
}
