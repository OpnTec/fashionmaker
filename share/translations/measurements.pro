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
# Same paths in variable INSTALL_TRANSLATIONS (app.pro).

# File measurements_*.ts we use in transifex.com. It is empty translation file only with english inside.
# transifex.com use this file like base for new language. Don't add path to measurements_*.ts to INSTALL_TRANSLATIONS
# variable (app.pro).
# When adding a translation here, also add it in the macx part (app.pro).
TRANSLATIONS += measurements_p0.ts \
                measurements_p0_ru_RU.ts \
                measurements_p0_uk_UA.ts \
                measurements_p0_de_DE.ts \
                measurements_p0_cs_CZ.ts \
                measurements_p0_he_IL.ts \
                measurements_p0_fr_FR.ts \
                measurements_p0_it_IT.ts \
                measurements_p0_nl_NL.ts \
                measurements_p0_id_ID.ts \
                measurements_p0_es_ES.ts \
                measurements_p0_fi_FI.ts \
                measurements_p0_en_US.ts

TRANSLATIONS += measurements_p1.ts \
                measurements_p1_ru_RU.ts \
                measurements_p1_uk_UA.ts \
                measurements_p1_de_DE.ts \
                measurements_p1_cs_CZ.ts \
                measurements_p1_he_IL.ts \
                measurements_p1_fr_FR.ts \
                measurements_p1_it_IT.ts \
                measurements_p1_nl_NL.ts \
                measurements_p1_id_ID.ts \
                measurements_p1_es_ES.ts \
                measurements_p1_fi_FI.ts \
                measurements_p1_en_US.ts

TRANSLATIONS += measurements_p2.ts \
                measurements_p2_ru_RU.ts \
                measurements_p2_uk_UA.ts \
                measurements_p2_de_DE.ts \
                measurements_p2_cs_CZ.ts \
                measurements_p2_he_IL.ts \
                measurements_p2_fr_FR.ts \
                measurements_p2_it_IT.ts \
                measurements_p2_nl_NL.ts \
                measurements_p2_id_ID.ts \
                measurements_p2_es_ES.ts \
                measurements_p2_fi_FI.ts \
                measurements_p2_en_US.ts

TRANSLATIONS += measurements_p3.ts \
                measurements_p3_ru_RU.ts \
                measurements_p3_uk_UA.ts \
                measurements_p3_de_DE.ts \
                measurements_p3_cs_CZ.ts \
                measurements_p3_he_IL.ts \
                measurements_p3_fr_FR.ts \
                measurements_p3_it_IT.ts \
                measurements_p3_nl_NL.ts \
                measurements_p3_id_ID.ts \
                measurements_p3_es_ES.ts \
                measurements_p3_fi_FI.ts \
                measurements_p3_en_US.ts

TRANSLATIONS += measurements_p4.ts \
                measurements_p4_ru_RU.ts \
                measurements_p4_uk_UA.ts \
                measurements_p4_de_DE.ts \
                measurements_p4_cs_CZ.ts \
                measurements_p4_he_IL.ts \
                measurements_p4_fr_FR.ts \
                measurements_p4_it_IT.ts \
                measurements_p4_nl_NL.ts \
                measurements_p4_id_ID.ts \
                measurements_p4_es_ES.ts \
                measurements_p4_fi_FI.ts \
                measurements_p4_en_US.ts

TRANSLATIONS += measurements_p5.ts \
                measurements_p5_ru_RU.ts \
                measurements_p5_uk_UA.ts \
                measurements_p5_de_DE.ts \
                measurements_p5_cs_CZ.ts \
                measurements_p5_he_IL.ts \
                measurements_p5_fr_FR.ts \
                measurements_p5_it_IT.ts \
                measurements_p5_nl_NL.ts \
                measurements_p5_id_ID.ts \
                measurements_p5_es_ES.ts \
                measurements_p5_fi_FI.ts \
                measurements_p5_en_US.ts

