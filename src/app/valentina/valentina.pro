#-------------------------------------------------
#
# Project created by QtCreator 2013-06-18T12:36:43
#
#-------------------------------------------------

# Compilation main binary file

# File with common stuff for whole project
include(../../../common.pri)

# Here we don't see "network" library, but, i think, "printsupport" depend on this library, so we still need this
# library in installer.
QT       += core gui widgets xml svg printsupport xmlpatterns

# We want create executable file
TEMPLATE = app

# Name of binary file
macx{
    TARGET = Valentina
} else {
    TARGET = valentina
}

# Use out-of-source builds (shadow builds)
CONFIG -= debug_and_release debug_and_release_target

# Since Q5.4 available support C++14
greaterThan(QT_MAJOR_VERSION, 4):greaterThan(QT_MINOR_VERSION, 3) {
    CONFIG += c++14
} else {
    # We use C++11 standard
    CONFIG += c++11
}

# Since Qt 5.4.0 the source code location is recorded only in debug builds.
# We need this information also in release builds. For this need define QT_MESSAGELOGCONTEXT.
DEFINES += QT_MESSAGELOGCONTEXT

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
include(valentina.pri)

# Resource files. This files will be included in binary.
RESOURCES += \
    share/resources/cursor.qrc \ # Tools cursor icons.
    share/resources/toolicon.qrc

# Compilation will fail without this files after we added them to this section.
OTHER_FILES += \
    share/resources/icon/64x64/icon64x64.ico # Valentina's logo.

# Set using ccache. Function enable_ccache() defined in common.pri.
$$enable_ccache()

include(warnings.pri)

CONFIG(release, debug|release){
    # Release mode
    !*msvc*:CONFIG += silent
    DEFINES += V_NO_ASSERT
    !unix:*g++*{
        QMAKE_CXXFLAGS += -fno-omit-frame-pointer # Need for exchndl.dll
    }

    noDebugSymbols{ # For enable run qmake with CONFIG+=noDebugSymbols
        DEFINES += V_NO_DEBUG
    } else {
        noCrashReports{
            DEFINES += V_NO_DEBUG
        }
        # Turn on debug symbols in release mode on Unix systems.
        # On Mac OS X temporarily disabled. Need find way how to strip binary file.
        !macx:!*msvc*{
            QMAKE_CXXFLAGS_RELEASE += -g -gdwarf-3
            QMAKE_CFLAGS_RELEASE += -g -gdwarf-3
            QMAKE_LFLAGS_RELEASE =
        }
    }
}

DVCS_HESH=$$FindBuildRevision()
message("Build revision:" $${DVCS_HESH})
DEFINES += "BUILD_REVISION=$${DVCS_HESH}" # Make available build revision number in sources.

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

# INSTALL_MULTISIZE_MEASUREMENTS and INSTALL_STANDARD_TEMPLATES inside tables.pri
include(../tables.pri)

win32 {
    INSTALL_PDFTOPS += ../../../dist/win/pdftops.exe
}

