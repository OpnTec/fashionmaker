#-------------------------------------------------
#
# Project created by QtCreator 2013-06-18T12:36:43
#
#-------------------------------------------------

# Compilation main binary file

# File with common stuff for whole project
include(../../Valentina.pri)

# Here we don't see "network" library, but, i think, "printsupport" depend on this library, so we still need this
# library in installer.
QT       += core gui widgets xml svg printsupport xmlpatterns

# We want create executable file
TEMPLATE = app

# Name of binary file
TARGET = valentina

# Use out-of-source builds (shadow builds)
CONFIG -= debug_and_release debug_and_release_target

# We use C++11 standard
CONFIG += c++11

# Directory for executable file
DESTDIR = bin

# Directory for files created moc
MOC_DIR = moc

# Directory for objecs files
OBJECTS_DIR = obj

# Directory for files created rcc
RCC_DIR = rcc

# Directory for files created uic
UI_DIR = uic

# Suport subdirectories. Just better project code tree.
include(container/container.pri)
include(dialogs/dialogs.pri)
include(exception/exception.pri)
include(geometry/geometry.pri)
include(tools/tools.pri)
include(widgets/widgets.pri)
include(xml/xml.pri)
include(undocommands/undocommands.pri)
include(visualization/visualization.pri)
include(core/core.pri)

# This include path help promoute VMainGraphicsView on main window. Without it compiler can't find path to custom view
INCLUDEPATH += "$${PWD}/widgets"

# Some source files
SOURCES += \
        main.cpp \
        mainwindow.cpp \
        tablewindow.cpp \
        stable.cpp \
        version.cpp \
        options.cpp

# Some header files
HEADERS  += \
        mainwindow.h \
        options.h \
        tablewindow.h \
        stable.h \
        version.h

# Main forms
FORMS    += \
        mainwindow.ui \
        tablewindow.ui

# Resource files. This files will be included in binary.
RESOURCES += \
    share/resources/icon.qrc \ # All other icons except cursors and Windows theme.
    share/resources/cursor.qrc \ # Tools cursor icons.
    share/resources/theme.qrc \ # Windows theme icons.
    share/resources/schema.qrc \ # Schemas for validation xml files.
    share/resources/measurements.qrc # For measurements files that we save as resource.

# Compilation will fail without this files after we added them to this section.
OTHER_FILES += \
    share/resources/valentina.rc \ # For Windows system.
    share/resources/icon/64x64/icon64x64.ico # Valentina's logo.

# Add here path to new translation file with name "valentina_*_*.ts" if you want to add new language.
# Same paths in variable INSTALL_TRANSLATIONS.

# File valentina.ts we use in transifex.com. It is empty translation file only with english inside. transifex.com use
# this file like base for new language. Don't add path to valentina.ts to INSTALL_TRANSLATIONS variable.
# When adding a translation here, also add it in the macx part.
TRANSLATIONS += share/translations/valentina.ts \
                share/translations/valentina_ru_RU.ts \
                share/translations/valentina_uk_UA.ts \
                share/translations/valentina_de_DE.ts \
                share/translations/valentina_cs_CZ.ts \
                share/translations/valentina_he_IL.ts \
                share/translations/valentina_fr_FR.ts \
                share/translations/valentina_it_IT.ts \
                share/translations/valentina_nl_NL.ts

# Set using ccache. Function enable_ccache() defined in Valentina.pri.
$$enable_ccache()

# Set precompiled headers. Function set_PCH() defined in Valentina.pri.
$$set_PCH()

CONFIG(debug, debug|release){
    # Debug mode
    unix {
        #Turn on compilers warnings.
        *-g++{
        QMAKE_CXXFLAGS += \
            # Key -isystem disable checking errors in system headers.
            -isystem "$${OUT_PWD}/$${UI_DIR}" \
            -isystem "$${OUT_PWD}/$${MOC_DIR}" \
            -isystem "$${OUT_PWD}/$${RCC_DIR}" \
            $$GCC_DEBUG_CXXFLAGS # See Valentina.pri for more details.
        }
        clang*{
        QMAKE_CXXFLAGS += \
            # Key -isystem disable checking errors in system headers.
            -isystem "$${OUT_PWD}/$${UI_DIR}" \
            -isystem "$${OUT_PWD}/$${MOC_DIR}" \
            -isystem "$${OUT_PWD}/$${RCC_DIR}" \
            $$CLANG_DEBUG_CXXFLAGS # See Valentina.pri for more details.
        }
    } else {
        *-g++{
        QMAKE_CXXFLAGS += $$GCC_DEBUG_CXXFLAGS # See Valentina.pri for more details.
        }
    }

    #Calculate revision number only in release mode. Change revision number each time requare recompilation
    #precompiled headers file.
    DEFINES += "LOC_REV=0"
}else{
    # Release mode
    DEFINES += QT_NO_DEBUG_OUTPUT

    # Turn on debug symbols in release mode on Unix systems.
    # On Mac OS X temporarily disabled. Need find way how to strip binary file.
    unix:!macx:QMAKE_CXXFLAGS_RELEASE += -g -gdwarf-3

    #local revision number for using in version
    HG_REV=$$system(hg parents --template '{rev}')
    isEmpty(HG_REV){
        HG_REV = 0 # if we can't find local revision left 0.
    }
    DEFINES += "LOC_REV=$${HG_REV}" # Make available local revision number in sources.
}