TRANSLATIONS += measurements_p6.ts \
                measurements_p6_ru_RU.ts \
                measurements_p6_uk_UA.ts \
                measurements_p6_de_DE.ts \
                measurements_p6_cs_CZ.ts \
                measurements_p6_he_IL.ts \
                measurements_p6_fr_FR.ts \
                measurements_p6_it_IT.ts \
                measurements_p6_nl_NL.ts \
                measurements_p6_id_ID.ts \
                measurements_p6_es_ES.ts \
                measurements_p6_fi_FI.ts \
                measurements_p6_en_US.ts

TRANSLATIONS += measurements_p7.ts \
                measurements_p7_ru_RU.ts \
                measurements_p7_uk_UA.ts \
                measurements_p7_de_DE.ts \
                measurements_p7_cs_CZ.ts \
                measurements_p7_he_IL.ts \
                measurements_p7_fr_FR.ts \
                measurements_p7_it_IT.ts \
                measurements_p7_nl_NL.ts \
                measurements_p7_id_ID.ts \
                measurements_p7_es_ES.ts \
                measurements_p7_fi_FI.ts \
                measurements_p7_en_US.ts

TRANSLATIONS += measurements_p8.ts \
                measurements_p8_ru_RU.ts \
                measurements_p8_uk_UA.ts \
                measurements_p8_de_DE.ts \
                measurements_p8_cs_CZ.ts \
                measurements_p8_he_IL.ts \
                measurements_p8_fr_FR.ts \
                measurements_p8_it_IT.ts \
                measurements_p8_nl_NL.ts \
                measurements_p8_id_ID.ts \
                measurements_p8_es_ES.ts \
                measurements_p8_fi_FI.ts \
                measurements_p8_en_US.ts

TRANSLATIONS += measurements_p9.ts \
                measurements_p9_ru_RU.ts \
                measurements_p9_uk_UA.ts \
                measurements_p9_de_DE.ts \
                measurements_p9_cs_CZ.ts \
                measurements_p9_he_IL.ts \
                measurements_p9_fr_FR.ts \
                measurements_p9_it_IT.ts \
                measurements_p9_nl_NL.ts \
                measurements_p9_id_ID.ts \
                measurements_p9_es_ES.ts \
                measurements_p9_fi_FI.ts \
                measurements_p9_en_US.ts

TRANSLATIONS += measurements_p10.ts \
                measurements_p10_ru_RU.ts \
                measurements_p10_uk_UA.ts \
                measurements_p10_de_DE.ts \
                measurements_p10_cs_CZ.ts \
                measurements_p10_he_IL.ts \
                measurements_p10_fr_FR.ts \
                measurements_p10_it_IT.ts \
                measurements_p10_nl_NL.ts \
                measurements_p10_id_ID.ts \
                measurements_p10_es_ES.ts \
                measurements_p10_fi_FI.ts \
                measurements_p10_en_US.ts

TRANSLATIONS += measurements_p11.ts \
                measurements_p11_ru_RU.ts \
                measurements_p11_uk_UA.ts \
                measurements_p11_de_DE.ts \
                measurements_p11_cs_CZ.ts \
                measurements_p11_he_IL.ts \
                measurements_p11_fr_FR.ts \
                measurements_p11_it_IT.ts \
                measurements_p11_nl_NL.ts \
                measurements_p11_id_ID.ts \
                measurements_p11_es_ES.ts \
                measurements_p11_fi_FI.ts \
                measurements_p11_en_US.ts

TRANSLATIONS += measurements_p12.ts \
                measurements_p12_ru_RU.ts \
                measurements_p12_uk_UA.ts \
                measurements_p12_de_DE.ts \
                measurements_p12_cs_CZ.ts \
                measurements_p12_he_IL.ts \
                measurements_p12_fr_FR.ts \
                measurements_p12_it_IT.ts \
                measurements_p12_nl_NL.ts \
                measurements_p12_id_ID.ts \
                measurements_p12_es_ES.ts \
                measurements_p12_fi_FI.ts \
                measurements_p12_en_US.ts

