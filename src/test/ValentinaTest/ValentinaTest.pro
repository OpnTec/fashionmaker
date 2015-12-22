#-------------------------------------------------
#
# Project created by QtCreator 2015-03-31T17:27:03
#
#-------------------------------------------------

QT       += testlib gui printsupport xml xmlpatterns

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
    tst_measurementregexp.cpp \
    tst_tapecommandline.cpp \
    tst_valentinacommandline.cpp \
    tst_qmutokenparser.cpp \
    tst_vmeasurements.cpp \
    tst_qmuparsererrormsg.cpp \
    tst_vlockguard.cpp \
    tst_misc.cpp \
    tst_vcommandline.cpp \
    tst_tstranslation.cpp

HEADERS += \
    tst_vposter.h \
    tst_vabstractdetail.h \
    tst_vspline.h \
    abstracttest.h \
    tst_nameregexp.h \
    tst_vlayoutdetail.h \
    tst_varc.h \
    stable.h \
    tst_measurementregexp.h \
    tst_tapecommandline.h \
    tst_valentinacommandline.h \
    tst_qmutokenparser.h \
    tst_vmeasurements.h \
    tst_qmuparsererrormsg.h \
    tst_vlockguard.h \
    tst_misc.h \
    tst_vcommandline.h \
    tst_tstranslation.h

# Set using ccache. Function enable_ccache() defined in common.pri.
$$enable_ccache()

DEFINES += TS_DIR=\\\"$${PWD}/../../../share/translations\\\"

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
                $$CLANG_DEBUG_CXXFLAGS \ # See common.pri for more details.
                -Wno-gnu-zero-variadic-macro-arguments\ # See macros QSKIP
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
}else{
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

# Only for adding path to LD_LIBRARY_PATH
# VPropertyExplorer library
win32:CONFIG(release, debug|release): LIBS += -L$${OUT_PWD}/../../libs/vpropertyexplorer/$${DESTDIR} -lvpropertyexplorer
else:win32:CONFIG(debug, debug|release): LIBS += -L$${OUT_PWD}/../../libs/vpropertyexplorer/$${DESTDIR} -lvpropertyexplorer
else:unix: LIBS += -L$${OUT_PWD}/../../libs/vpropertyexplorer/$${DESTDIR} -lvpropertyexplorer

INCLUDEPATH += $${PWD}/../../libs/vpropertyexplorer
DEPENDPATH += $${PWD}/../../libs/vpropertyexplorer

TAPE_TEST_FILES += \
    tst_tape/keiko.vit \
    tst_tape/empty.vit \
    tst_tape/all_measurements_v0.3.0.vit \
    tst_tape/all_measurements_v0.4.0.vst \
    tst_tape/GOST_man_ru_v0.3.0.vst \
    tst_tape/all_measurements_v0.3.3.vit \
    tst_tape/all_measurements_v0.4.2.vst \
    tst_tape/GOST_man_ru_v0.4.2.vst \
    tst_tape/broken1.vit \
    tst_tape/broken2.vit \
    tst_tape/broken3.vit \
    tst_tape/broken4.vit \
    tst_tape/text.vit \
    tst_tape/text.vst

VALENTINA_TEST_FILES += \
    tst_valentina/empty.val \
    tst_valentina/issue_372.val \
    tst_valentina/wrong_obj_type.val \
    tst_valentina/text.val \
    tst_valentina/glimited_no_m.val \
    tst_valentina/glimited_vit.val \
    tst_valentina/glimited.vit \
    tst_valentina/glimited_vst.val \
    tst_valentina/glimited.vst \
    tst_valentina/issue_256.val \
    tst_valentina/issue_256_wrong_path.val \
    tst_valentina/issue_256_correct.vit \
    tst_valentina/issue_256_wrong.vit \
    tst_valentina/issue_256_correct.vst \
    tst_valentina/issue_256_wrong.vit


# Compilation will fail without this files after we added them to this section.
OTHER_FILES += \
    $$TAPE_TEST_FILES \
    $$VALENTINA_TEST_FILES

for(DIR, TAPE_TEST_FILES) {
     #add these absolute paths to a variable which
     #ends up as 'mkcommands = path1 path2 path3 ...'
     tape_path += $${PWD}/$$DIR
}

copyToDestdir($$tape_path, $$shell_path($${OUT_PWD}/$$DESTDIR/tst_tape))

for(DIR, VALENTINA_TEST_FILES) {
     #add these absolute paths to a variable which
     #ends up as 'mkcommands = path1 path2 path3 ...'
     valentina_path += $${PWD}/$$DIR
}

copyToDestdir($$valentina_path, $$shell_path($${OUT_PWD}/$$DESTDIR/tst_valentina))