# Some extra information about Qt. Can be usefull.
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

# Path to recource file.
win32:RC_FILE = share/resources/valentina.rc

# Set "make install" command for Unix-like systems.
unix{
isEmpty(PREFIX){
    PREFIX = $$DEFAULT_PREFIX
}

# Keep path to all files with standard measurements we support right now
INSTALL_STANDARD_MEASHUREMENTS += share/resources/tables/standard/GOST_man_ru.vst

# Prefix for binary file.
unix:!macx{
    # Add to this variable all translation files that you want install with program.
    # For generation *.qm file first you need create *.ts. See section TRANSLATIONS.
    INSTALL_TRANSLATIONS += \
        share/translations/valentina_ru_RU.qm \
        share/translations/valentina_uk_UA.qm \
        share/translations/valentina_de_DE.qm \
        share/translations/valentina_cs_CZ.qm \
        share/translations/valentina_he_IL.qm \
        share/translations/valentina_fr_FR.qm \
        share/translations/valentina_it_IT.qm \
        share/translations/valentina_nl_NL.qm



    DATADIR =$$PREFIX/share
    DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"

    # Path to bin file after installation
    target.path = $$PREFIX/bin

    # .desctop file
    desktop.path = $$DATADIR/applications/
    desktop.files += ../../dist/$${TARGET}.desktop

    # logo
    pixmaps.path = $$DATADIR/pixmaps/
    pixmaps.files += ../../dist/$${TARGET}.png

    # Path to translation files after installation
    translations.path = $$DATADIR/$${TARGET}/translations/
    translations.files = $$INSTALL_TRANSLATIONS

    # Path to standard measurement after installation
    standard.path = $$DATADIR/$${TARGET}/tables/standard/
    standard.files = $$INSTALL_STANDARD_MEASHUREMENTS

    INSTALLS += \
        target \
        desktop \
        pixmaps \
        translations \
        standard
    }
}
macx{
    # Some macx stuff
    QMAKE_MAC_SDK = macosx

    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.6
    # Path to resources in app bundle
    RESOURCES_DIR = "Contents/Resources"
    FRAMEWORKS_DIR = "Contents/Frameworks"
    # On macx we will use app bundle. Bundle doesn't need bin directory inside.
    # See issue #166: Creating OSX Homebrew (Mac OS X package manager) formula.
    target.path = $$PREFIX/

    # Copy in bundle translation files.
    exists(share/translations/valentina_ru_RU.qm){
        TRANSLATION_ru_RU.files += \
            share/translations/valentina_ru_RU.qm \
            share/translations/Localizable.strings
        TRANSLATION_ru_RU.path = "$$RESOURCES_DIR/translations/ru_RU.lproj"
        QMAKE_BUNDLE_DATA += TRANSLATION_ru_RU
    }

    exists(share/translations/valentina_uk_UA.qm){
        TRANSLATION_uk_UA.files += \
            share/translations/valentina_uk_UA.qm \
            share/translations/Localizable.strings
        TRANSLATION_uk_UA.path = "$$RESOURCES_DIR/translations/uk_UA.lproj"
        QMAKE_BUNDLE_DATA += TRANSLATION_uk_UA
    }

    exists(share/translations/valentina_de_DE.qm){
        TRANSLATION_de_DE.files += \
            share/translations/valentina_de_DE.qm \
            share/translations/Localizable.strings
        TRANSLATION_de_DE.path = "$$RESOURCES_DIR/translations/de_DE.lproj"
        QMAKE_BUNDLE_DATA += TRANSLATION_de_DE
    }

    exists(share/translations/valentina_cs_CZ.qm){
        TRANSLATION_cs_CZ.files += \
            share/translations/valentina_cs_CZ.qm \
            share/translations/Localizable.strings
        TRANSLATION_cs_CZ.path = "$$RESOURCES_DIR/translations/cs_CZ.lproj"
        QMAKE_BUNDLE_DATA += TRANSLATION_cs_CZ
    }

    exists(share/translations/valentina_he_IL.qm){
        TRANSLATION_he_IL.files += \
            share/translations/valentina_he_IL.qm \
            share/translations/Localizable.strings
        TRANSLATION_he_IL.path = "$$RESOURCES_DIR/translations/he_IL.lproj"
        QMAKE_BUNDLE_DATA += TRANSLATION_he_IL
    }

    exists(share/translations/valentina_fr_FR.qm){
        TRANSLATION_fr_FR.files += \
            share/translations/valentina_fr_FR.qm \
            share/translations/Localizable.strings
        TRANSLATION_fr_FR.path = "$$RESOURCES_DIR/translations/fr_FR.lproj"
        QMAKE_BUNDLE_DATA += TRANSLATION_fr_FR
    }

    exists(share/translations/valentina_it_IT.qm){
        TRANSLATION_it_IT.files += \
            share/translations/valentina_it_IT.qm \
            share/translations/Localizable.strings
        TRANSLATION_it_IT.path = "$$RESOURCES_DIR/translations/it_IT.lproj"
        QMAKE_BUNDLE_DATA += TRANSLATION_it_IT
    }

    exists(share/translations/valentina_nl_NL.qm){
        TRANSLATION_nl_NL.files += \
            share/translations/valentina_nl_NL.qm \
            share/translations/Localizable.strings
        TRANSLATION_nl_NL.path = "$$RESOURCES_DIR/translations/nl_NL.lproj"
        QMAKE_BUNDLE_DATA += TRANSLATION_nl_NL
    }
    frameworks.path = $$FRAMEWORKS_DIR
    frameworks.files += $$files($${OUT_PWD}/../libs/qmuparser/$${DESTDIR}/*)
    frameworks.files += $$files($${OUT_PWD}/../libs/vpropertyexplorer/$${DESTDIR}/*)
    message( framework files $$frameworks.files )
    # logo on macx.
    ICON = ../../dist/Valentina.icns


    # Copy to bundle standard measurements files
    standard.path = $$RESOURCES_DIR/tables/standard/
    standard.files = $$INSTALL_STANDARD_MEASHUREMENTS

    QMAKE_BUNDLE_DATA += \
        standard \
        frameworks


}

# Run generation *.qm file for available *.ts files each time you run qmake.
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

# Make possible run program even you do not install it. Seek files in local directory.
copyToDestdir($$tr_path, $$shell_path($${OUT_PWD}/$$DESTDIR/translations))

for(DIR, INSTALL_STANDARD_MEASHUREMENTS) {
     #add these absolute paths to a variable which
     #ends up as 'mkcommands = path1 path2 path3 ...'
     st_path += $${PWD}/$$DIR
}

copyToDestdir($$st_path, $$shell_path($${OUT_PWD}/$$DESTDIR/tables/standard))

# QMuParser library
win32:CONFIG(release, debug|release): LIBS += -L$${OUT_PWD}/../libs/qmuparser/$${DESTDIR} -lqmuparser2
else:win32:CONFIG(debug, debug|release): LIBS += -L$${OUT_PWD}/../libs/qmuparser/$${DESTDIR} -lqmuparser2
else:unix: LIBS += -L$${OUT_PWD}/../libs/qmuparser/$${DESTDIR} -lqmuparser

INCLUDEPATH += $${PWD}/../libs/qmuparser
DEPENDPATH += $${PWD}/../libs/qmuparser

# VPropertyExplorer library
win32:CONFIG(release, debug|release): LIBS += -L$${OUT_PWD}/../libs/vpropertyexplorer/$${DESTDIR} -lvpropertyexplorer
else:win32:CONFIG(debug, debug|release): LIBS += -L$${OUT_PWD}/../libs/vpropertyexplorer/$${DESTDIR} -lvpropertyexplorer
else:unix: LIBS += -L$${OUT_PWD}/../libs/vpropertyexplorer/$${DESTDIR} -lvpropertyexplorer

INCLUDEPATH += $${PWD}/../libs/vpropertyexplorer
DEPENDPATH += $${PWD}/../libs/vpropertyexplorer


# Strip after you link all libaries.
CONFIG(release, debug|release){
    unix:!macx{
        # Strip debug symbols.
        QMAKE_POST_LINK += objcopy --only-keep-debug $(TARGET) $(TARGET).debug &&
        QMAKE_POST_LINK += strip --strip-debug --strip-unneeded $(TARGET) &&
        QMAKE_POST_LINK += objcopy --add-gnu-debuglink $(TARGET).debug $(TARGET)
    }
}


macx{
   # run macdeployqt to include all qt libraries in packet
   QMAKE_POST_LINK += $$[QT_INSTALL_BINS]/macdeployqt $${OUT_PWD}/$${DESTDIR}/$${TARGET}.app
}