TRANSLATIONS += measurements_p13.ts \
                measurements_p13_ru_RU.ts \
                measurements_p13_uk_UA.ts \
                measurements_p13_de_DE.ts \
                measurements_p13_cs_CZ.ts \
                measurements_p13_he_IL.ts \
                measurements_p13_fr_FR.ts \
                measurements_p13_it_IT.ts \
                measurements_p13_nl_NL.ts \
                measurements_p13_id_ID.ts \
                measurements_p13_es_ES.ts \
                measurements_p13_fi_FI.ts \
                measurements_p13_en_US.ts

TRANSLATIONS += measurements_p14.ts \
                measurements_p14_ru_RU.ts \
                measurements_p14_uk_UA.ts \
                measurements_p14_de_DE.ts \
                measurements_p14_cs_CZ.ts \
                measurements_p14_he_IL.ts \
                measurements_p14_fr_FR.ts \
                measurements_p14_it_IT.ts \
                measurements_p14_nl_NL.ts \
                measurements_p14_id_ID.ts \
                measurements_p14_es_ES.ts \
                measurements_p14_fi_FI.ts \
                measurements_p14_en_US.ts

TRANSLATIONS += measurements_p15.ts \
                measurements_p15_ru_RU.ts \
                measurements_p15_uk_UA.ts \
                measurements_p15_de_DE.ts \
                measurements_p15_cs_CZ.ts \
                measurements_p15_he_IL.ts \
                measurements_p15_fr_FR.ts \
                measurements_p15_it_IT.ts \
                measurements_p15_nl_NL.ts \
                measurements_p15_id_ID.ts \
                measurements_p15_es_ES.ts \
                measurements_p15_fi_FI.ts \
                measurements_p15_en_US.ts

TRANSLATIONS += measurements_p16.ts \
                measurements_p16_ru_RU.ts \
                measurements_p16_uk_UA.ts \
                measurements_p16_de_DE.ts \
                measurements_p16_cs_CZ.ts \
                measurements_p16_he_IL.ts \
                measurements_p16_fr_FR.ts \
                measurements_p16_it_IT.ts \
                measurements_p16_nl_NL.ts \
                measurements_p16_id_ID.ts \
                measurements_p16_es_ES.ts \
                measurements_p16_fi_FI.ts \
                measurements_p16_en_US.ts

TRANSLATIONS += measurements_p17.ts \
                measurements_p17_ru_RU.ts \
                measurements_p17_uk_UA.ts \
                measurements_p17_de_DE.ts \
                measurements_p17_cs_CZ.ts \
                measurements_p17_he_IL.ts \
                measurements_p17_fr_FR.ts \
                measurements_p17_it_IT.ts \
                measurements_p17_nl_NL.ts \
                measurements_p17_id_ID.ts \
                measurements_p17_es_ES.ts \
                measurements_p17_fi_FI.ts \
                measurements_p17_en_US.ts

TRANSLATIONS += measurements_p18.ts \
                measurements_p18_ru_RU.ts \
                measurements_p18_uk_UA.ts \
                measurements_p18_de_DE.ts \
                measurements_p18_cs_CZ.ts \
                measurements_p18_he_IL.ts \
                measurements_p18_fr_FR.ts \
                measurements_p18_it_IT.ts \
                measurements_p18_nl_NL.ts \
                measurements_p18_id_ID.ts \
                measurements_p18_es_ES.ts \
                measurements_p18_fi_FI.ts \
                measurements_p18_en_US.ts

TRANSLATIONS += measurements_p19.ts \
                measurements_p19_ru_RU.ts \
                measurements_p19_uk_UA.ts \
                measurements_p19_de_DE.ts \
                measurements_p19_cs_CZ.ts \
                measurements_p19_he_IL.ts \
                measurements_p19_fr_FR.ts \
                measurements_p19_it_IT.ts \
                measurements_p19_nl_NL.ts \
                measurements_p19_id_ID.ts \
                measurements_p19_es_ES.ts \
                measurements_p19_fi_FI.ts \
                measurements_p19_en_US.ts

