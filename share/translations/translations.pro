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
# Same paths in variable INSTALL_TRANSLATIONS (app.pro).

# File valentina.ts we use in transifex.com. It is empty translation file only with english inside. transifex.com use
# this file like base for new language. Don't add path to valentina.ts to INSTALL_TRANSLATIONS variable (app.pro).
# When adding a translation here, also add it in the macx part (app.pro).
TRANSLATIONS += valentina.ts \
                valentina_ru_RU.ts \
                valentina_uk_UA.ts \
                valentina_de_DE.ts \
                valentina_cs_CZ.ts \
                valentina_he_IL.ts \
                valentina_fr_FR.ts \
                valentina_it_IT.ts \
                valentina_nl_NL.ts \
                valentina_id_ID.ts \
                valentina_es_ES.ts \
                valentina_fi_FI.ts \
                valentina_en_US.ts \
                valentina_en_CA.ts \
                valentina_en_IN.ts \
                valentina_ro_RO.ts
