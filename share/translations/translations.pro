# Don't use this pro file for building project!!!
# I made this hack for creation one TS file for whole project.
# Because we have several libraries that also have strings for translation, have several TS files for each
# subproject annoying.
#
# Original idea "How to generate a single translation file for a large Qt project?"
# https://stackoverflow.com/questions/1829474/how-to-generate-a-single-translation-file-for-a-large-qt-project

TEMPLATE = app
DEPENDPATH +=  \
            ../../src/app/valentina \
            ../../src/app/tape \
            ../../src/libs/qmuparser \
            ../../src/libs/vpropertyexplorer \
            ../../src/libs/ifc \
            ../../src/libs/vobj \
            ../../src/libs/vlayout \
            ../../src/libs/vgeometry \
            ../../src/libs/vpatterndb \
            ../../src/libs/vmisc \
            ../../src/libs/vtools \
            ../../src/libs/vformat

include(../../src/app/valentina/valentina.pri)
include(../../src/app/tape/tape.pri)
include(../../src/libs/qmuparser/qmuparser.pri)
include(../../src/libs/vpropertyexplorer/vpropertyexplorer.pri)
include(../../src/libs/ifc/ifc.pri)
include(../../src/libs/vobj/vobj.pri)
include(../../src/libs/vlayout/vlayout.pri)
include(../../src/libs/vgeometry/vgeometry.pri)
include(../../src/libs/vpatterndb/vpatterndb.pri)
include(../../src/libs/vmisc/vmisc.pri)
include(../../src/libs/vtools/vtools.pri)
include(../../src/libs/vformat/vformat.pri)

# Add here path to new translation file with name "valentina_*_*.ts" if you want to add new language.
# Same paths in variable INSTALL_TRANSLATIONS (translations.pri).

# File valentina.ts we use in transifex.com. It is empty translation file only with english inside. transifex.com use
# this file like base for new language. Don't add path to valentina.ts to INSTALL_TRANSLATIONS variable (translations.pri).
# When adding a translation here, also add it in the macx part (translations.pri).

LANGUAGES += \
    ru_RU \
    uk_UA \
    de_DE \
    cs_CZ \
    he_IL \
    fr_FR \
    it_IT \
    nl_NL \
    id_ID \
    es_ES \
    fi_FI \
    en_US \
    en_CA \
    en_IN \
    ro_RO \
    zh_CN

TRANSLATIONS += valentina.ts

for(lang, LANGUAGES) {
    TRANSLATIONS += valentina_$${lang}.ts
}