TRANSLATIONS += measurements_p20.ts \
                measurements_p20_ru_RU.ts \
                measurements_p20_uk_UA.ts \
                measurements_p20_de_DE.ts \
                measurements_p20_cs_CZ.ts \
                measurements_p20_he_IL.ts \
                measurements_p20_fr_FR.ts \
                measurements_p20_it_IT.ts \
                measurements_p20_nl_NL.ts \
                measurements_p20_id_ID.ts \
                measurements_p20_es_ES.ts \
                measurements_p20_fi_FI.ts \
                measurements_p20_en_US.ts

TRANSLATIONS += measurements_p21.ts \
                measurements_p21_ru_RU.ts \
                measurements_p21_uk_UA.ts \
                measurements_p21_de_DE.ts \
                measurements_p21_cs_CZ.ts \
                measurements_p21_he_IL.ts \
                measurements_p21_fr_FR.ts \
                measurements_p21_it_IT.ts \
                measurements_p21_nl_NL.ts \
                measurements_p21_id_ID.ts \
                measurements_p21_es_ES.ts \
                measurements_p21_fi_FI.ts \
                measurements_p21_en_US.ts

TRANSLATIONS += measurements_p22.ts \
                measurements_p22_ru_RU.ts \
                measurements_p22_uk_UA.ts \
                measurements_p22_de_DE.ts \
                measurements_p22_cs_CZ.ts \
                measurements_p22_he_IL.ts \
                measurements_p22_fr_FR.ts \
                measurements_p22_it_IT.ts \
                measurements_p22_nl_NL.ts \
                measurements_p22_id_ID.ts \
                measurements_p22_es_ES.ts \
                measurements_p22_fi_FI.ts \
                measurements_p22_en_US.ts

TRANSLATIONS += measurements_p23.ts \
                measurements_p23_ru_RU.ts \
                measurements_p23_uk_UA.ts \
                measurements_p23_de_DE.ts \
                measurements_p23_cs_CZ.ts \
                measurements_p23_he_IL.ts \
                measurements_p23_fr_FR.ts \
                measurements_p23_it_IT.ts \
                measurements_p23_nl_NL.ts \
                measurements_p23_id_ID.ts \
                measurements_p23_es_ES.ts \
                measurements_p23_fi_FI.ts \
                measurements_p23_en_US.ts

TRANSLATIONS += measurements_p24.ts \
                measurements_p24_ru_RU.ts \
                measurements_p24_uk_UA.ts \
                measurements_p24_de_DE.ts \
                measurements_p24_cs_CZ.ts \
                measurements_p24_he_IL.ts \
                measurements_p24_fr_FR.ts \
                measurements_p24_it_IT.ts \
                measurements_p24_nl_NL.ts \
                measurements_p24_id_ID.ts \
                measurements_p24_es_ES.ts \
                measurements_p24_fi_FI.ts \
                measurements_p24_en_US.ts

TRANSLATIONS += measurements_p25.ts \
                measurements_p25_ru_RU.ts \
                measurements_p25_uk_UA.ts \
                measurements_p25_de_DE.ts \
                measurements_p25_cs_CZ.ts \
                measurements_p25_he_IL.ts \
                measurements_p25_fr_FR.ts \
                measurements_p25_it_IT.ts \
                measurements_p25_nl_NL.ts \
                measurements_p25_id_ID.ts \
                measurements_p25_es_ES.ts \
                measurements_p25_fi_FI.ts \
                measurements_p25_en_US.ts

TRANSLATIONS += measurements_p26.ts \
                measurements_p26_ru_RU.ts \
                measurements_p26_uk_UA.ts \
                measurements_p26_de_DE.ts \
                measurements_p26_cs_CZ.ts \
                measurements_p26_he_IL.ts \
                measurements_p26_fr_FR.ts \
                measurements_p26_it_IT.ts \
                measurements_p26_nl_NL.ts \
                measurements_p26_id_ID.ts \
                measurements_p26_es_ES.ts \
                measurements_p26_fi_FI.ts \
                measurements_p26_en_US.ts

