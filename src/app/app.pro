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
include(app.pri)

# This include path help promoute VMainGraphicsView on main window. Without it compiler can't find path to custom view
INCLUDEPATH += "$${PWD}/widgets"

# Resource files. This files will be included in binary.
RESOURCES += \
    share/resources/icon.qrc \ # All other icons except cursors and Windows theme.
    share/resources/cursor.qrc \ # Tools cursor icons.
    share/resources/theme.qrc \ # Windows theme icons.
    share/resources/measurements.qrc \ # For measurements files that we save as resource.
    share/resources/flags.qrc

# Compilation will fail without this files after we added them to this section.
OTHER_FILES += \
    share/resources/valentina.rc \ # For Windows system.
    share/resources/icon/64x64/icon64x64.ico # Valentina's logo.

# Set using ccache. Function enable_ccache() defined in Valentina.pri.
macx {
    CONFIG(debug, debug|release){
        $$enable_ccache()# Use only in debug mode on Mac
    }
} else {
    $$enable_ccache()
}

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

        #gcc’s 4.8.0 Address Sanitizer
        #http://blog.qt.digia.com/blog/2013/04/17/using-gccs-4-8-0-address-sanitizer-with-qt/
        QMAKE_CFLAGS+=-fsanitize=address -fno-omit-frame-pointer
        QMAKE_LFLAGS+=-fsanitize=address
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

    #Calculate latest tag distance and build revision only in release mode. Change number each time requare
    #recompilation precompiled headers file.
    DEFINES += "LATEST_TAG_DISTANCE=0"
    DEFINES += "BUILD_REVISION=\\\"uknown\\\""
}else{
    # Release mode
    DEFINES += V_NO_ASSERT
    !unix:*-g++{
        QMAKE_CXXFLAGS += -fno-omit-frame-pointer # Need for exchndl.dll
    }

    # Turn on debug symbols in release mode on Unix systems.
    # On Mac OS X temporarily disabled. Need find way how to strip binary file.
    !macx:!win32-msvc*{
        QMAKE_CXXFLAGS_RELEASE += -g -gdwarf-3
        QMAKE_CFLAGS_RELEASE += -g -gdwarf-3
        QMAKE_LFLAGS_RELEASE =
    }

    #latest tag distance number for using in version
    HG_DISTANCE=$$system(hg log -r tip --template '{latesttagdistance}')
    isEmpty(HG_DISTANCE){
        HG_DISTANCE = 0 # if we can't find local revision left 0.
    }
    message("Latest tag distance:" $${HG_DISTANCE})
    DEFINES += "LATEST_TAG_DISTANCE=$${HG_DISTANCE}" # Make available latest tag distance number in sources.

    #build revision number for using in version
    unix {
        HG_HESH=$$system("hg log -r tip --template '{node|short}'")
    } else {
        # Use escape character before "|" on Windows
        HG_HESH=$$system(hg log -r tip --template "{node^|short}")
    }
    isEmpty(HG_HESH){
        HG_HESH = "unknown" # if we can't find build revision left unknown.
    }
    message("Build revision:" $${HG_HESH})
    DEFINES += "BUILD_REVISION=\\\"$${HG_HESH}\\\"" # Make available build revision number in sources.
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

# Keep path to all files with standard measurements we support right now
INSTALL_STANDARD_MEASHUREMENTS += share/resources/tables/standard/GOST_man_ru.vst

TRANSLATIONS_PATH = ../../share/translations

# Add to this variable all translation files that you want install with program.
# For generation *.qm file first you need create *.ts.
# See section TRANSLATIONS in file ../../share/translations/translations.pro.
INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/valentina_ru_RU.qm \
    $${TRANSLATIONS_PATH}/valentina_uk_UA.qm \
    $${TRANSLATIONS_PATH}/valentina_de_DE.qm \
    $${TRANSLATIONS_PATH}/valentina_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/valentina_he_IL.qm \
    $${TRANSLATIONS_PATH}/valentina_fr_FR.qm \
    $${TRANSLATIONS_PATH}/valentina_it_IT.qm \
    $${TRANSLATIONS_PATH}/valentina_nl_NL.qm \
    $${TRANSLATIONS_PATH}/valentina_id_ID.qm

# Set "make install" command for Unix-like systems.
unix{
    # Prefix for binary file.
    isEmpty(PREFIX){
        PREFIX = $$DEFAULT_PREFIX
    }

    unix:!macx{
        DATADIR =$$PREFIX/share
        DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"

        # Path to bin file after installation
        target.path = $$PREFIX/bin

        # .desktop file
        desktop.path = $$DATADIR/applications/
        desktop.files += ../../dist/$${TARGET}.desktop

        # logo
        pixmaps.path = $$DATADIR/pixmaps/
        pixmaps.files += ../../dist/$${TARGET}.png \
                         ../../dist/application-x-valentina-pattern.png

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
        exists($${TRANSLATIONS_PATH}/valentina_ru_RU.qm){
            TRANSLATION_ru_RU.files += \
                $${TRANSLATIONS_PATH}/valentina_ru_RU.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_ru_RU.path = "$$RESOURCES_DIR/translations/ru_RU.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_ru_RU
        }

        exists($${TRANSLATIONS_PATH}/valentina_uk_UA.qm){
            TRANSLATION_uk_UA.files += \
                $${TRANSLATIONS_PATH}/valentina_uk_UA.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_uk_UA.path = "$$RESOURCES_DIR/translations/uk_UA.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_uk_UA
        }

        exists($${TRANSLATIONS_PATH}/valentina_de_DE.qm){
            TRANSLATION_de_DE.files += \
                $${TRANSLATIONS_PATH}/valentina_de_DE.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_de_DE.path = "$$RESOURCES_DIR/translations/de_DE.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_de_DE
        }

        exists($${TRANSLATIONS_PATH}/valentina_cs_CZ.qm){
            TRANSLATION_cs_CZ.files += \
                $${TRANSLATIONS_PATH}/valentina_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_cs_CZ.path = "$$RESOURCES_DIR/translations/cs_CZ.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_cs_CZ
        }

        exists($${TRANSLATIONS_PATH}/valentina_he_IL.qm){
            TRANSLATION_he_IL.files += \
                $${TRANSLATIONS_PATH}/valentina_he_IL.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_he_IL.path = "$$RESOURCES_DIR/translations/he_IL.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_he_IL
        }

        exists($${TRANSLATIONS_PATH}/valentina_fr_FR.qm){
            TRANSLATION_fr_FR.files += \
                $${TRANSLATIONS_PATH}/valentina_fr_FR.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_fr_FR.path = "$$RESOURCES_DIR/translations/fr_FR.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_fr_FR
        }

        exists($${TRANSLATIONS_PATH}/valentina_it_IT.qm){
            TRANSLATION_it_IT.files += \
                $${TRANSLATIONS_PATH}/valentina_it_IT.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_it_IT.path = "$$RESOURCES_DIR/translations/it_IT.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_it_IT
        }

        exists($${TRANSLATIONS_PATH}/valentina_nl_NL.qm){
            TRANSLATION_nl_NL.files += \
                $${TRANSLATIONS_PATH}/valentina_nl_NL.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_nl_NL.path = "$$RESOURCES_DIR/translations/nl_NL.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_nl_NL
        }

        exists($${TRANSLATIONS_PATH}/valentina_id_ID.qm){
            TRANSLATION_id_ID.files += \
                $${TRANSLATIONS_PATH}/valentina_id_ID.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_id_ID.path = "$$RESOURCES_DIR/translations/id_ID.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_id_ID
        }

    qmuparser.path = $$FRAMEWORKS_DIR
    qmuparser.files = $${OUT_PWD}/../libs/qmuparser/$${DESTDIR}/
    qmuparser.files += $${OUT_PWD}/../libs/qmuparser/$${DESTDIR}/libqmuparser.2.dylib
    vpropertyexplorer.path = $$FRAMEWORKS_DIR
    vpropertyexplorer.files = $${OUT_PWD}/../libs/vpropertyexplorer/$${DESTDIR}/


    # logo on macx.
    ICON = ../../dist/Valentina.icns

    # Copy to bundle standard measurements files
    standard.path = $$RESOURCES_DIR/tables/standard/
    standard.files = $$INSTALL_STANDARD_MEASHUREMENTS

    QMAKE_BUNDLE_DATA += \
        standard \
        qmuparser \
        vpropertyexplorer

    }
}

# Some systems use special name for lrelease. For example opensuse 13.2 has lrelease-qt5.
isEmpty(LRELEASE){
    LRELEASE = lrelease
}

# Run generation *.qm file for available *.ts files each time you run qmake.
for(_translation_name, INSTALL_TRANSLATIONS) {
  _translation_name_qm = $$basename(_translation_name)
  _translation_name_ts = $$section(_translation_name_qm, ".", 0, 0).ts

  system($$shell_path($$[QT_INSTALL_BINS]/$$LRELEASE) -removeidentical -nounfinished $$shell_path($${PWD}/$${TRANSLATIONS_PATH}/$$_translation_name_ts) -qm $$shell_path($${PWD}/$$_translation_name))
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

# IFC static library
unix|win32: LIBS += -L$$OUT_PWD/../libs/ifc/$${DESTDIR}/ -lifc

INCLUDEPATH += $$PWD/../libs/ifc
DEPENDPATH += $$PWD/../libs/ifc

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../libs/ifc/$${DESTDIR}/ifc.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../libs/ifc/$${DESTDIR}/libifc.a

# VObj static library
unix|win32: LIBS += -L$$OUT_PWD/../libs/vobj/$${DESTDIR}/ -lvobj

INCLUDEPATH += $$PWD/../libs/vobj
DEPENDPATH += $$PWD/../libs/vobj

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../libs/vobj/$${DESTDIR}/vobj.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../libs/vobj/$${DESTDIR}/libvobj.a


# Strip after you link all libaries.
CONFIG(release, debug|release){
    win32:!win32-msvc*{
        # Strip debug symbols.
        QMAKE_POST_LINK += objcopy --only-keep-debug bin/${TARGET} bin/${TARGET}.dbg &&
        QMAKE_POST_LINK += objcopy --strip-debug bin/${TARGET} &&
        QMAKE_POST_LINK += objcopy --add-gnu-debuglink="bin/${TARGET}.dbg" bin/${TARGET}
    }

    unix:!macx{
        # Strip debug symbols.
        QMAKE_POST_LINK += objcopy --only-keep-debug ${TARGET} ${TARGET}.dbg &&
        QMAKE_POST_LINK += objcopy --strip-debug ${TARGET} &&
        QMAKE_POST_LINK += objcopy --add-gnu-debuglink="${TARGET}.dbg" ${TARGET}
    }
}



macx{
   # run macdeployqt to include all qt libraries in packet
   QMAKE_POST_LINK += $$[QT_INSTALL_BINS]/macdeployqt $${OUT_PWD}/$${DESTDIR}/$${TARGET}.app
}
