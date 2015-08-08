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
    share/resources/measurements.qrc \ # For measurements files that we save as resource.
    share/resources/toolicon.qrc

# Compilation will fail without this files after we added them to this section.
OTHER_FILES += \
    share/resources/valentina.rc \ # For Windows system.
    share/resources/icon/64x64/icon64x64.ico # Valentina's logo.

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

TRANSLATIONS_PATH = ../../../share/translations

# Add to this variable all translation files that you want install with program.
# For generation *.qm file first you need create *.ts.
# See section TRANSLATIONS in file ../../share/translations/translations.pro.
# See section TRANSLATIONS in file ../../share/translations/measurements.pro.
INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/valentina_ru_RU.qm \
    $${TRANSLATIONS_PATH}/valentina_uk_UA.qm \
    $${TRANSLATIONS_PATH}/valentina_de_DE.qm \
    $${TRANSLATIONS_PATH}/valentina_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/valentina_he_IL.qm \
    $${TRANSLATIONS_PATH}/valentina_fr_FR.qm \
    $${TRANSLATIONS_PATH}/valentina_it_IT.qm \
    $${TRANSLATIONS_PATH}/valentina_nl_NL.qm \
    $${TRANSLATIONS_PATH}/valentina_id_ID.qm \
    $${TRANSLATIONS_PATH}/valentina_es_ES.qm \
    $${TRANSLATIONS_PATH}/valentina_fi_FI.qm \
    $${TRANSLATIONS_PATH}/valentina_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p0_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p0_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p0_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p0_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p0_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p0_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p0_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p0_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p0_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p0_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p0_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p0_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p1_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p1_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p1_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p1_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p1_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p1_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p1_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p1_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p1_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p1_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p1_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p1_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p2_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p2_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p2_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p2_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p2_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p2_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p2_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p2_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p2_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p2_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p2_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p2_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p3_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p3_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p3_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p3_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p3_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p3_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p3_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p3_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p3_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p3_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p3_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p3_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p4_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p4_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p4_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p4_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p4_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p4_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p4_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p4_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p4_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p4_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p4_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p4_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p5_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p5_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p5_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p5_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p5_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p5_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p5_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p5_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p5_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p5_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p5_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p5_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p6_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p6_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p6_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p6_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p6_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p6_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p6_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p6_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p6_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p6_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p6_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p6_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p7_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p7_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p7_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p7_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p7_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p7_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p7_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p7_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p7_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p7_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p7_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p7_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p8_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p8_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p8_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p8_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p8_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p8_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p8_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p8_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p8_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p8_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p8_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p8_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p9_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p9_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p9_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p9_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p9_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p9_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p9_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p9_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p9_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p9_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p9_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p9_en_US.qm

TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p10_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p10_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p10_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p10_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p10_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p10_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p10_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p10_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p10_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p10_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p10_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p10_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p11_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p11_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p11_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p11_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p11_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p11_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p11_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p11_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p11_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p11_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p11_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p11_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p12_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p12_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p12_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p12_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p12_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p12_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p12_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p12_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p12_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p12_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p12_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p12_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p13_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p13_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p13_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p13_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p13_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p13_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p13_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p13_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p13_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p13_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p13_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p13_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p14_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p14_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p14_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p14_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p14_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p14_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p14_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p14_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p14_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p14_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p14_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p14_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p15_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p15_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p15_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p15_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p15_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p15_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p15_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p15_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p15_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p15_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p15_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p15_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p16_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p16_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p16_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p16_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p16_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p16_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p16_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p16_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p16_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p16_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p16_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p16_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p17_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p17_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p17_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p17_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p17_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p17_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p17_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p17_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p17_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p17_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p17_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p17_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p18_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p18_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p18_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p18_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p18_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p18_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p18_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p18_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p18_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p18_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p18_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p18_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p19_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p19_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p19_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p19_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p19_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p19_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p19_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p19_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p19_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p19_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p19_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p19_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p20_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p20_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p20_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p20_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p20_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p20_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p20_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p20_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p20_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p20_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p20_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p20_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p21_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p21_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p21_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p21_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p21_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p21_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p21_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p21_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p21_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p21_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p21_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p21_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p22_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p22_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p22_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p22_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p22_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p22_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p22_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p22_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p22_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p22_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p22_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p22_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p23_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p23_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p23_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p23_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p23_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p23_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p23_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p23_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p23_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p23_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p23_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p23_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p24_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p24_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p24_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p24_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p24_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p24_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p24_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p24_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p24_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p24_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p24_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p24_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p25_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p25_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p25_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p25_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p25_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p25_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p25_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p25_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p25_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p25_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p25_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p25_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p26_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p26_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p26_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p26_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p26_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p26_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p26_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p26_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p26_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p26_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p26_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p26_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p27_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p27_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p27_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p27_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p27_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p27_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p27_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p27_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p27_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p27_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p27_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p27_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p28_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p28_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p28_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p28_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p28_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p28_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p28_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p28_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p28_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p28_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p28_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p28_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p29_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p29_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p29_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p29_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p29_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p29_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p29_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p29_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p29_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p29_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p29_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p29_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p30_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p30_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p30_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p30_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p30_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p30_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p30_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p30_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p30_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p30_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p30_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p30_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p31_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p31_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p31_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p31_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p31_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p31_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p31_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p31_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p31_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p31_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p31_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p31_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p32_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p32_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p32_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p32_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p32_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p32_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p32_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p32_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p32_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p32_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p32_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p32_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p33_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p33_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p33_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p33_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p33_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p33_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p33_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p33_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p33_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p33_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p33_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p33_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p34_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p34_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p34_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p34_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p34_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p34_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p34_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p34_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p34_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p34_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p34_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p34_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p35_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p35_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p35_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p35_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p35_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p35_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p35_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p35_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p35_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p35_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p35_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p35_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p36_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p36_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p36_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p36_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p36_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p36_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p36_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p36_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p36_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p36_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p36_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p36_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p37_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p37_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p37_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p37_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p37_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p37_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p37_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p37_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p37_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p37_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p37_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p37_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p38_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p38_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p38_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p38_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p38_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p38_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p38_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p38_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p38_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p38_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p38_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p38_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p39_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p39_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p39_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p39_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p39_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p39_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p39_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p39_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p39_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p39_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p39_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p39_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p40_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p40_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p40_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p40_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p40_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p40_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p40_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p40_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p40_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p40_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p40_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p40_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p41_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p41_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p41_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p41_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p41_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p41_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p41_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p41_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p41_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p41_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p41_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p41_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p42_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p42_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p42_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p42_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p42_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p42_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p42_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p42_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p42_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p42_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p42_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p42_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p43_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p43_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p43_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p43_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p43_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p43_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p43_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p43_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p43_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p43_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p43_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p43_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p44_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p44_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p44_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p44_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p44_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p44_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p44_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p44_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p44_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p44_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p44_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p44_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p45_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p45_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p45_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p45_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p45_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p45_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p45_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p45_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p45_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p45_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p45_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p45_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p46_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p46_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p46_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p46_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p46_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p46_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p46_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p46_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p46_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p46_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p46_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p46_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p47_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p47_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p47_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p47_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p47_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p47_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p47_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p47_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p47_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p47_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p47_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p47_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p48_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p48_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p48_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p48_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p48_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p48_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p48_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p48_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p48_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p48_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p48_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p48_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p49_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p49_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p49_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p49_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p49_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p49_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p49_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p49_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p49_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p49_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p49_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p49_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p50_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p50_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p50_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p50_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p50_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p50_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p50_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p50_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p50_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p50_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p50_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p50_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p51_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p51_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p51_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p51_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p51_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p51_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p51_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p51_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p51_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p51_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p51_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p51_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p52_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p52_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p52_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p52_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p52_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p52_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p52_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p52_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p52_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p52_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p52_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p52_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p53_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p53_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p53_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p53_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p53_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p53_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p53_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p53_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p53_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p53_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p53_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p53_en_US.qm