include(../translations.pri)

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

        tape.path = $$PREFIX/bin
        tape.files += $${OUT_PWD}/../tape/$${DESTDIR}/tape

        # .desktop file
        desktop.path = /usr/share/applications/
        desktop.files += ../../../dist/$${TARGET}.desktop \
        desktop.files += ../../../dist/tape.desktop

        # logo
        pixmaps.path = /usr/share/pixmaps/
        pixmaps.files += \
            ../../../dist/$${TARGET}.png \
            ../../../dist/tape.png \
            ../../../dist/application-x-valentina-pattern.png \
            ../../../dist/application-x-valentina-i-measurements.png \
            ../../../dist/application-x-valentina-s-measurements.png \

        # Path to translation files after installation
        translations.path = /usr/share/$${TARGET}/translations/
        translations.files = $$INSTALL_TRANSLATIONS

        # Path to multisize measurement after installation
        multisize.path = /usr/share/$${TARGET}/tables/multisize/
        multisize.files = $$INSTALL_MULTISIZE_MEASUREMENTS

        # Path to templates after installation
        templates.path = /usr/share/$${TARGET}/tables/templates/
        templates.files = $$INSTALL_STANDARD_TEMPLATES

        INSTALLS += \
            target \
            tape \
            desktop \
            pixmaps \
            translations \
            multisize \
            templates
    }
    macx{
        # Some macx stuff
        QMAKE_MAC_SDK = macosx

        # Check which minimal OSX version supports current Qt version
        # See page https://doc.qt.io/qt-5/supported-platforms-and-configurations.html
        equals(QT_MAJOR_VERSION, 5):greaterThan(QT_MINOR_VERSION, 7) {# Qt 5.8
            QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
        } else {
            equals(QT_MAJOR_VERSION, 5):greaterThan(QT_MINOR_VERSION, 6) {# Qt 5.7
                QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.8
            } else {
                equals(QT_MAJOR_VERSION, 5):greaterThan(QT_MINOR_VERSION, 3) {# Qt 5.4
                    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7
                } else {
                    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.6
                }
            }
        }

        QMAKE_RPATHDIR += @executable_path/../Frameworks

        # Path to resources in app bundle
        #RESOURCES_DIR = "Contents/Resources" defined in translation.pri
        FRAMEWORKS_DIR = "Contents/Frameworks"
        MACOS_DIR = "Contents/MacOS"
        # On macx we will use app bundle. Bundle doesn't need bin directory inside.
        # See issue #166: Creating OSX Homebrew (Mac OS X package manager) formula.
        target.path = $$MACOS_DIR

        #languages added inside translations.pri

        # Symlinks also good names for copying. Make will take origin file and copy them with using symlink name.
        # For bundle this names more then enough. We don't need care much about libraries versions.
        libraries.path = $$FRAMEWORKS_DIR
        libraries.files += $${OUT_PWD}/../../libs/qmuparser/$${DESTDIR}/libqmuparser.2.dylib
        libraries.files += $${OUT_PWD}/../../libs/vpropertyexplorer/$${DESTDIR}/libvpropertyexplorer.1.dylib

        tape.path = $$MACOS_DIR
        tape.files += $${OUT_PWD}/../tape/$${DESTDIR}/tape.app/$$MACOS_DIR/tape

        # Utility pdftops need for saving a layout image to PS and EPS formates.
        xpdf.path = $$MACOS_DIR
        xpdf.files += $${PWD}/../../../dist/macx/bin64/pdftops

        # logo on macx.
        ICON = ../../../dist/Valentina.icns

        QMAKE_INFO_PLIST = $$PWD/../../../dist/macx/valentina/Info.plist

        # Copy to bundle multisize measurements files
        multisize.path = $$RESOURCES_DIR/tables/multisize/
        multisize.files = $$INSTALL_MULTISIZE_MEASUREMENTS

        # Copy to bundle templates files
        templates.path = $$RESOURCES_DIR/tables/templates/
        templates.files = $$INSTALL_STANDARD_TEMPLATES

        icns_resources.path = $$RESOURCES_DIR/
        icns_resources.files += $$PWD/../../../dist/macx/i-measurements.icns
        icns_resources.files += $$PWD/../../../dist/macx/s-measurements.icns
        icns_resources.files += $$PWD/../../../dist/macx/pattern.icns

        # Copy to bundle multisize measurements files
        # We cannot add none exist files to bundle through QMAKE_BUNDLE_DATA. That's why we must do this manually.
        QMAKE_POST_LINK += $$VCOPY $$quote($${OUT_PWD}/../tape/$${DESTDIR}/tape.app/$$RESOURCES_DIR/diagrams.rcc) $$quote($$shell_path($${OUT_PWD}/$$DESTDIR/$${TARGET}.app/$$RESOURCES_DIR/)) $$escape_expand(\\n\\t)

        QMAKE_BUNDLE_DATA += \
            templates \
            multisize \
            libraries \
            tape \
            xpdf \
            icns_resources
    }
}

