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
TARGET = valentina

# Use out-of-source builds (shadow builds)
CONFIG -= debug_and_release debug_and_release_target

# We use C++11 standard
CONFIG += c++11

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
                -isystem "$${OUT_PWD}/../../libs/vtools/$${UI_DIR}" \ # For VTools UI files
                $$GCC_DEBUG_CXXFLAGS # See common.pri for more details.

            noAddressSanitizer{ # For enable run qmake with CONFIG+=noAddressSanitizer
                # do nothing
            } else {
                #gcc’s 4.8.0 Address Sanitizer
                #http://blog.qt.digia.com/blog/2013/04/17/using-gccs-4-8-0-address-sanitizer-with-qt/
                QMAKE_CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer
                QMAKE_CFLAGS += -fsanitize=address -fno-omit-frame-pointer
                QMAKE_LFLAGS += -fsanitize=address
            }
        }
        clang*{
            QMAKE_CXXFLAGS += \
                # Key -isystem disable checking errors in system headers.
                -isystem "$${OUT_PWD}/$${UI_DIR}" \
                -isystem "$${OUT_PWD}/$${MOC_DIR}" \
                -isystem "$${OUT_PWD}/$${RCC_DIR}" \
                -isystem "$${OUT_PWD}/../../libs/vtools/$${UI_DIR}" \ # For VTools UI files
                $$CLANG_DEBUG_CXXFLAGS # See common.pri for more details.

            # -isystem key works only for headers. In some cases it's not enough. But we can't delete this warnings and
            # want them in global list. Compromise decision delete them from local list.
            QMAKE_CXXFLAGS -= \
                -Wmissing-prototypes \
                -Wundefined-reinterpret-cast
        }
        *-icc-*{
            QMAKE_CXXFLAGS+= \
                -isystem "$${OUT_PWD}/$${UI_DIR}" \
                -isystem "$${OUT_PWD}/$${MOC_DIR}" \
                -isystem "$${OUT_PWD}/$${RCC_DIR}" \
                -isystem "$${OUT_PWD}/../../libs/vtools/$${UI_DIR}" \ # For VTools UI files
                $$ICC_DEBUG_CXXFLAGS
        }
    } else {
        *-g++{
        QMAKE_CXXFLAGS += $$GCC_DEBUG_CXXFLAGS # See common.pri for more details.
        }
    }

    #Calculate latest tag distance and build revision only in release mode. Change number each time requare
    #recompilation precompiled headers file.
    DEFINES += "LATEST_TAG_DISTANCE=0"
    DEFINES += "BUILD_REVISION=\\\"unknown\\\""
}else{
    # Release mode
    !win32-msvc*:CONFIG += silent
    DEFINES += V_NO_ASSERT
    !unix:*-g++{
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
        !macx:!win32-msvc*{
            QMAKE_CXXFLAGS_RELEASE += -g -gdwarf-3
            QMAKE_CFLAGS_RELEASE += -g -gdwarf-3
            QMAKE_LFLAGS_RELEASE =
        }
    }

    macx{
        HG = /usr/local/bin/hg # Can't defeat PATH variable on Mac OS.
    }else {
        HG = hg # All other platforms all OK.
    }
    #latest tag distance number for using in version
    HG_DISTANCE=$$system($${HG} log -r. --template '{latesttagdistance}')
    isEmpty(HG_DISTANCE){
        HG_DISTANCE = 0 # if we can't find local revision left 0.
    }
    message("Latest tag distance:" $${HG_DISTANCE})
    DEFINES += "LATEST_TAG_DISTANCE=$${HG_DISTANCE}" # Make available latest tag distance number in sources.

    #build revision number for using in version
    unix {
        HG_HESH=$$system("$${HG} log -r. --template '{node|short}'")
    } else {
        # Use escape character before "|" on Windows
        HG_HESH=$$system($${HG} log -r. --template "{node^|short}")
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

# Keep path to all template files we have right now
INSTALL_STANDARD_TEMPLATES += ../tape/share/resources/templates/template_all_measurements.vit

win32 {
    INSTALL_PDFTOPS += ../../../dist/win/pdftops.exe
}

# TRANSLATIONS_PATH defined inside translations.pri
include(translations.pri)

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
        desktop.path = $$DATADIR/applications/
        desktop.files += ../../../dist/$${TARGET}.desktop

        # logo
        pixmaps.path = $$DATADIR/pixmaps/
        pixmaps.files += ../../../dist/$${TARGET}.png \
                         ../../../dist/application-x-valentina-pattern.png

        # Path to translation files after installation
        translations.path = $$DATADIR/$${TARGET}/translations/
        translations.files = $$INSTALL_TRANSLATIONS

        # Path to standard measurement after installation
        standard.path = $$DATADIR/$${TARGET}/tables/standard/
        standard.files = $$INSTALL_STANDARD_MEASHUREMENTS

        # Path to templates after installation
        templates.path = $$DATADIR/$${TARGET}/tables/templates/
        templates.files = $$INSTALL_STANDARD_TEMPLATES

        INSTALLS += \
            target \
            tape \
            desktop \
            pixmaps \
            translations \
            standard \
            templates
    }
    macx{
        # Some macx stuff
        QMAKE_MAC_SDK = macosx

        QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.6
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
        tape.files += $${OUT_PWD}/../tape/$${DESTDIR}/tape

        # Utility pdftops need for saving a layout image to PS and EPS formates.
        xpdf.path = $$MACOS_DIR
        xpdf.files += $${PWD}/../../../dist/macx/bin64/pdftops

        # logo on macx.
        ICON = ../../../dist/Valentina.icns

        QMAKE_INFO_PLIST = $$PWD/../../../dist/macx/Info.plist

        # Copy to bundle standard measurements files
        standard.path = $$RESOURCES_DIR/tables/standard/
        standard.files = $$INSTALL_STANDARD_MEASHUREMENTS

        # Copy to bundle templates files
        templates.path = $$RESOURCES_DIR/tables/templates/
        templates.files = $$INSTALL_STANDARD_TEMPLATES

        # Copy to bundle standard measurements files
        diagrams.path = $$RESOURCES_DIR/
        diagrams.files = $${OUT_PWD}/../tape/$${DESTDIR}/diagrams.rcc

        format.path = $$RESOURCES_DIR/
        format.files = $$PWD/../../../dist/macx/measurements.icns

        QMAKE_BUNDLE_DATA += \
            templates \
            standard \
            libraries \
            tape \
            xpdf \
            diagrams \
            format
    }
}

