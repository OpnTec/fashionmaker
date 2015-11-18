#-------------------------------------------------
#
# Project created by QtCreator 2015-07-07T14:35:12
#
#-------------------------------------------------

# File with common stuff for whole project
include(../../../common.pri)

QT       += core gui widgets network xml xmlpatterns printsupport svg

# Name of binary file
TARGET = tape

# We want create executable file
TEMPLATE = app

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
include(tape.pri)

RESOURCES += \
    share/resources/tapeicon.qrc

DATA_RESOURCE = share/resources/diagrams.qrc # External Binary Resource

# The list helps to check that all files are exist.
# Don't forget to convert text to curves.
DIAGRAMS += \
        $${PWD}/share/resources/diagrams/Ap1.svg \
        $${PWD}/share/resources/diagrams/Ap2.svg \
        $${PWD}/share/resources/diagrams/Bp1.svg \
        $${PWD}/share/resources/diagrams/Bp2.svg \
        $${PWD}/share/resources/diagrams/Cp1.svg \
        $${PWD}/share/resources/diagrams/Cp2.svg \
        $${PWD}/share/resources/diagrams/Dp1.svg \
        $${PWD}/share/resources/diagrams/Dp2.svg \
        $${PWD}/share/resources/diagrams/Dp3.svg \
        $${PWD}/share/resources/diagrams/Ep1.svg \
        $${PWD}/share/resources/diagrams/Ep2.svg \
        $${PWD}/share/resources/diagrams/Fp1.svg \
        $${PWD}/share/resources/diagrams/Fp2.svg \
        $${PWD}/share/resources/diagrams/Fp3.svg \
        $${PWD}/share/resources/diagrams/Gp1.svg \
        $${PWD}/share/resources/diagrams/Gp2.svg \
        $${PWD}/share/resources/diagrams/Gp3.svg \
        $${PWD}/share/resources/diagrams/Gp4.svg \
        $${PWD}/share/resources/diagrams/Gp5.svg \
        $${PWD}/share/resources/diagrams/Gp6.svg \
        $${PWD}/share/resources/diagrams/Gp7.svg \
        $${PWD}/share/resources/diagrams/Gp8.svg \
        $${PWD}/share/resources/diagrams/Gp9.svg \
        $${PWD}/share/resources/diagrams/Hp1.svg \
        $${PWD}/share/resources/diagrams/Hp2.svg \
        $${PWD}/share/resources/diagrams/Hp3.svg \
        $${PWD}/share/resources/diagrams/Hp4.svg \
        $${PWD}/share/resources/diagrams/Hp5.svg \
        $${PWD}/share/resources/diagrams/Hp6.svg \
        $${PWD}/share/resources/diagrams/Hp7.svg \
        $${PWD}/share/resources/diagrams/Hp8.svg \
        $${PWD}/share/resources/diagrams/Hp9.svg \
        $${PWD}/share/resources/diagrams/Hp10.svg \
        $${PWD}/share/resources/diagrams/Hp11.svg \
        $${PWD}/share/resources/diagrams/Hp12.svg \
        $${PWD}/share/resources/diagrams/Hp13.svg \
        $${PWD}/share/resources/diagrams/Ip1.svg \
        $${PWD}/share/resources/diagrams/Ip2.svg \
        $${PWD}/share/resources/diagrams/Ip3.svg \
        $${PWD}/share/resources/diagrams/Ip4.svg \
        $${PWD}/share/resources/diagrams/Ip5.svg \
        $${PWD}/share/resources/diagrams/Ip6.svg \
        $${PWD}/share/resources/diagrams/Ip7.svg \
        $${PWD}/share/resources/diagrams/Jp1.svg \
        $${PWD}/share/resources/diagrams/Jp2.svg \
        $${PWD}/share/resources/diagrams/Jp3.svg \
        $${PWD}/share/resources/diagrams/Jp4.svg \
        $${PWD}/share/resources/diagrams/Jp5.svg \
        $${PWD}/share/resources/diagrams/Kp1.svg \
        $${PWD}/share/resources/diagrams/Kp2.svg \
        $${PWD}/share/resources/diagrams/Kp3.svg \
        $${PWD}/share/resources/diagrams/Kp4.svg \
        $${PWD}/share/resources/diagrams/Kp5.svg \
        $${PWD}/share/resources/diagrams/Kp6.svg \
        $${PWD}/share/resources/diagrams/Kp7.svg \
        $${PWD}/share/resources/diagrams/Kp8.svg \
        $${PWD}/share/resources/diagrams/Kp9.svg \
        $${PWD}/share/resources/diagrams/Kp10.svg \
        $${PWD}/share/resources/diagrams/Kp11.svg \
        $${PWD}/share/resources/diagrams/Lp1.svg \
        $${PWD}/share/resources/diagrams/Lp2.svg \
        $${PWD}/share/resources/diagrams/Lp3.svg \
        $${PWD}/share/resources/diagrams/Lp4.svg \
        $${PWD}/share/resources/diagrams/Lp5.svg \
        $${PWD}/share/resources/diagrams/Lp6.svg \
        $${PWD}/share/resources/diagrams/Lp7.svg \
        $${PWD}/share/resources/diagrams/Lp8.svg \
        $${PWD}/share/resources/diagrams/Lp9.svg \
        $${PWD}/share/resources/diagrams/Lp10.svg \
        $${PWD}/share/resources/diagrams/Mp1.svg \
        $${PWD}/share/resources/diagrams/Mp2.svg \
        $${PWD}/share/resources/diagrams/Mp3.svg \
        $${PWD}/share/resources/diagrams/Np1.svg \
        $${PWD}/share/resources/diagrams/Np2.svg \
        $${PWD}/share/resources/diagrams/Np3.svg \
        $${PWD}/share/resources/diagrams/Np4.svg \
        $${PWD}/share/resources/diagrams/Np5.svg \
        $${PWD}/share/resources/diagrams/Op1.svg \
        $${PWD}/share/resources/diagrams/Op2.svg \
        $${PWD}/share/resources/diagrams/Op3.svg \
        $${PWD}/share/resources/diagrams/Op4.svg \
        $${PWD}/share/resources/diagrams/Op5.svg \
        $${PWD}/share/resources/diagrams/Op6.svg \
        $${PWD}/share/resources/diagrams/Op7.svg \
        $${PWD}/share/resources/diagrams/Op8.svg \
        $${PWD}/share/resources/diagrams/Op9.svg \
        $${PWD}/share/resources/diagrams/Op10.svg \
        $${PWD}/share/resources/diagrams/Op11.svg \
        $${PWD}/share/resources/diagrams/Pp1.svg \
        $${PWD}/share/resources/diagrams/Pp2.svg \
        $${PWD}/share/resources/diagrams/Pp3.svg \
        $${PWD}/share/resources/diagrams/Pp4.svg \
        $${PWD}/share/resources/diagrams/Pp5.svg \
        $${PWD}/share/resources/diagrams/Pp6.svg \
        $${PWD}/share/resources/diagrams/Pp7.svg \
        $${PWD}/share/resources/diagrams/Pp8.svg \
        $${PWD}/share/resources/diagrams/Pp9.svg \
        $${PWD}/share/resources/diagrams/Pp10.svg \
        $${PWD}/share/resources/diagrams/Pp11.svg \
        $${PWD}/share/resources/diagrams/Pp12.svg \
        $${PWD}/share/resources/diagrams/Qp1.svg \
        $${PWD}/share/resources/diagrams/Qp2.svg \
        $${PWD}/share/resources/diagrams/Qp3.svg