# "make install" command for Windows.
# Depend on inno setup script and create installer in folder "package"
win32:*g++* {
    package.path = $${OUT_PWD}/../../../package/valentina
    package.files += \
        $${OUT_PWD}/$${DESTDIR}/valentina.exe \
        $${OUT_PWD}/../tape/$${DESTDIR}/tape.exe \
        $${OUT_PWD}/../tape/$${DESTDIR}/diagrams.rcc \
        $$PWD/../../../dist/win/valentina.ico \
        $$PWD/../../../dist/win/i-measurements.ico \
        $$PWD/../../../dist/win/s-measurements.ico \
        $$PWD/../../../dist/win/pattern.ico \
        $$PWD/../../../dist/win/pdftops.exe \
        $$PWD/../../../dist/win/libeay32.dll \
        $$PWD/../../../dist/win/ssleay32.dll \
        $$PWD/../../../dist/win/msvcr120.dll \
        $$PWD/../../../AUTHORS.txt \
        $$PWD/../../../LICENSE_GPL.txt \
        $$PWD/../../../README.txt \
        $$PWD/../../../ChangeLog.txt \
        $$PWD/../../libs/qmuparser/LICENSE_BSD.txt \
        $${OUT_PWD}/../../libs/qmuparser/$${DESTDIR}/qmuparser2.dll \
        $${OUT_PWD}/../../libs/vpropertyexplorer/$${DESTDIR}/vpropertyexplorer.dll \
        $$[QT_INSTALL_BINS]/icudt*.dll \ # Different name for different Qt releases
        $$[QT_INSTALL_BINS]/icuin*.dll \ # Different name for different Qt releases
        $$[QT_INSTALL_BINS]/icuuc*.dll \ # Different name for different Qt releases
        $$[QT_INSTALL_BINS]/Qt5Core.dll \
        $$[QT_INSTALL_BINS]/Qt5Gui.dll \
        $$[QT_INSTALL_BINS]/Qt5Network.dll \
        $$[QT_INSTALL_BINS]/Qt5PrintSupport.dll \
        $$[QT_INSTALL_BINS]/Qt5Svg.dll \
        $$[QT_INSTALL_BINS]/Qt5Widgets.dll \
        $$[QT_INSTALL_BINS]/Qt5Xml.dll \
        $$[QT_INSTALL_BINS]/Qt5XmlPatterns.dll \
        $$[QT_INSTALL_BINS]/libgcc_s_dw2-1.dll \
        $$[QT_INSTALL_BINS]/libstdc++-6.dll \
        $$[QT_INSTALL_BINS]/libwinpthread-1.dll

    !noDebugSymbols:!noCrashReports{
        package.files += \
            $${OUT_PWD}/$${DESTDIR}/valentina.exe.dbg \
            $${OUT_PWD}/../tape/$${DESTDIR}/tape.exe.dbg \
            $$PWD/../../../dist/win/exchndl.dll \
            $$PWD/../../../dist/win/dbghelp.dll \
            $$PWD/../../../dist/win/mgwhelp.dll \
            $$PWD/../../../dist/win/symsrv.dll \
            $$PWD/../../../dist/win/symsrv.yes \
            $${OUT_PWD}/../../libs/qmuparser/$${DESTDIR}/qmuparser2.dll.dbg \
            $${OUT_PWD}/../../libs/vpropertyexplorer/$${DESTDIR}/vpropertyexplorer.dll.dbg \
            $$PWD/../../../dist/win/curl.exe
    }

    package.CONFIG = no_check_exist
    INSTALLS += package

    package_tables.path = $${OUT_PWD}/../../../package/valentina/tables/multisize
    package_tables.files += $$INSTALL_MULTISIZE_MEASUREMENTS
    INSTALLS += package_tables

    package_templates.path = $${OUT_PWD}/../../../package/valentina/tables/templates
    package_templates.files += $$INSTALL_STANDARD_TEMPLATES
    INSTALLS += package_templates

    package_translations.path = $${OUT_PWD}/../../../package/valentina/translations
    package_translations.files += \
        $$INSTALL_TRANSLATIONS \ # Valentina
        $$[QT_INSTALL_TRANSLATIONS]/qt_ar.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_pl.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_pt.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_ru.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_sk.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_sl.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_sv.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_uk.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_zh_CN.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_zh_TW.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_ca.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_cs.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_da.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_de.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_es.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_en.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_fa.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_fi.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_fr.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_gl.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_he.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_hu.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_it.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_ja.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_ko.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_lt.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_*.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qtbase_*.qm
    INSTALLS += package_translations

    package_bearer.path = $${OUT_PWD}/../../../package/valentina/bearer
    package_bearer.files += \
        $$[QT_INSTALL_PLUGINS]/bearer/qgenericbearer.dll \
        $$[QT_INSTALL_PLUGINS]/bearer/qnativewifibearer.dll
    INSTALLS += package_bearer

    package_iconengines.path = $${OUT_PWD}/../../../package/valentina/iconengines
    package_iconengines.files += $$[QT_INSTALL_PLUGINS]/iconengines/qsvgicon.dll
    INSTALLS += package_iconengines

    package_imageformats.path = $${OUT_PWD}/../../../package/valentina/imageformats
    package_imageformats.files += \
        $$[QT_INSTALL_PLUGINS]/imageformats/qdds.dll \
        $$[QT_INSTALL_PLUGINS]/imageformats/qgif.dll \
        $$[QT_INSTALL_PLUGINS]/imageformats/qicns.dll \
        $$[QT_INSTALL_PLUGINS]/imageformats/qico.dll \
        $$[QT_INSTALL_PLUGINS]/imageformats/qjp2.dll \
        $$[QT_INSTALL_PLUGINS]/imageformats/qjpeg.dll \
        $$[QT_INSTALL_PLUGINS]/imageformats/qmng.dll \
        $$[QT_INSTALL_PLUGINS]/imageformats/qsvg.dll \
        $$[QT_INSTALL_PLUGINS]/imageformats/qtga.dll \
        $$[QT_INSTALL_PLUGINS]/imageformats/qtiff.dll \
        $$[QT_INSTALL_PLUGINS]/imageformats/qwbmp.dll \
        $$[QT_INSTALL_PLUGINS]/imageformats/qwebp.dll \
    INSTALLS += package_imageformats

    package_platforms.path = $${OUT_PWD}/../../../package/valentina/platforms
    package_platforms.files += $$[QT_INSTALL_PLUGINS]/platforms/qwindows.dll
    INSTALLS += package_platforms

    package_printsupport.path = $${OUT_PWD}/../../../package/valentina/printsupport
    package_printsupport.files += $$[QT_INSTALL_PLUGINS]/printsupport/windowsprintersupport.dll
    INSTALLS += package_printsupport

    noWindowsInstaller{ # For enable run qmake with CONFIG+=noWindowsInstaller
        #do nothing
    } else {
        SCP_FOUND = false
        exists("C:/Program Files (x86)/Inno Setup 5/iscc.exe") {
                    INNO_ISCC = "C:/Program Files (x86)/Inno Setup 5/iscc.exe"
                    SCP_FOUND = true
            } else {
                exists("C:/Program Files/Inno Setup 5/iscc.exe") {
                    INNO_ISCC = "C:/Program Files/Inno Setup 5/iscc.exe"
                    SCP_FOUND = true
               }
        }

        if($$SCP_FOUND) {
            package_inno.path = $${OUT_PWD}/../../../package
            package_inno.files += \
                $$PWD/../../../dist/win/inno/LICENSE_VALENTINA \
                $$PWD/../../../dist/win/inno/valentina.iss
            INSTALLS += package_inno

            # Do the packaging
            # First, mangle all of INSTALLS values. We depend on them.
            unset(MANGLED_INSTALLS)
            for(x, INSTALLS):MANGLED_INSTALLS += install_$${x}
            build_package.path = $${OUT_PWD}/../../../package
            build_package.commands = $$INNO_ISCC \"$${OUT_PWD}/../../../package/valentina.iss\"
            build_package.depends = $${MANGLED_INSTALLS}
            INSTALLS += build_package
        } else {
            message("Inno Setup was not found!")
        }
    }
}

