#-------------------------------------------------
#
# Project created by QtCreator 2015-03-31T17:27:03
#
#-------------------------------------------------

QT       += testlib gui printsupport

TARGET = ValentinaTests

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
    tst_vposter.cpp \
    tst_vabstractdetail.cpp \
    tst_vspline.cpp \
    abstracttest.cpp \
    tst_nameregexp.cpp \
    tst_vlayoutdetail.cpp \
    tst_varc.cpp \
    stable.cpp \
    tst_measurementregexp.cpp

HEADERS += \
    tst_vposter.h \
    tst_vabstractdetail.h \
    tst_vspline.h \
    abstracttest.h \
    tst_nameregexp.h \
    tst_vlayoutdetail.h \
    tst_varc.h \
    stable.h \
    tst_measurementregexp.h

# Set using ccache. Function enable_ccache() defined in common.pri.
$$enable_ccache()

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
}else{
    # Release mode
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
unix|win32: LIBS += -L$$OUT_PWD/../../libs/vgeometry/$${DESTDIR} -lvgeometry

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
unix|win32: LIBS += -L$$OUT_PWD/../../libs/vlayout/$${DESTDIR} -lvlayout

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