TRANSLATIONS += measurements_p27.ts \
                measurements_p27_ru_RU.ts \
                measurements_p27_uk_UA.ts \
                measurements_p27_de_DE.ts \
                measurements_p27_cs_CZ.ts \
                measurements_p27_he_IL.ts \
                measurements_p27_fr_FR.ts \
                measurements_p27_it_IT.ts \
                measurements_p27_nl_NL.ts \
                measurements_p27_id_ID.ts \
                measurements_p27_es_ES.ts \
                measurements_p27_fi_FI.ts \
                measurements_p27_en_US.ts

TRANSLATIONS += measurements_p28.ts \
                measurements_p28_ru_RU.ts \
                measurements_p28_uk_UA.ts \
                measurements_p28_de_DE.ts \
                measurements_p28_cs_CZ.ts \
                measurements_p28_he_IL.ts \
                measurements_p28_fr_FR.ts \
                measurements_p28_it_IT.ts \
                measurements_p28_nl_NL.ts \
                measurements_p28_id_ID.ts \
                measurements_p28_es_ES.ts \
                measurements_p28_fi_FI.ts \
                measurements_p28_en_US.ts

TRANSLATIONS += measurements_p29.ts \
                measurements_p29_ru_RU.ts \
                measurements_p29_uk_UA.ts \
                measurements_p29_de_DE.ts \
                measurements_p29_cs_CZ.ts \
                measurements_p29_he_IL.ts \
                measurements_p29_fr_FR.ts \
                measurements_p29_it_IT.ts \
                measurements_p29_nl_NL.ts \
                measurements_p29_id_ID.ts \
                measurements_p29_es_ES.ts \
                measurements_p29_fi_FI.ts \
                measurements_p29_en_US.ts

TRANSLATIONS += measurements_p30.ts \
                measurements_p30_ru_RU.ts \
                measurements_p30_uk_UA.ts \
                measurements_p30_de_DE.ts \
                measurements_p30_cs_CZ.ts \
                measurements_p30_he_IL.ts \
                measurements_p30_fr_FR.ts \
                measurements_p30_it_IT.ts \
                measurements_p30_nl_NL.ts \
                measurements_p30_id_ID.ts \
                measurements_p30_es_ES.ts \
                measurements_p30_fi_FI.ts \
                measurements_p30_en_US.ts

TRANSLATIONS += measurements_p31.ts \
                measurements_p31_ru_RU.ts \
                measurements_p31_uk_UA.ts \
                measurements_p31_de_DE.ts \
                measurements_p31_cs_CZ.ts \
                measurements_p31_he_IL.ts \
                measurements_p31_fr_FR.ts \
                measurements_p31_it_IT.ts \
                measurements_p31_nl_NL.ts \
                measurements_p31_id_ID.ts \
                measurements_p31_es_ES.ts \
                measurements_p31_fi_FI.ts \
                measurements_p31_en_US.ts

TRANSLATIONS += measurements_p32.ts \
                measurements_p32_ru_RU.ts \
                measurements_p32_uk_UA.ts \
                measurements_p32_de_DE.ts \
                measurements_p32_cs_CZ.ts \
                measurements_p32_he_IL.ts \
                measurements_p32_fr_FR.ts \
                measurements_p32_it_IT.ts \
                measurements_p32_nl_NL.ts \
                measurements_p32_id_ID.ts \
                measurements_p32_es_ES.ts \
                measurements_p32_fi_FI.ts \
                measurements_p32_en_US.ts

TRANSLATIONS += measurements_p33.ts \
                measurements_p33_ru_RU.ts \
                measurements_p33_uk_UA.ts \
                measurements_p33_de_DE.ts \
                measurements_p33_cs_CZ.ts \
                measurements_p33_he_IL.ts \
                measurements_p33_fr_FR.ts \
                measurements_p33_it_IT.ts \
                measurements_p33_nl_NL.ts \
                measurements_p33_id_ID.ts \
                measurements_p33_es_ES.ts \
                measurements_p33_fi_FI.ts \
                measurements_p33_en_US.ts