win32 {
    for(DIR, INSTALL_PDFTOPS) {
        #add these absolute paths to a variable which
        #ends up as 'mkcommands = path1 path2 path3 ...'
        pdftops_path += $${PWD}/$$DIR
    }
    copyToDestdir($$pdftops_path, $$shell_path($${OUT_PWD}/$$DESTDIR))

    for(DIR, INSTALL_OPENSSL) {
        #add these absolute paths to a variable which
        #ends up as 'mkcommands = path1 path2 path3 ...'
        openssl_path += $${PWD}/$$DIR
    }
    copyToDestdir($$openssl_path, $$shell_path($${OUT_PWD}/$$DESTDIR))
}

noRunPath{ # For enable run qmake with CONFIG+=noRunPath
    # do nothing
} else {
    unix:!macx{
        # suppress the default RPATH
        # helps to run the program without Qt Creator
        # see problem with path to libqmuparser and libpropertybrowser
        QMAKE_LFLAGS_RPATH =
        QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\' -Wl,-rpath,$${OUT_PWD}/../../libs/qmuparser/$${DESTDIR} -Wl,-rpath,$${OUT_PWD}/../../libs/vpropertyexplorer/$${DESTDIR}"
    }
}

# When the GNU linker sees a library, it discards all symbols that it doesn't need.
# Dependent library go first.