diagrams.name = resource diagrams
diagrams.CONFIG += no_link target_predeps
diagrams.depends = $$DIAGRAMS # expects a list of files
diagrams.input = DATA_RESOURCE # expects the name of a variable
diagrams.output = ${QMAKE_FILE_BASE}.rcc
diagrams.commands = $$shell_path($$[QT_INSTALL_BINS]/rcc) --binary --no-compress ${QMAKE_FILE_IN} -o $${OUT_PWD}/$${DESTDIR}/${QMAKE_FILE_OUT}

QMAKE_EXTRA_COMPILERS += diagrams

QMAKE_CLEAN += $${OUT_PWD}/$${DESTDIR}/diagrams.rcc

# INSTALL_STANDARD_MEASHUREMENTS and INSTALL_STANDARD_TEMPLATES inside tables.pri
include(../tables.pri)
copyToDestdir($$INSTALL_STANDARD_TEMPLATES, $$shell_path($${OUT_PWD}/$${DESTDIR}/tables/templates))
include(../translations.pri)

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

    # logo on macx.
    ICON = $$PWD/../../../dist/Tape.icns

    QMAKE_INFO_PLIST = $$PWD/../../../dist/macx/tape/Info.plist

    # Copy to bundle standard measurements files
    standard.path = $$RESOURCES_DIR/tables/standard/
    standard.files = $$INSTALL_STANDARD_MEASHUREMENTS

    # Copy to bundle templates files
    templates.path = $$RESOURCES_DIR/tables/templates/
    templates.files = $$INSTALL_STANDARD_TEMPLATES

    # Copy to bundle standard measurements files
    diagrams.path = $$RESOURCES_DIR/
    diagrams.files = $${OUT_PWD}/$${DESTDIR}/diagrams.rcc

    format.path = $$RESOURCES_DIR/
    format.files = $$PWD/../../../dist/macx/measurements.icns

    QMAKE_BUNDLE_DATA += \
        templates \
        standard \
        libraries \
        diagrams \
        format
}