TRANSLATIONS += measurements_p34.ts \
                measurements_p34_ru_RU.ts \
                measurements_p34_uk_UA.ts \
                measurements_p34_de_DE.ts \
                measurements_p34_cs_CZ.ts \
                measurements_p34_he_IL.ts \
                measurements_p34_fr_FR.ts \
                measurements_p34_it_IT.ts \
                measurements_p34_nl_NL.ts \
                measurements_p34_id_ID.ts \
                measurements_p34_es_ES.ts \
                measurements_p34_fi_FI.ts \
                measurements_p34_en_US.ts

TRANSLATIONS += measurements_p35.ts \
                measurements_p35_ru_RU.ts \
                measurements_p35_uk_UA.ts \
                measurements_p35_de_DE.ts \
                measurements_p35_cs_CZ.ts \
                measurements_p35_he_IL.ts \
                measurements_p35_fr_FR.ts \
                measurements_p35_it_IT.ts \
                measurements_p35_nl_NL.ts \
                measurements_p35_id_ID.ts \
                measurements_p35_es_ES.ts \
                measurements_p35_fi_FI.ts \
                measurements_p35_en_US.ts

TRANSLATIONS += measurements_p36.ts \
                measurements_p36_ru_RU.ts \
                measurements_p36_uk_UA.ts \
                measurements_p36_de_DE.ts \
                measurements_p36_cs_CZ.ts \
                measurements_p36_he_IL.ts \
                measurements_p36_fr_FR.ts \
                measurements_p36_it_IT.ts \
                measurements_p36_nl_NL.ts \
                measurements_p36_id_ID.ts \
                measurements_p36_es_ES.ts \
                measurements_p36_fi_FI.ts \
                measurements_p36_en_US.ts

TRANSLATIONS += measurements_p37.ts \
                measurements_p37_ru_RU.ts \
                measurements_p37_uk_UA.ts \
                measurements_p37_de_DE.ts \
                measurements_p37_cs_CZ.ts \
                measurements_p37_he_IL.ts \
                measurements_p37_fr_FR.ts \
                measurements_p37_it_IT.ts \
                measurements_p37_nl_NL.ts \
                measurements_p37_id_ID.ts \
                measurements_p37_es_ES.ts \
                measurements_p37_fi_FI.ts \
                measurements_p37_en_US.ts

TRANSLATIONS += measurements_p38.ts \
                measurements_p38_ru_RU.ts \
                measurements_p38_uk_UA.ts \
                measurements_p38_de_DE.ts \
                measurements_p38_cs_CZ.ts \
                measurements_p38_he_IL.ts \
                measurements_p38_fr_FR.ts \
                measurements_p38_it_IT.ts \
                measurements_p38_nl_NL.ts \
                measurements_p38_id_ID.ts \
                measurements_p38_es_ES.ts \
                measurements_p38_fi_FI.ts \
                measurements_p38_en_US.ts

TRANSLATIONS += measurements_p39.ts \
                measurements_p39_ru_RU.ts \
                measurements_p39_uk_UA.ts \
                measurements_p39_de_DE.ts \
                measurements_p39_cs_CZ.ts \
                measurements_p39_he_IL.ts \
                measurements_p39_fr_FR.ts \
                measurements_p39_it_IT.ts \
                measurements_p39_nl_NL.ts \
                measurements_p39_id_ID.ts \
                measurements_p39_es_ES.ts \
                measurements_p39_fi_FI.ts \
                measurements_p39_en_US.ts

TRANSLATIONS += measurements_p40.ts \
                measurements_p40_ru_RU.ts \
                measurements_p40_uk_UA.ts \
                measurements_p40_de_DE.ts \
                measurements_p40_cs_CZ.ts \
                measurements_p40_he_IL.ts \
                measurements_p40_fr_FR.ts \
                measurements_p40_it_IT.ts \
                measurements_p40_nl_NL.ts \
                measurements_p40_id_ID.ts \
                measurements_p40_es_ES.ts \
                measurements_p40_fi_FI.ts \
                measurements_p40_en_US.ts