#VTools static library (depend on VWidgets, VMisc, VPatternDB)
unix|win32: LIBS += -L$$OUT_PWD/../../libs/vtools/$${DESTDIR}/ -lvtools

INCLUDEPATH += $$PWD/../../libs/vtools
INCLUDEPATH += $$OUT_PWD/../../libs/vtools/$${UI_DIR} # For UI files
DEPENDPATH += $$PWD/../../libs/vtools

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vtools/$${DESTDIR}/vtools.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vtools/$${DESTDIR}/libvtools.a

#VWidgets static library
unix|win32: LIBS += -L$$OUT_PWD/../../libs/vwidgets/$${DESTDIR}/ -lvwidgets

INCLUDEPATH += $$PWD/../../libs/vwidgets
DEPENDPATH += $$PWD/../../libs/vwidgets

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vwidgets/$${DESTDIR}/vwidgets.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vwidgets/$${DESTDIR}/libvwidgets.a

# VFormat static library (depend on VPatternDB, IFC)
unix|win32: LIBS += -L$$OUT_PWD/../../libs/vformat/$${DESTDIR}/ -lvformat

INCLUDEPATH += $$PWD/../../libs/vformat
DEPENDPATH += $$PWD/../../libs/vformat

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vformat/$${DESTDIR}/vformat.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vformat/$${DESTDIR}/libvformat.a

#VPatternDB static library (depend on vgeometry, vmisc, VLayout)
unix|win32: LIBS += -L$$OUT_PWD/../../libs/vpatterndb/$${DESTDIR} -lvpatterndb

INCLUDEPATH += $$PWD/../../libs/vpatterndb
DEPENDPATH += $$PWD/../../libs/vpatterndb

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vpatterndb/$${DESTDIR}/vpatterndb.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vpatterndb/$${DESTDIR}/libvpatterndb.a

# VGeometry static library (depend on ifc)
unix|win32: LIBS += -L$$OUT_PWD/../../libs/vgeometry/$${DESTDIR}/ -lvgeometry

INCLUDEPATH += $$PWD/../../libs/vgeometry
DEPENDPATH += $$PWD/../../libs/vgeometry

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vgeometry/$${DESTDIR}/vgeometry.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vgeometry/$${DESTDIR}/libvgeometry.a

# Fervor static library (depend on VMisc, IFC)
unix|win32: LIBS += -L$$OUT_PWD/../../libs/fervor/$${DESTDIR}/ -lfervor

INCLUDEPATH += $$PWD/../../libs/fervor
DEPENDPATH += $$PWD/../../libs/fervor

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/fervor/$${DESTDIR}/fervor.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/fervor/$${DESTDIR}/libfervor.a

