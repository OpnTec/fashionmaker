# Don't use this pro file for building project!!!
# I made this hack for creation one TS file for whole project.
# Because we have several libraries that also have strings for translation, have several TS files for each
# subproject annoying.
#
# Original idea "How to generate a single translation file for a large Qt project?"
# https://stackoverflow.com/questions/1829474/how-to-generate-a-single-translation-file-for-a-large-qt-project

TEMPLATE = app
DEPENDPATH +=  \
            ../src/app \
            ../src/libs/qmuparser \
            ../src/libs/vpropertyexplorer \
            ../src/libs/ifc \
            ../src/libs/vobj \
            ../src/libs/vlayout \
			../src/libs/vgeometry \
			../src/libs/vpatterndb \
			../src/libs/vmisc

include(../src/app/app.pri)
include(../src/libs/qmuparser/qmuparser.pri)
include(../src/libs/vpropertyexplorer/vpropertyexplorer.pri)
include(../src/libs/ifc/ifc.pri)
include(../src/libs/vobj/vobj.pri)
include(../src/libs/vlayout/vlayout.pri)
include(../src/libs/vgeometry/vgeometry.pri)
include(../src/libs/vpatterndb/vpatterndb.pri)

# Add here path to new translation file with name "valentina_*_*.ts" if you want to add new language.
# Same paths in variable INSTALL_TRANSLATIONS (app.pro).

# File valentina.ts we use in transifex.com. It is empty translation file only with english inside. transifex.com use
# this file like base for new language. Don't add path to valentina.ts to INSTALL_TRANSLATIONS variable (app.pro).
# When adding a translation here, also add it in the macx part (app.pro).
TRANSLATIONS += translations/valentina.ts \
                translations/valentina_ru_RU.ts \
                translations/valentina_uk_UA.ts \
                translations/valentina_de_DE.ts \
                translations/valentina_cs_CZ.ts \
                translations/valentina_he_IL.ts \
                translations/valentina_fr_FR.ts \
                translations/valentina_it_IT.ts \
                translations/valentina_nl_NL.ts \
                translations/valentina_id_ID.ts \
                translations/valentina_es_ES.ts \
                translations/valentina_fi_FI.ts \
                translations/valentina_en_US.ts