TRANSLATIONS += measurements_p41.ts \
                measurements_p41_ru_RU.ts \
                measurements_p41_uk_UA.ts \
                measurements_p41_de_DE.ts \
                measurements_p41_cs_CZ.ts \
                measurements_p41_he_IL.ts \
                measurements_p41_fr_FR.ts \
                measurements_p41_it_IT.ts \
                measurements_p41_nl_NL.ts \
                measurements_p41_id_ID.ts \
                measurements_p41_es_ES.ts \
                measurements_p41_fi_FI.ts \
                measurements_p41_en_US.ts

TRANSLATIONS += measurements_p42.ts \
                measurements_p42_ru_RU.ts \
                measurements_p42_uk_UA.ts \
                measurements_p42_de_DE.ts \
                measurements_p42_cs_CZ.ts \
                measurements_p42_he_IL.ts \
                measurements_p42_fr_FR.ts \
                measurements_p42_it_IT.ts \
                measurements_p42_nl_NL.ts \
                measurements_p42_id_ID.ts \
                measurements_p42_es_ES.ts \
                measurements_p42_fi_FI.ts \
                measurements_p42_en_US.ts

TRANSLATIONS += measurements_p43.ts \
                measurements_p43_ru_RU.ts \
                measurements_p43_uk_UA.ts \
                measurements_p43_de_DE.ts \
                measurements_p43_cs_CZ.ts \
                measurements_p43_he_IL.ts \
                measurements_p43_fr_FR.ts \
                measurements_p43_it_IT.ts \
                measurements_p43_nl_NL.ts \
                measurements_p43_id_ID.ts \
                measurements_p43_es_ES.ts \
                measurements_p43_fi_FI.ts \
                measurements_p43_en_US.ts

TRANSLATIONS += measurements_p44.ts \
                measurements_p44_ru_RU.ts \
                measurements_p44_uk_UA.ts \
                measurements_p44_de_DE.ts \
                measurements_p44_cs_CZ.ts \
                measurements_p44_he_IL.ts \
                measurements_p44_fr_FR.ts \
                measurements_p44_it_IT.ts \
                measurements_p44_nl_NL.ts \
                measurements_p44_id_ID.ts \
                measurements_p44_es_ES.ts \
                measurements_p44_fi_FI.ts \
                measurements_p44_en_US.ts

TRANSLATIONS += measurements_p45.ts \
                measurements_p45_ru_RU.ts \
                measurements_p45_uk_UA.ts \
                measurements_p45_de_DE.ts \
                measurements_p45_cs_CZ.ts \
                measurements_p45_he_IL.ts \
                measurements_p45_fr_FR.ts \
                measurements_p45_it_IT.ts \
                measurements_p45_nl_NL.ts \
                measurements_p45_id_ID.ts \
                measurements_p45_es_ES.ts \
                measurements_p45_fi_FI.ts \
                measurements_p45_en_US.ts

TRANSLATIONS += measurements_p46.ts \
                measurements_p46_ru_RU.ts \
                measurements_p46_uk_UA.ts \
                measurements_p46_de_DE.ts \
                measurements_p46_cs_CZ.ts \
                measurements_p46_he_IL.ts \
                measurements_p46_fr_FR.ts \
                measurements_p46_it_IT.ts \
                measurements_p46_nl_NL.ts \
                measurements_p46_id_ID.ts \
                measurements_p46_es_ES.ts \
                measurements_p46_fi_FI.ts \
                measurements_p46_en_US.ts

TRANSLATIONS += measurements_p47.ts \
                measurements_p47_ru_RU.ts \
                measurements_p47_uk_UA.ts \
                measurements_p47_de_DE.ts \
                measurements_p47_cs_CZ.ts \
                measurements_p47_he_IL.ts \
                measurements_p47_fr_FR.ts \
                measurements_p47_it_IT.ts \
                measurements_p47_nl_NL.ts \
                measurements_p47_id_ID.ts \
                measurements_p47_es_ES.ts \
                measurements_p47_fi_FI.ts \
                measurements_p47_en_US.ts