INSTALL_TRANSLATIONS += \
    $${TRANSLATIONS_PATH}/measurements_p54_ru_RU.qm \
    $${TRANSLATIONS_PATH}/measurements_p54_uk_UA.qm \
    $${TRANSLATIONS_PATH}/measurements_p54_de_DE.qm \
    $${TRANSLATIONS_PATH}/measurements_p54_cs_CZ.qm \
    $${TRANSLATIONS_PATH}/measurements_p54_he_IL.qm \
    $${TRANSLATIONS_PATH}/measurements_p54_fr_FR.qm \
    $${TRANSLATIONS_PATH}/measurements_p54_it_IT.qm \
    $${TRANSLATIONS_PATH}/measurements_p54_nl_NL.qm \
    $${TRANSLATIONS_PATH}/measurements_p54_id_ID.qm \
    $${TRANSLATIONS_PATH}/measurements_p54_es_ES.qm \
    $${TRANSLATIONS_PATH}/measurements_p54_fi_FI.qm \
    $${TRANSLATIONS_PATH}/measurements_p54_en_US.qm

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

        INSTALLS += \
            target \
            tape \
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
        MACOS_DIR = "Contents/MacOS"
        # On macx we will use app bundle. Bundle doesn't need bin directory inside.
        # See issue #166: Creating OSX Homebrew (Mac OS X package manager) formula.
        target.path = $$MACOS_DIR

        # Copy in bundle translation files.
        exists($${TRANSLATIONS_PATH}/valentina_ru_RU.qm){
            TRANSLATION_ru_RU.files += \
                $${TRANSLATIONS_PATH}/valentina_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p0_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p1_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p2_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p3_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p4_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p5_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p6_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p7_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p8_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p9_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p10_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p11_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p12_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p13_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p14_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p15_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p16_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p17_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p18_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p19_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p20_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p21_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p22_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p23_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p24_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p25_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p26_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p27_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p28_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p29_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p30_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p31_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p32_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p33_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p34_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p35_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p36_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p37_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p38_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p39_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p40_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p41_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p42_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p43_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p44_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p45_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p46_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p47_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p48_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p49_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p50_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p51_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p52_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p53_ru_RU.qm \
                $${TRANSLATIONS_PATH}/measurements_p54_ru_RU.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_ru_RU.path = "$$RESOURCES_DIR/translations/ru_RU.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_ru_RU
        }

        exists($${TRANSLATIONS_PATH}/valentina_uk_UA.qm){
            TRANSLATION_uk_UA.files += \
                $${TRANSLATIONS_PATH}/valentina_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p0_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p1_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p2_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p3_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p4_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p5_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p6_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p7_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p8_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p9_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p10_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p11_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p12_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p13_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p14_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p15_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p16_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p17_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p18_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p19_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p20_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p21_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p22_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p23_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p24_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p25_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p26_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p27_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p28_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p29_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p30_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p31_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p32_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p33_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p34_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p35_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p36_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p37_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p38_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p39_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p40_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p41_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p42_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p43_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p44_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p45_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p46_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p47_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p48_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p49_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p50_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p51_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p52_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p53_uk_UA.qm \
                $${TRANSLATIONS_PATH}/measurements_p54_uk_UA.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_uk_UA.path = "$$RESOURCES_DIR/translations/uk_UA.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_uk_UA
        }

        exists($${TRANSLATIONS_PATH}/valentina_de_DE.qm){
            TRANSLATION_de_DE.files += \
                $${TRANSLATIONS_PATH}/valentina_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p0_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p1_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p2_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p3_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p4_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p5_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p6_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p7_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p8_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p9_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p10_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p11_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p12_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p13_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p14_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p15_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p16_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p17_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p18_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p19_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p20_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p21_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p22_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p23_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p24_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p25_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p26_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p27_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p28_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p29_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p30_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p31_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p32_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p33_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p34_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p35_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p36_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p37_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p38_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p39_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p40_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p41_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p42_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p43_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p44_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p45_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p46_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p47_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p48_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p49_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p50_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p51_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p52_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p53_de_DE.qm \
                $${TRANSLATIONS_PATH}/measurements_p54_de_DE.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_de_DE.path = "$$RESOURCES_DIR/translations/de_DE.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_de_DE
        }

        exists($${TRANSLATIONS_PATH}/valentina_cs_CZ.qm){
            TRANSLATION_cs_CZ.files += \
                $${TRANSLATIONS_PATH}/valentina_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p0_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p1_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p2_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p3_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p4_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p5_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p6_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p7_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p8_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p9_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p10_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p11_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p12_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p13_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p14_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p15_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p16_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p17_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p18_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p19_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p20_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p21_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p22_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p23_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p24_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p25_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p26_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p27_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p28_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p29_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p30_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p31_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p32_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p33_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p34_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p35_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p36_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p37_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p38_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p39_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p40_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p41_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p42_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p43_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p44_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p45_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p46_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p47_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p48_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p49_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p50_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p51_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p52_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p53_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/measurements_p54_cs_CZ.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_cs_CZ.path = "$$RESOURCES_DIR/translations/cs_CZ.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_cs_CZ
        }

        exists($${TRANSLATIONS_PATH}/valentina_he_IL.qm){
            TRANSLATION_he_IL.files += \
                $${TRANSLATIONS_PATH}/valentina_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p0_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p1_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p2_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p3_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p4_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p5_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p6_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p7_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p8_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p9_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p10_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p11_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p12_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p13_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p14_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p15_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p16_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p17_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p18_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p19_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p20_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p21_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p22_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p23_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p24_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p25_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p26_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p27_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p28_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p29_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p30_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p31_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p32_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p33_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p34_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p35_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p36_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p37_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p38_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p39_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p40_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p41_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p42_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p43_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p44_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p45_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p46_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p47_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p48_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p49_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p50_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p51_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p52_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p53_he_IL.qm \
                $${TRANSLATIONS_PATH}/measurements_p54_he_IL.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_he_IL.path = "$$RESOURCES_DIR/translations/he_IL.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_he_IL
        }

        exists($${TRANSLATIONS_PATH}/valentina_fr_FR.qm){
            TRANSLATION_fr_FR.files += \
                $${TRANSLATIONS_PATH}/valentina_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p0_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p1_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p2_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p3_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p4_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p5_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p6_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p7_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p8_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p9_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p10_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p11_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p12_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p13_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p14_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p15_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p16_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p17_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p18_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p19_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p20_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p21_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p22_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p23_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p24_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p25_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p26_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p27_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p28_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p29_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p30_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p31_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p32_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p33_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p34_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p35_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p36_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p37_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p38_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p39_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p40_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p41_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p42_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p43_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p44_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p45_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p46_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p47_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p48_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p49_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p50_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p51_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p52_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p53_fr_FR.qm \
                $${TRANSLATIONS_PATH}/measurements_p54_fr_FR.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_fr_FR.path = "$$RESOURCES_DIR/translations/fr_FR.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_fr_FR
        }

        exists($${TRANSLATIONS_PATH}/valentina_it_IT.qm){
            TRANSLATION_it_IT.files += \
                $${TRANSLATIONS_PATH}/valentina_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p0_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p1_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p2_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p3_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p4_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p5_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p6_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p7_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p8_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p9_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p10_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p11_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p12_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p13_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p14_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p15_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p16_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p17_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p18_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p19_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p20_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p21_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p22_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p23_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p24_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p25_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p26_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p27_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p28_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p29_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p30_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p31_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p32_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p33_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p34_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p35_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p36_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p37_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p38_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p39_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p40_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p41_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p42_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p43_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p44_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p45_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p46_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p47_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p48_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p49_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p50_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p51_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p52_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p53_it_IT.qm \
                $${TRANSLATIONS_PATH}/measurements_p54_it_IT.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_it_IT.path = "$$RESOURCES_DIR/translations/it_IT.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_it_IT
        }

        exists($${TRANSLATIONS_PATH}/valentina_nl_NL.qm){
            TRANSLATION_nl_NL.files += \
                $${TRANSLATIONS_PATH}/valentina_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p0_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p1_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p2_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p3_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p4_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p5_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p6_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p7_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p8_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p9_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p10_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p11_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p12_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p13_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p14_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p15_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p16_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p17_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p18_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p19_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p20_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p21_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p22_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p23_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p24_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p25_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p26_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p27_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p28_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p29_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p30_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p31_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p32_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p33_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p34_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p35_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p36_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p37_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p38_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p39_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p40_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p41_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p42_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p43_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p44_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p45_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p46_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p47_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p48_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p49_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p50_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p51_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p52_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p53_nl_NL.qm \
                $${TRANSLATIONS_PATH}/measurements_p54_nl_NL.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_nl_NL.path = "$$RESOURCES_DIR/translations/nl_NL.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_nl_NL
        }

        exists($${TRANSLATIONS_PATH}/valentina_id_ID.qm){
            TRANSLATION_id_ID.files += \
                $${TRANSLATIONS_PATH}/valentina_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p0_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p1_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p2_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p3_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p4_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p5_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p6_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p7_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p8_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p9_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p10_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p11_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p12_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p13_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p14_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p15_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p16_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p17_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p18_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p19_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p20_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p21_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p22_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p23_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p24_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p25_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p26_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p27_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p28_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p29_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p30_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p31_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p32_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p33_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p34_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p35_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p36_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p37_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p38_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p39_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p40_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p41_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p42_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p43_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p44_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p45_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p46_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p47_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p48_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p49_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p50_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p51_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p52_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p53_id_ID.qm \
                $${TRANSLATIONS_PATH}/measurements_p54_id_ID.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_id_ID.path = "$$RESOURCES_DIR/translations/id_ID.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_id_ID
        }

        exists($${TRANSLATIONS_PATH}/valentina_es_ES.qm){
            TRANSLATION_es_ES.files += \
                $${TRANSLATIONS_PATH}/valentina_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p0_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p1_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p2_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p3_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p4_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p5_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p6_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p7_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p8_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p9_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p10_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p11_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p12_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p13_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p14_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p15_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p16_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p17_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p18_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p19_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p20_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p21_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p22_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p23_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p24_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p25_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p26_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p27_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p28_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p29_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p30_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p31_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p32_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p33_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p34_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p35_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p36_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p37_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p38_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p39_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p40_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p41_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p42_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p43_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p44_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p45_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p46_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p47_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p48_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p49_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p50_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p51_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p52_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p53_es_ES.qm \
                $${TRANSLATIONS_PATH}/measurements_p54_es_ES.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_es_ES.path = "$$RESOURCES_DIR/translations/es_ES.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_es_ES
        }

        exists($${TRANSLATIONS_PATH}/valentina_fi_FI.qm){
            TRANSLATION_fi_FI.files += \
                $${TRANSLATIONS_PATH}/valentina_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p0_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p1_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p2_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p3_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p4_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p5_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p6_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p7_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p8_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p9_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p10_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p11_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p12_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p13_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p14_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p15_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p16_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p17_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p18_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p19_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p20_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p21_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p22_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p23_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p24_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p25_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p26_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p27_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p28_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p29_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p30_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p31_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p32_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p33_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p34_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p35_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p36_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p37_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p38_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p39_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p40_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p41_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p42_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p43_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p44_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p45_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p46_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p47_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p48_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p49_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p50_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p51_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p52_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p53_fi_FI.qm \
                $${TRANSLATIONS_PATH}/measurements_p54_fi_FI.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_fi_FI.path = "$$RESOURCES_DIR/translations/fi_FI.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_fi_FI
        }

        exists($${TRANSLATIONS_PATH}/valentina_en_US.qm){
            TRANSLATION_en_US.files += \
                $${TRANSLATIONS_PATH}/valentina_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p0_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p1_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p2_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p3_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p4_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p5_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p6_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p7_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p8_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p9_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p10_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p11_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p12_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p13_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p14_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p15_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p16_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p17_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p18_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p19_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p20_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p21_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p22_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p23_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p24_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p25_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p26_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p27_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p28_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p29_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p30_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p31_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p32_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p33_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p34_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p35_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p36_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p37_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p38_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p39_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p40_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p41_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p42_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p43_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p44_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p45_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p46_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p47_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p48_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p49_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p50_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p51_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p52_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p53_en_US.qm \
                $${TRANSLATIONS_PATH}/measurements_p54_en_US.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_en_US.path = "$$RESOURCES_DIR/translations/en_US.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_en_US
        }

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

    # Copy to bundle standard measurements files
    standard.path = $$RESOURCES_DIR/tables/standard/
    standard.files = $$INSTALL_STANDARD_MEASHUREMENTS

    # Copy to bundle standard measurements files
    diagrams.path = $$RESOURCES_DIR/
    diagrams.files = $${OUT_PWD}/../tape/$${DESTDIR}/diagrams.rcc

    QMAKE_BUNDLE_DATA += \
        standard \
        libraries \
        tape \
        xpdf \
        diagrams
    }
}

