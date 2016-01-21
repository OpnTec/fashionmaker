# Don't use this pro file for building project!!!
# I made this hack for creation one TS file for whole project.
# Because we have several libraries that also have strings for translation, have several TS files for each
# subproject annoying.
#
# Original idea "How to generate a single translation file for a large Qt project?"
# https://stackoverflow.com/questions/1829474/how-to-generate-a-single-translation-file-for-a-large-qt-project

TEMPLATE = app
DEPENDPATH +=  \
            ../../src/libs/vpatterndb

include(../../src/libs/vpatterndb/trmeasurements.pri)

# Add here path to new translation file with name "valentina_*_*.ts" if you want to add new language.
# Same paths in variable INSTALL_TRANSLATIONS (translations.pri).

# File measurements_*.ts we use in transifex.com. It is empty translation file only with english inside.
# transifex.com use this file like base for new language. Don't add path to measurements_*.ts to INSTALL_TRANSLATIONS
# variable (app.pro).
# When adding a translation here, also add it in the macx part (translations.pri).

PMSYSTEMS += \
    p0 p1 p2 p3 p4 p5 p6 p7 p8 p9 p10 p11 p12 p13 p14 p15 p16 p17 p18 p19 p20 p21 p22 p23 p24 p25 p26 p27 p28 p29 p30 \
    p31 p32 p33 p34 p35 p36 p37 p38 p39 p40 p41 p42 p43 p44 p45 p46 p47 p48 p49 p50 p51 p52 p53 p54 p998

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


for(sys, PMSYSTEMS) {
	TRANSLATIONS += measurements_$${sys}.ts

	for(lang, LANGUAGES) {
        TRANSLATIONS += measurements_$${sys}_$${lang}.ts
    }
}

