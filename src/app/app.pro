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
include(undocommands/undocommands.pri)
include(visualization/visualization.pri)

INCLUDEPATH += "$${PWD}/widgets"

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
    share/resources/valentina.exe.manifest \
    share/resources/icon/64x64/icon64x64.ico

TRANSLATIONS += share/translations/valentina.ts \
                share/translations/valentina_ru_RU.ts \
                share/translations/valentina_uk_UA.ts \
                share/translations/valentina_de_DE.ts \
                share/translations/valentina_cs_CZ.ts \
                share/translations/valentina_he_IL.ts \
                share/translations/valentina_fr_FR.ts \
                share/translations/valentina_it_IT.ts \
                share/translations/valentina_nl_NL.ts

unix {
    *-g++{
        QMAKE_CXX = ccache g++
    }
    clang*{
        QMAKE_CXX = ccache clang++
    }
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
        QMAKE_CXXFLAGS += \
            -isystem "/usr/include/qt5" \
            -isystem "/usr/include/qt5/QtWidgets" \
            -isystem "/usr/include/qt5/QtXml" \
            -isystem "/usr/include/qt5/QtGui" \
            -isystem "/usr/include/qt5/QtXmlPatterns" \
            -isystem "/usr/include/qt5/QtCore" \
            -isystem "$${OUT_PWD}/$${UI_DIR}" \
            -isystem "$${OUT_PWD}/$${MOC_DIR}" \
            -isystem "$${OUT_PWD}/$${RCC_DIR}" \
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
            -isystem "$${OUT_PWD}/$${UI_DIR}" \
            -isystem "$${OUT_PWD}/$${MOC_DIR}" \
            -isystem "$${OUT_PWD}/$${RCC_DIR}" \
            $$CLANG_CXXFLAGS
        }
    } else {
        *-g++{#Don't use additional GCC keys on Windows system.
        QMAKE_CXXFLAGS += -O0 -Wall -Wextra -pedantic
        }
    }

    #Calculate revision number only in release mode. Change revision number each time requare recompilation
    #precompiled headers file.
    DEFINES += "LOC_REV=0"
}else{
    # Release
    DEFINES += QT_NO_DEBUG_OUTPUT

    QMAKE_CXXFLAGS_RELEASE += -g

    #local revision number for using in version
    HG_REV=$$system(hg parents --template '{rev}')
    isEmpty(HG_REV){
        HG_REV = 0
    }
    DEFINES += "LOC_REV=$${HG_REV}"
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

INSTALL_TRANSLATIONS += \
    share/translations/valentina_ru_RU.qm \
    share/translations/valentina_uk_UA.qm \
    share/translations/valentina_de_DE.qm \
    share/translations/valentina_cs_CZ.qm \
    share/translations/valentina_he_IL.qm \
    share/translations/valentina_fr_FR.qm \
    share/translations/valentina_it_IT.qm \
    share/translations/valentina_nl_NL.qm

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
desktop.files += ../../dist/$${TARGET}.desktop
pixmaps.path = $$DATADIR/pixmaps/
pixmaps.files += ../../dist/$${TARGET}.png
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

      system($$shell_path($$[QT_INSTALL_BINS]/lrelease) $$shell_path($${PWD}/$$_translation_name) -qm $$shell_path($${PWD}/$$_translation_name_qm))
    }
}

for(DIR, INSTALL_TRANSLATIONS) {
     #add these absolute paths to a variable which
     #ends up as 'mkcommands = path1 path2 path3 ...'
     tr_path += $${PWD}/$$DIR
}

copyToDestdir($$tr_path, $$shell_path($${OUT_PWD}/$$DESTDIR/translations))

for(DIR, INSTALL_STANDARD_MEASHUREMENTS) {
     #add these absolute paths to a variable which
     #ends up as 'mkcommands = path1 path2 path3 ...'
     st_path += $${PWD}/$$DIR
}

copyToDestdir($$st_path, $$shell_path($${OUT_PWD}/$$DESTDIR/tables/standard))

win32:CONFIG(release, debug|release): LIBS += -L$${OUT_PWD}/../libs/qmuparser/$${DESTDIR} -lqmuparser2
else:win32:CONFIG(debug, debug|release): LIBS += -L$${OUT_PWD}/../libs/qmuparser/$${DESTDIR} -lqmuparser2
else:unix: LIBS += -L$${OUT_PWD}/../libs/qmuparser/$${DESTDIR} -lqmuparser

INCLUDEPATH += $${PWD}/../libs/qmuparser
DEPENDPATH += $${PWD}/../libs/qmuparser

win32:CONFIG(release, debug|release): LIBS += -L$${OUT_PWD}/../libs/vpropertyexplorer/$${DESTDIR} -lvpropertyexplorer
else:win32:CONFIG(debug, debug|release): LIBS += -L$${OUT_PWD}/../libs/vpropertyexplorer/$${DESTDIR} -lvpropertyexplorer
else:unix: LIBS += -L$${OUT_PWD}/../libs/vpropertyexplorer/$${DESTDIR} -lvpropertyexplorer

INCLUDEPATH += $${PWD}/../libs/vpropertyexplorer
DEPENDPATH += $${PWD}/../libs/vpropertyexplorer

CONFIG(release, debug|release){
    unix:QMAKE_POST_LINK += objcopy --only-keep-debug $(TARGET) $(TARGET).debug &&
    unix:QMAKE_POST_LINK += strip --strip-debug --strip-unneeded $(TARGET) &&
    unix:QMAKE_POST_LINK += objcopy --add-gnu-debuglink $(TARGET).debug $(TARGET)
}