# "make install" command for Windows.
# Depend on nsis script and create installer in folder "package"
win32{
    package.path = $${OUT_PWD}/package/valentina
    package.files += \
        $${OUT_PWD}/$${DESTDIR}/valentina.exe \
        $${OUT_PWD}/$${DESTDIR}/valentina.exe.dbg \
        $${OUT_PWD}/../tape/$${DESTDIR}/tape.exe \
        $${OUT_PWD}/../tape/$${DESTDIR}/tape.exe.dbg \
        $${OUT_PWD}/../tape/$${DESTDIR}/diagrams.rcc \
        $$PWD/../../../dist/win/valentina.ico \
        $$PWD/../../../dist/win/curl.exe \
        $$PWD/../../../dist/win/exchndl.dll \
        $$PWD/../../../dist/win/dbghelp.dll \
        $$PWD/../../../dist/win/mgwhelp.dll \
        $$PWD/../../../dist/win/symsrv.dll \
        $$PWD/../../../dist/win/symsrv.yes \
        $$PWD/../../../dist/win/pdftops.exe \
        $$PWD/../../../AUTHORS.txt \
        $$PWD/../../../LICENSE_GPL.txt \
        $$PWD/../../../README.txt \
        $$PWD/../../../ChangeLog.txt \
        $$PWD/../../libs/qmuparser/LICENSE_BSD.txt \
        $${OUT_PWD}/../../libs/qmuparser/$${DESTDIR}/qmuparser2.dll \
        $${OUT_PWD}/../../libs/qmuparser/$${DESTDIR}/qmuparser2.dll.dbg \
        $${OUT_PWD}/../../libs/vpropertyexplorer/$${DESTDIR}/vpropertyexplorer.dll \
        $${OUT_PWD}/../../libs/vpropertyexplorer/$${DESTDIR}/vpropertyexplorer.dll.dbg \
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
    INSTALLS += package

    package_tables.path = $${OUT_PWD}/package/valentina/tables/standard
    package_tables.files += $${OUT_PWD}/$${DESTDIR}/tables/standard/GOST_man_ru.vst
    INSTALLS += package_tables

    package_translations.path = $${OUT_PWD}/package/valentina/translations
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
        $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_uk.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_ca.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_cs.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_de.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_hu.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_it.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_ja.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_ru.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_sk.qm
    INSTALLS += package_translations

    package_bearer.path = $${OUT_PWD}/package/valentina/bearer
    package_bearer.files += \
        $$[QT_INSTALL_PLUGINS]/bearer/qgenericbearer.dll \
        $$[QT_INSTALL_PLUGINS]/bearer/qnativewifibearer.dll
    INSTALLS += package_bearer

    package_iconengines.path = $${OUT_PWD}/package/valentina/iconengines
    package_iconengines.files += $$[QT_INSTALL_PLUGINS]/iconengines/qsvgicon.dll
    INSTALLS += package_iconengines

    package_imageformats.path = $${OUT_PWD}/package/valentina/imageformats
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

    package_platforms.path = $${OUT_PWD}/package/valentina/platforms
    package_platforms.files += $$[QT_INSTALL_PLUGINS]/platforms/qwindows.dll
    INSTALLS += package_platforms

    package_printsupport.path = $${OUT_PWD}/package/valentina/printsupport
    package_printsupport.files += $$[QT_INSTALL_PLUGINS]/printsupport/windowsprintersupport.dll
    INSTALLS += package_printsupport

    package_nsis.path = $${OUT_PWD}/package
    package_nsis.files += $$PWD/../../dist/win/nsis/valentina.nsi
    INSTALLS += package_nsis

    package_nsis_headers.path = $${OUT_PWD}/package/headers
    package_nsis_headers.files += \
        $$PWD/../../../dist/win/nsis/headers/fileassoc.nsh \
        $$PWD/../../../dist/win/nsis/headers/fileversion.nsh
    INSTALLS += package_nsis_headers

    # Do the packaging
    # First, mangle all of INSTALLS values. We depend on them.
    unset(MANGLED_INSTALLS)
    for(x, INSTALLS):MANGLED_INSTALLS += install_$${x}
    build_package.path = $${OUT_PWD}/package
    build_package.commands = \"C:/Program Files/NSIS/makensisw.exe\" \"$${OUT_PWD}/package/valentina.nsi\"
    build_package.depends = $${MANGLED_INSTALLS}
    INSTALLS += build_package
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

# When the GNU linker sees a library, it discards all symbols that it doesn't need.
# Add dependent library the first.

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

# IFC static library
unix|win32: LIBS += -L$$OUT_PWD/../../libs/ifc/$${DESTDIR}/ -lifc

INCLUDEPATH += $$PWD/../../libs/ifc
DEPENDPATH += $$PWD/../../libs/ifc

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/ifc/$${DESTDIR}/ifc.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/ifc/$${DESTDIR}/libifc.a

# VObj static library
unix|win32: LIBS += -L$$OUT_PWD/../../libs/vobj/$${DESTDIR}/ -lvobj

INCLUDEPATH += $$PWD/../../libs/vobj
DEPENDPATH += $$PWD/../../libs/vobj

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vobj/$${DESTDIR}/vobj.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../../libs/vobj/$${DESTDIR}/libvobj.a

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
        }
    }
}


macx{
   # run macdeployqt to include all qt libraries in packet
   QMAKE_POST_LINK += $$[QT_INSTALL_BINS]/macdeployqt $${OUT_PWD}/$${DESTDIR}/$${TARGET}.app
}
