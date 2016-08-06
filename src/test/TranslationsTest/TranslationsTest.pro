#-------------------------------------------------
#
# Project created by QtCreator 2016-08-04T16:33:17
#
#-------------------------------------------------

QT       += testlib widgets xml printsupport

QT       -= gui

TARGET = TranslationsTest

# File with common stuff for whole project
include(../../../common.pri)

# CONFIG += testcase adds a  'make check' which is great. But by default it also
# adds a 'make install' that installs the test cases, which we do not want.
# Can configure it not to do that with 'no_testcase_installs'
# We use C++11 standard
CONFIG += c++11 testcase no_testcase_installs

# Use out-of-source builds (shadow builds)
CONFIG -= app_bundle debug_and_release debug_and_release_target

TEMPLATE = app

# directory for executable file
DESTDIR = bin

# Directory for files created moc
MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj

DEFINES += SRCDIR=\\\"$$PWD/\\\"

SOURCES += \
    qttestmainlambda.cpp \
    tst_measurementregexp.cpp \
    tst_qmuparsererrormsg.cpp \
    tst_tstranslation.cpp

win32-msvc*:SOURCES += stable.cpp

HEADERS += \
    stable.h \
    tst_measurementregexp.h \
    tst_qmuparsererrormsg.h \
    tst_tstranslation.h

# Set using ccache. Function enable_ccache() defined in common.pri.
$$enable_ccache()

DEFINES += TS_DIR=\\\"$${PWD}/../../../share/translations\\\"

include(warnings.pri)

CONFIG(release, debug|release){
    # Release mode
    !win32-msvc*:CONFIG += silent
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
}

##VTools static library (depend on VWidgets, VMisc, VPatternDB)
#unix|win32: LIBS += -L$$OUT_PWD/../../libs/vtools/$${DESTDIR}/ -lvtools

#INCLUDEPATH += $$PWD/../../libs/vtools
#DEPENDPATH += $$PWD/../../libs/vtools

#win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vtools/$${DESTDIR}/vtools.lib
#else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vtools/$${DESTDIR}/libvtools.a

##VWidgets static library
#unix|win32: LIBS += -L$$OUT_PWD/../../libs/vwidgets/$${DESTDIR}/ -lvwidgets

#INCLUDEPATH += $$PWD/../../libs/vwidgets
#DEPENDPATH += $$PWD/../../libs/vwidgets

#win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vwidgets/$${DESTDIR}/vwidgets.lib
#else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vwidgets/$${DESTDIR}/libvwidgets.a

## VFormat static library (depend on VPatternDB, IFC)
#unix|win32: LIBS += -L$$OUT_PWD/../../libs/vformat/$${DESTDIR}/ -lvformat

#INCLUDEPATH += $$PWD/../../libs/vformat
#DEPENDPATH += $$PWD/../../libs/vformat

#win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vformat/$${DESTDIR}/vformat.lib
#else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vformat/$${DESTDIR}/libvformat.a

#VPatternDB static library (depend on vgeometry, vmisc, VLayout)
unix|win32: LIBS += -L$$OUT_PWD/../../libs/vpatterndb/$${DESTDIR} -lvpatterndb

INCLUDEPATH += $$PWD/../../libs/vpatterndb
DEPENDPATH += $$PWD/../../libs/vpatterndb

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vpatterndb/$${DESTDIR}/vpatterndb.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vpatterndb/$${DESTDIR}/libvpatterndb.a

#VMisc static library
unix|win32: LIBS += -L$$OUT_PWD/../../libs/vmisc/$${DESTDIR} -lvmisc

INCLUDEPATH += $$PWD/../../libs/vmisc
DEPENDPATH += $$PWD/../../libs/vmisc

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vmisc/$${DESTDIR}/vmisc.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vmisc/$${DESTDIR}/libvmisc.a

## VGeometry static library (depend on ifc)
#unix|win32: LIBS += -L$$OUT_PWD/../../libs/vgeometry/$${DESTDIR} -lvgeometry

#INCLUDEPATH += $$PWD/../../libs/vgeometry
#DEPENDPATH += $$PWD/../../libs/vgeometry

#win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vgeometry/$${DESTDIR}/vgeometry.lib
#else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vgeometry/$${DESTDIR}/libvgeometry.a

# IFC static library (depend on QMuParser)
unix|win32: LIBS += -L$$OUT_PWD/../../libs/ifc/$${DESTDIR}/ -lifc

INCLUDEPATH += $$PWD/../../libs/ifc
DEPENDPATH += $$PWD/../../libs/ifc

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/ifc/$${DESTDIR}/ifc.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/ifc/$${DESTDIR}/libifc.a

## VLayout static library
#unix|win32: LIBS += -L$$OUT_PWD/../../libs/vlayout/$${DESTDIR} -lvlayout

#INCLUDEPATH += $$PWD/../../libs/vlayout
#DEPENDPATH += $$PWD/../../libs/vlayout

#win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vlayout/$${DESTDIR}/vlayout.lib
#else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vlayout/$${DESTDIR}/libvlayout.a

# QMuParser library
win32:CONFIG(release, debug|release): LIBS += -L$${OUT_PWD}/../../libs/qmuparser/$${DESTDIR} -lqmuparser2
else:win32:CONFIG(debug, debug|release): LIBS += -L$${OUT_PWD}/../../libs/qmuparser/$${DESTDIR} -lqmuparser2
else:unix: LIBS += -L$${OUT_PWD}/../../libs/qmuparser/$${DESTDIR} -lqmuparser

INCLUDEPATH += $${PWD}/../../libs/qmuparser
DEPENDPATH += $${PWD}/../../libs/qmuparser

## Only for adding path to LD_LIBRARY_PATH
## VPropertyExplorer library
#win32:CONFIG(release, debug|release): LIBS += -L$${OUT_PWD}/../../libs/vpropertyexplorer/$${DESTDIR} -lvpropertyexplorer
#else:win32:CONFIG(debug, debug|release): LIBS += -L$${OUT_PWD}/../../libs/vpropertyexplorer/$${DESTDIR} -lvpropertyexplorer
#else:unix: LIBS += -L$${OUT_PWD}/../../libs/vpropertyexplorer/$${DESTDIR} -lvpropertyexplorer

#INCLUDEPATH += $${PWD}/../../libs/vpropertyexplorer
#DEPENDPATH += $${PWD}/../../libs/vpropertyexplorer