# Compilation will fail without this files after we added them to this section.
OTHER_FILES += \
    share/resources/tapeicon/64x64/logo.ico \ # Tape's logo.
    $$DATA_RESOURCE

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
                $$CLANG_DEBUG_CXXFLAGS # See common.pri for more details.

            # -isystem key works only for headers. In some cases it's not enough. But we can't delete this warnings and
            # want them in global list. Compromise decision delete them from local list.
            QMAKE_CXXFLAGS -= \
                -Wundefined-reinterpret-cast \
                -Wmissing-prototypes # rcc folder
        }
        *-icc-*{
            QMAKE_CXXFLAGS += \
                -isystem "$${OUT_PWD}/$${UI_DIR}" \
                -isystem "$${OUT_PWD}/$${MOC_DIR}" \
                -isystem "$${OUT_PWD}/$${RCC_DIR}" \
                $$ICC_DEBUG_CXXFLAGS
        }
    } else {
        *-g++{
        QMAKE_CXXFLAGS += $$GCC_DEBUG_CXXFLAGS # See common.pri for more details.
        }
    }
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

# Path to recource file.
win32:RC_FILE = share/resources/tape.rc

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

#VMisc static library
unix|win32: LIBS += -L$$OUT_PWD/../../libs/vmisc/$${DESTDIR}/ -lvmisc

INCLUDEPATH += $$PWD/../../libs/vmisc
DEPENDPATH += $$PWD/../../libs/vmisc

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vmisc/$${DESTDIR}/vmisc.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vmisc/$${DESTDIR}/libvmisc.a

# VGeometry static library (depend on ifc)
unix|win32: LIBS += -L$$OUT_PWD/../../libs/vgeometry/$${DESTDIR}/ -lvgeometry

INCLUDEPATH += $$PWD/../../libs/vgeometry
DEPENDPATH += $$PWD/../../libs/vgeometry

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vgeometry/$${DESTDIR}/vgeometry.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vgeometry/$${DESTDIR}/libvgeometry.a

# IFC static library (depend on QMuParser)
unix|win32: LIBS += -L$$OUT_PWD/../../libs/ifc/$${DESTDIR}/ -lifc

INCLUDEPATH += $$PWD/../../libs/ifc
DEPENDPATH += $$PWD/../../libs/ifc

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/ifc/$${DESTDIR}/ifc.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/ifc/$${DESTDIR}/libifc.a

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
   QMAKE_POST_LINK += $$[QT_INSTALL_BINS]/macdeployqt $${OUT_PWD}/$${DESTDIR}/$${TARGET}.app
}