# IFC static library (depend on QMuParser, VMisc)
unix|win32: LIBS += -L$$OUT_PWD/../../libs/ifc/$${DESTDIR}/ -lifc

INCLUDEPATH += $$PWD/../../libs/ifc
DEPENDPATH += $$PWD/../../libs/ifc

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/ifc/$${DESTDIR}/ifc.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/ifc/$${DESTDIR}/libifc.a

#VMisc static library
unix|win32: LIBS += -L$$OUT_PWD/../../libs/vmisc/$${DESTDIR}/ -lvmisc

INCLUDEPATH += $$PWD/../../libs/vmisc
DEPENDPATH += $$PWD/../../libs/vmisc

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vmisc/$${DESTDIR}/vmisc.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vmisc/$${DESTDIR}/libvmisc.a

# VObj static library
unix|win32: LIBS += -L$$OUT_PWD/../../libs/vobj/$${DESTDIR}/ -lvobj

INCLUDEPATH += $$PWD/../../libs/vobj
DEPENDPATH += $$PWD/../../libs/vobj

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vobj/$${DESTDIR}/vobj.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vobj/$${DESTDIR}/libvobj.a

# VDxf static library
unix|win32: LIBS += -L$$OUT_PWD/../../libs/vdxf/$${DESTDIR}/ -lvdxf

INCLUDEPATH += $$PWD/../../libs/vdxf
DEPENDPATH += $$PWD/../../libs/vdxf

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vdxf/$${DESTDIR}/vdxf.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vdxf/$${DESTDIR}/libvdxf.a

# VLayout static library
unix|win32: LIBS += -L$$OUT_PWD/../../libs/vlayout/$${DESTDIR}/ -lvlayout

INCLUDEPATH += $$PWD/../../libs/vlayout
DEPENDPATH += $$PWD/../../libs/vlayout

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vlayout/$${DESTDIR}/vlayout.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vlayout/$${DESTDIR}/libvlayout.a

# QMuParser library
win32:CONFIG(release, debug|release): LIBS += -L$${OUT_PWD}/../../libs/qmuparser/$${DESTDIR} -lqmuparser2
else:win32:CONFIG(debug, debug|release): LIBS += -L$${OUT_PWD}/../../libs/qmuparser/$${DESTDIR} -lqmuparser2
else:unix: LIBS += -L$${OUT_PWD}/../../libs/qmuparser/$${DESTDIR} -lqmuparser

INCLUDEPATH += $${PWD}/../../libs/qmuparser
DEPENDPATH += $${PWD}/../../libs/qmuparser

# VPropertyExplorer library
win32:CONFIG(release, debug|release): LIBS += -L$${OUT_PWD}/../../libs/vpropertyexplorer/$${DESTDIR} -lvpropertyexplorer
else:win32:CONFIG(debug, debug|release): LIBS += -L$${OUT_PWD}/../../libs/vpropertyexplorer/$${DESTDIR} -lvpropertyexplorer
else:unix: LIBS += -L$${OUT_PWD}/../../libs/vpropertyexplorer/$${DESTDIR} -lvpropertyexplorer

INCLUDEPATH += $${PWD}/../../libs/vpropertyexplorer
DEPENDPATH += $${PWD}/../../libs/vpropertyexplorer

noDebugSymbols{ # For enable run qmake with CONFIG+=noDebugSymbols
    # do nothing
} else {
    noStripDebugSymbols { # For enable run qmake with CONFIG+=noStripDebugSymbols
        # do nothing
    } else {
        # Strip after you link all libaries.
        CONFIG(release, debug|release){
            win32:!*msvc*{
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

            !macx:!*msvc*{
                QMAKE_DISTCLEAN += bin/${TARGET}.dbg
            }
        }
    }
}

macx{
   # run macdeployqt to include all qt libraries in packet
   QMAKE_POST_LINK += $$[QT_INSTALL_BINS]/macdeployqt $${OUT_PWD}/$${DESTDIR}/$${TARGET}.app
}
