#-------------------------------------------------
#
# Project created by QtCreator 2013-06-18T12:36:43
#
#-------------------------------------------------

# Use out-of-source builds (shadow builds)

include(../../Valentina.pri)

QT       += core gui widgets xml svg printsupport xmlpatterns

TEMPLATE = app

TARGET = valentina

CONFIG -= debug_and_release debug_and_release_target
CONFIG += c++11

#DEFINES += ...

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

include(container/container.pri)
include(dialogs/dialogs.pri)
include(exception/exception.pri)
include(geometry/geometry.pri)
include(tools/tools.pri)
include(widgets/widgets.pri)
include(xml/xml.pri)

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        tablewindow.cpp \
        stable.cpp \
        version.cpp \
    options.cpp

HEADERS  += \
        mainwindow.h \
        options.h \
        tablewindow.h \
        stable.h \
        version.h

FORMS    += \
        mainwindow.ui \
        tablewindow.ui

RESOURCES += \
    share/resources/icon.qrc \
    share/resources/cursor.qrc \
    share/resources/theme.qrc \
    share/resources/schema.qrc \
    share/resources/measurements.qrc

OTHER_FILES += \
    share/resources/valentina.rc \
    share/resources/valentina.manifest \
    share/resources/icon/64x64/icon64x64.ico

TRANSLATIONS += share/translations/valentina.ts \
                share/translations/valentina_ru.ts \
                share/translations/valentina_uk.ts \
                share/translations/valentina_de.ts \
                share/translations/valentina_cs.ts \
                share/translations/valentina_he_IL.ts \
                share/translations/valentina_fr.ts

unix {
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
        QMAKE_CXXFLAGS += -isystem "/usr/include/qt5" -isystem "/usr/include/qt5/QtWidgets" \
                          -isystem "/usr/include/qt5/QtXml" -isystem "/usr/include/qt5/QtGui" \
                          -isystem "/usr/include/qt5/QtCore" -isystem "$${UI_DIR}" -isystem "$${MOC_DIR}" \
                          -isystem "$${RCC_DIR}" \
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

#local revision number for using in version
!system(hg) {
    DEFINES += LOCAL_REVISION=\\\"0\\\"
}
else
{
    DEFINES += LOCAL_REVISION=$$system(hg parents --template '{rev}')
}

win32:RC_FILE = share/resources/valentina.rc

# Remove generated files at cleaning
QMAKE_DISTCLEAN += $${DESTDIR}/* \
                   $${OBJECTS_DIR}/* \
                   $${UI_DIR}/* \
                   $${MOC_DIR}/* \
                   $${RCC_DIR}/* \
                   $$PWD/share/translations/valentina_*.qm

INSTALL_TRANSLATIONS += \
    share/translations/valentina_ru.qm \
    share/translations/valentina_uk.qm \
    share/translations/valentina_de.qm \
    share/translations/valentina_cs.qm \
    share/translations/valentina_he_IL.qm \
    share/translations/valentina_fr.qm

INSTALL_STANDARD_MEASHUREMENTS += share/resources/tables/standard/GOST_man_ru.vst

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
translations.path = $$DATADIR/$${TARGET}/translations/
translations.files = $$INSTALL_TRANSLATIONS
standard.path = $$DATADIR/$${TARGET}/tables/standard/
standard.files = $$INSTALL_STANDARD_MEASHUREMENTS
INSTALLS += \
    target \
    desktop \
    pixmaps \
    translations \
    standard
}

!isEmpty(TRANSLATIONS): {
    for(_translation_name, TRANSLATIONS) {
      _translation_name_qm = $$section(_translation_name,".", 0, 0).qm

      system($$shell_path($$[QT_INSTALL_BINS]/lrelease) $$shell_path($$PWD/$$_translation_name) -qm $$shell_path($$PWD/$$_translation_name_qm))
    }
}

for(DIR, INSTALL_TRANSLATIONS) {
     #add these absolute paths to a variable which
     #ends up as 'mkcommands = path1 path2 path3 ...'
     tr_path += $$PWD/$$DIR
}

copyToDestdir($$tr_path, $$shell_path($$OUT_PWD/$$DESTDIR/translations))

for(DIR, INSTALL_STANDARD_MEASHUREMENTS) {
     #add these absolute paths to a variable which
     #ends up as 'mkcommands = path1 path2 path3 ...'
     st_path += $$PWD/$$DIR
}

copyToDestdir($$st_path, $$shell_path($$OUT_PWD/$$DESTDIR/tables/standard))

win32:CONFIG(release, debug|release): LIBS += -L../libs/qmuparser/bin -lqmuparser2
else:win32:CONFIG(debug, debug|release): LIBS += -L../libs/qmuparser/bin -lqmuparser2
else:unix: LIBS += -L../libs/qmuparser/bin -lqmuparser

INCLUDEPATH += ../libs/qmuparser
DEPENDPATH += ../libs/qmuparser