# "make install" command for Windows.
# Depend on nsis script and create installer in folder "package"
win32:*-g++ {
    package.path = $${OUT_PWD}/../../../package/valentina
    package.files += \
        $${OUT_PWD}/$${DESTDIR}/valentina.exe \
        $${OUT_PWD}/../tape/$${DESTDIR}/tape.exe \
        $${OUT_PWD}/../tape/$${DESTDIR}/diagrams.rcc \
        $$PWD/../../../dist/win/valentina.ico \
        $$PWD/../../../dist/win/measurements.ico \
        $$PWD/../../../dist/win/pdftops.exe \
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

    package_tables.path = $${OUT_PWD}/../../../package/valentina/tables/standard
    package_tables.files += $$INSTALL_STANDARD_MEASHUREMENTS
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

    NSIS_MAKENSISW = "C:/Program Files/NSIS/makensisw.exe"

    exists($$NSIS_MAKENSISW) {
        package_nsis.path = $${OUT_PWD}/../../../package
        package_nsis.files += \
            $$PWD/../../../dist/win/nsis/valentina.nsi \
            $$PWD/../../../dist/win/nsis/unList.exe # copy exe instead of creating from nsi
        INSTALLS += package_nsis

        package_nsis_headers.path = $${OUT_PWD}/../../../package/headers
        package_nsis_headers.files += \
            $$PWD/../../../dist/win/nsis/headers/fileassoc.nsh \
            $$PWD/../../../dist/win/nsis/headers/fileversion.nsh
        INSTALLS += package_nsis_headers

        # Do the packaging
        # First, mangle all of INSTALLS values. We depend on them.
        unset(MANGLED_INSTALLS)
        for(x, INSTALLS):MANGLED_INSTALLS += install_$${x}
        build_package.path = $${OUT_PWD}/../../../package
        build_package.commands = $$NSIS_MAKENSISW \"$${OUT_PWD}/../../../package/valentina.nsi\"
        build_package.depends = $${MANGLED_INSTALLS}
        INSTALLS += build_package
    } else {
        message("NSIS was not found!")
    }
}

for(DIR, INSTALL_STANDARD_MEASHUREMENTS) {
     #add these absolute paths to a variable which
     #ends up as 'mkcommands = path1 path2 path3 ...'
     st_path += $${PWD}/$$DIR
}

copyToDestdir($$st_path, $$shell_path($${OUT_PWD}/$$DESTDIR/tables/standard))

for(DIR, INSTALL_STANDARD_TEMPLATES) {
     #add these absolute paths to a variable which
     #ends up as 'mkcommands = path1 path2 path3 ...'
     t_path += $${PWD}/$$DIR
}

copyToDestdir($$t_path, $$shell_path($${OUT_PWD}/../tape/$${DESTDIR}/tables/templates))

win32 {
    for(DIR, INSTALL_PDFTOPS) {
        #add these absolute paths to a variable which
        #ends up as 'mkcommands = path1 path2 path3 ...'
        pdftops_path += $${PWD}/$$DIR
    }
    copyToDestdir($$pdftops_path, $$shell_path($${OUT_PWD}/$$DESTDIR))
}

unix:!macx{
    # suppress the default RPATH
    QMAKE_LFLAGS_RPATH =
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\' -Wl,-rpath,$${OUT_PWD}/../../libs/qmuparser/$${DESTDIR} -Wl,-rpath,$${OUT_PWD}/../../libs/vpropertyexplorer/$${DESTDIR}"
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

            !macx:!win32-msvc*{
                QMAKE_DISTCLEAN += bin/${TARGET}.dbg
            }
        }
    }
}

macx{
   # run macdeployqt to include all qt libraries in packet
   QMAKE_POST_LINK += $$[QT_INSTALL_BINS]/macdeployqt $${OUT_PWD}/$${DESTDIR}/$${TARGET}.app -executable=$${OUT_PWD}/$${DESTDIR}/$${TARGET}.app/$$MACOS_DIR/tape
}
