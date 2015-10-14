#-------------------------------------------------
#
# Project created by QtCreator 2015-07-07T14:35:12
#
#-------------------------------------------------

# File with common stuff for whole project
include(../../../common.pri)

QT       += core gui widgets network xml xmlpatterns printsupport

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

macx{
    # logo on macx.
    ICON = ../../../dist/Tape.icns
}

# Suport subdirectories. Just better project code tree.
include(tape.pri)

RESOURCES += \
    share/resources/tapeicon.qrc

DATA_RESOURCE = share/resources/diagrams.qrc # External Binary Resource

# Compilation will fail without this files after we added them to this section.
OTHER_FILES += \
    share/resources/tapeicon/64x64/logo.ico \ # Tape's logo.
    $$DATA_RESOURCE

# Set using ccache. Function enable_ccache() defined in common.pri.
macx {
    CONFIG(debug, debug|release){
        $$enable_ccache()# Use only in debug mode on Mac
    }
} else {
    $$enable_ccache()
}

# Set precompiled headers. Function set_PCH() defined in common.pri.
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
        }
    } else {
        *-g++{
        QMAKE_CXXFLAGS += $$GCC_DEBUG_CXXFLAGS # See common.pri for more details.
        }
    }
    DEFINES += "BUILD_REVISION=\\\"unknown\\\""
}else{
    # Release mode
    CONFIG += silent
    DEFINES += V_NO_ASSERT
    !unix:*-g++{
        QMAKE_CXXFLAGS += -fno-omit-frame-pointer # Need for exchndl.dll
    }

    noDebugSymbols{ # For enable run qmake with CONFIG+=noDebugSymbols
        # do nothing
    } else {
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

DIAGRAMS += \
        $${PWD}/share/resources/diagrams/Ap1.png \
        $${PWD}/share/resources/diagrams/Ap2.png \
        $${PWD}/share/resources/diagrams/Bp1.png \
        $${PWD}/share/resources/diagrams/Bp2.png \
        $${PWD}/share/resources/diagrams/Cp1.png \
        $${PWD}/share/resources/diagrams/Cp2.png \
        $${PWD}/share/resources/diagrams/Dp1.png \
        $${PWD}/share/resources/diagrams/Dp2.png \
        $${PWD}/share/resources/diagrams/Dp3.png \
        $${PWD}/share/resources/diagrams/Ep1.png \
        $${PWD}/share/resources/diagrams/Ep2.png \
        $${PWD}/share/resources/diagrams/Fp1.png \
        $${PWD}/share/resources/diagrams/Fp2.png \
        $${PWD}/share/resources/diagrams/Fp3.png \
        $${PWD}/share/resources/diagrams/Gp1.png \
        $${PWD}/share/resources/diagrams/Gp2.png \
        $${PWD}/share/resources/diagrams/Gp3.png \
        $${PWD}/share/resources/diagrams/Gp4.png \
        $${PWD}/share/resources/diagrams/Gp5.png \
        $${PWD}/share/resources/diagrams/Gp6.png \
        $${PWD}/share/resources/diagrams/Gp7.png \
        $${PWD}/share/resources/diagrams/Gp8.png \
        $${PWD}/share/resources/diagrams/Hp1.png \
        $${PWD}/share/resources/diagrams/Hp2.png \
        $${PWD}/share/resources/diagrams/Hp3.png \
        $${PWD}/share/resources/diagrams/Hp4.png \
        $${PWD}/share/resources/diagrams/Hp5.png \
        $${PWD}/share/resources/diagrams/Hp6.png \
        $${PWD}/share/resources/diagrams/Hp7.png \
        $${PWD}/share/resources/diagrams/Hp8.png \
        $${PWD}/share/resources/diagrams/Hp9.png \
        $${PWD}/share/resources/diagrams/Hp10.png \
        $${PWD}/share/resources/diagrams/Hp11.png \
        $${PWD}/share/resources/diagrams/Hp12.png \
        $${PWD}/share/resources/diagrams/Ip1.png \
        $${PWD}/share/resources/diagrams/Ip2.png \
        $${PWD}/share/resources/diagrams/Ip3.png \
        $${PWD}/share/resources/diagrams/Ip4.png \
        $${PWD}/share/resources/diagrams/Ip5.png \
        $${PWD}/share/resources/diagrams/Ip6.png \
        $${PWD}/share/resources/diagrams/Ip7.png \
        $${PWD}/share/resources/diagrams/Jp1.png \
        $${PWD}/share/resources/diagrams/Jp2.png \
        $${PWD}/share/resources/diagrams/Jp3.png \
        $${PWD}/share/resources/diagrams/Jp4.png \
        $${PWD}/share/resources/diagrams/Jp5.png \
        $${PWD}/share/resources/diagrams/Kp1.png \
        $${PWD}/share/resources/diagrams/Kp2.png \
        $${PWD}/share/resources/diagrams/Kp3.png \
        $${PWD}/share/resources/diagrams/Kp4.png \
        $${PWD}/share/resources/diagrams/Kp5.png \
        $${PWD}/share/resources/diagrams/Kp6.png \
        $${PWD}/share/resources/diagrams/Kp7.png \
        $${PWD}/share/resources/diagrams/Kp8.png \
        $${PWD}/share/resources/diagrams/Kp9.png \
        $${PWD}/share/resources/diagrams/Kp10.png \
        $${PWD}/share/resources/diagrams/Kp11.png \
        $${PWD}/share/resources/diagrams/Lp1.png \
        $${PWD}/share/resources/diagrams/Lp2.png \
        $${PWD}/share/resources/diagrams/Lp3.png \
        $${PWD}/share/resources/diagrams/Lp4.png \
        $${PWD}/share/resources/diagrams/Lp5.png \
        $${PWD}/share/resources/diagrams/Lp6.png \
        $${PWD}/share/resources/diagrams/Lp7.png \
        $${PWD}/share/resources/diagrams/Lp8.png \
        $${PWD}/share/resources/diagrams/Lp9.png \
        $${PWD}/share/resources/diagrams/Mp1.png \
        $${PWD}/share/resources/diagrams/Mp2.png \
        $${PWD}/share/resources/diagrams/Mp3.png \
        $${PWD}/share/resources/diagrams/Np1.png \
        $${PWD}/share/resources/diagrams/Np2.png \
        $${PWD}/share/resources/diagrams/Np3.png \
        $${PWD}/share/resources/diagrams/Np4.png \
        $${PWD}/share/resources/diagrams/Op1.png \
        $${PWD}/share/resources/diagrams/Op2.png \
        $${PWD}/share/resources/diagrams/Op3.png \
        $${PWD}/share/resources/diagrams/Op4.png \
        $${PWD}/share/resources/diagrams/Op5.png \
        $${PWD}/share/resources/diagrams/Op6.png \
        $${PWD}/share/resources/diagrams/Op7.png \
        $${PWD}/share/resources/diagrams/Op8.png \
        $${PWD}/share/resources/diagrams/Op9.png \
        $${PWD}/share/resources/diagrams/Op10.png \
        $${PWD}/share/resources/diagrams/Op11.png \
        $${PWD}/share/resources/diagrams/Pp1.png \
        $${PWD}/share/resources/diagrams/Pp2.png \
        $${PWD}/share/resources/diagrams/Pp3.png \
        $${PWD}/share/resources/diagrams/Pp4.png \
        $${PWD}/share/resources/diagrams/Pp5.png \
        $${PWD}/share/resources/diagrams/Pp6.png \
        $${PWD}/share/resources/diagrams/Pp7.png \
        $${PWD}/share/resources/diagrams/Pp8.png \
        $${PWD}/share/resources/diagrams/Pp9.png \
        $${PWD}/share/resources/diagrams/Pp10.png \
        $${PWD}/share/resources/diagrams/Pp11.png \
        $${PWD}/share/resources/diagrams/Pp12.png \
        $${PWD}/share/resources/diagrams/Qp1.png \
        $${PWD}/share/resources/diagrams/Qp2.png \
        $${PWD}/share/resources/diagrams/Qp3.png

diagrams.name = resource diagrams
diagrams.CONFIG += no_link target_predeps
diagrams.depends = $$DIAGRAMS # expects a list of files
diagrams.input = DATA_RESOURCE # expects the name of a variable
diagrams.output = ${QMAKE_FILE_BASE}.rcc
diagrams.commands = $$shell_path($$[QT_INSTALL_BINS]/rcc) --binary --no-compress ${QMAKE_FILE_IN} -o $${OUT_PWD}/$${DESTDIR}/${QMAKE_FILE_OUT}

QMAKE_EXTRA_COMPILERS += diagrams

QMAKE_CLEAN += $${OUT_PWD}/$${DESTDIR}/diagrams.rcc

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

            !macx{
                QMAKE_DISTCLEAN += bin/${TARGET}.dbg
            }
        }
    }
}