TRANSLATIONS += measurements_p48.ts \
                measurements_p48_ru_RU.ts \
                measurements_p48_uk_UA.ts \
                measurements_p48_de_DE.ts \
                measurements_p48_cs_CZ.ts \
                measurements_p48_he_IL.ts \
                measurements_p48_fr_FR.ts \
                measurements_p48_it_IT.ts \
                measurements_p48_nl_NL.ts \
                measurements_p48_id_ID.ts \
                measurements_p48_es_ES.ts \
                measurements_p48_fi_FI.ts \
                measurements_p48_en_US.ts

TRANSLATIONS += measurements_p49.ts \
                measurements_p49_ru_RU.ts \
                measurements_p49_uk_UA.ts \
                measurements_p49_de_DE.ts \
                measurements_p49_cs_CZ.ts \
                measurements_p49_he_IL.ts \
                measurements_p49_fr_FR.ts \
                measurements_p49_it_IT.ts \
                measurements_p49_nl_NL.ts \
                measurements_p49_id_ID.ts \
                measurements_p49_es_ES.ts \
                measurements_p49_fi_FI.ts \
                measurements_p49_en_US.ts

TRANSLATIONS += measurements_p50.ts \
                measurements_p50_ru_RU.ts \
                measurements_p50_uk_UA.ts \
                measurements_p50_de_DE.ts \
                measurements_p50_cs_CZ.ts \
                measurements_p50_he_IL.ts \
                measurements_p50_fr_FR.ts \
                measurements_p50_it_IT.ts \
                measurements_p50_nl_NL.ts \
                measurements_p50_id_ID.ts \
                measurements_p50_es_ES.ts \
                measurements_p50_fi_FI.ts \
                measurements_p50_en_US.ts

TRANSLATIONS += measurements_p51.ts \
                measurements_p51_ru_RU.ts \
                measurements_p51_uk_UA.ts \
                measurements_p51_de_DE.ts \
                measurements_p51_cs_CZ.ts \
                measurements_p51_he_IL.ts \
                measurements_p51_fr_FR.ts \
                measurements_p51_it_IT.ts \
                measurements_p51_nl_NL.ts \
                measurements_p51_id_ID.ts \
                measurements_p51_es_ES.ts \
                measurements_p51_fi_FI.ts \
                measurements_p51_en_US.ts

TRANSLATIONS += measurements_p52.ts \
                measurements_p52_ru_RU.ts \
                measurements_p52_uk_UA.ts \
                measurements_p52_de_DE.ts \
                measurements_p52_cs_CZ.ts \
                measurements_p52_he_IL.ts \
                measurements_p52_fr_FR.ts \
                measurements_p52_it_IT.ts \
                measurements_p52_nl_NL.ts \
                measurements_p52_id_ID.ts \
                measurements_p52_es_ES.ts \
                measurements_p52_fi_FI.ts \
                measurements_p52_en_US.ts

TRANSLATIONS += measurements_p53.ts \
                measurements_p53_ru_RU.ts \
                measurements_p53_uk_UA.ts \
                measurements_p53_de_DE.ts \
                measurements_p53_cs_CZ.ts \
                measurements_p53_he_IL.ts \
                measurements_p53_fr_FR.ts \
                measurements_p53_it_IT.ts \
                measurements_p53_nl_NL.ts \
                measurements_p53_id_ID.ts \
                measurements_p53_es_ES.ts \
                measurements_p53_fi_FI.ts \
                measurements_p53_en_US.ts

TRANSLATIONS += measurements_p54.ts \
                measurements_p54_ru_RU.ts \
                measurements_p54_uk_UA.ts \
                measurements_p54_de_DE.ts \
                measurements_p54_cs_CZ.ts \
                measurements_p54_he_IL.ts \
                measurements_p54_fr_FR.ts \
                measurements_p54_it_IT.ts \
                measurements_p54_nl_NL.ts \
                measurements_p54_id_ID.ts \
                measurements_p54_es_ES.ts \
                measurements_p54_fi_FI.ts \
                measurements_p54_en_US.ts
