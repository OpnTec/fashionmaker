TRANSLATIONS_PATH = $${PWD}/../../share/translations

# Add to this variable all translation files that you want install with program.
# For generation *.qm file first you need create *.ts.
# See section TRANSLATIONS in file ../../share/translations/translations.pro.
# See section TRANSLATIONS in file ../../share/translations/measurements.pro.

PMSYSTEMS += \
    p0 p1 p2 p3 p4 p5 p6 p7 p8 p9 p10 p11 p12 p13 p14 p15 p16 p17 p18 p19 p20 p21 p22 p23 p24 p25 p26 p27 p28 p29 p30 \
    p31 p32 p33 p34 p35 p36 p37 p38 p39 p40 p41 p42 p43 p44 p45 p46 p47 p48 p49 p50 p51 p52 p53 p54 p998

# An example of using LOCALES="de_DE nl_NL"
isEmpty(LOCALES){
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
        zh_CN \
        pt_BR \
        el_GR
} else {
    LANGUAGES = $${LOCALES}
}

for(lang, LANGUAGES) {
    INSTALL_TRANSLATIONS += $${TRANSLATIONS_PATH}/valentina_$${lang}.qm
}

for(lang, LANGUAGES) {
    for(sys, PMSYSTEMS) {
        INSTALL_TRANSLATIONS += $${TRANSLATIONS_PATH}/measurements_$${sys}_$${lang}.qm
    }
}

# Some systems use special name for lrelease. For example opensuse 13.2 has lrelease-qt5.
isEmpty(LRELEASE){
    win32{
        LRELEASE = lrelease.exe
    } else {
        LRELEASE = lrelease
    }
}

# Run generation *.qm file for available *.ts files each time you run qmake.
for(_translation_name, INSTALL_TRANSLATIONS) {
  _translation_name_qm = $$basename(_translation_name)
  _translation_name_ts = $$section(_translation_name_qm, ".", 0, 0).ts

    !exists($$_translation_name) {
        system($$shell_path($$[QT_INSTALL_BINS]/$$LRELEASE) -silent $$shell_path($${TRANSLATIONS_PATH}/$$_translation_name_ts) -qm $$shell_path($$_translation_name))
        unix {
            exists($${OUT_PWD}/$$DESTDIR/valentina) {
                system(rm -fv $${OUT_PWD}/$$DESTDIR/valentina) # force to call linking
            }
            system(rm -fv $${OUT_PWD}/$$DESTDIR/translations/*.qm)
        }
        QMAKE_CLEAN += $$shell_path($$_translation_name)
    }
    QMAKE_CLEAN += $$shell_path($$_translation_name)
}

# Make possible run program even you do not install it. Seek files in local directory.
forceCopyToDestdir($$INSTALL_TRANSLATIONS, $$shell_path($${OUT_PWD}/$$DESTDIR/translations))

macx{
    RESOURCES_DIR = "Contents/Resources"

        # Copy in bundle translation files.
        exists($${TRANSLATIONS_PATH}/valentina_ru_RU.qm){
            TRANSLATION_ru_RU.files += \
                $$files($${TRANSLATIONS_PATH}/*_ru_RU.qm) \
                $$[QT_INSTALL_TRANSLATIONS]/qtbase_ru.qm \
                $$[QT_INSTALL_TRANSLATIONS]/qt_ru.qm \
                $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_ru.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_ru_RU.path = "$$RESOURCES_DIR/translations/ru_RU.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_ru_RU
        }

        exists($${TRANSLATIONS_PATH}/valentina_uk_UA.qm){
            TRANSLATION_uk_UA.files += \
                $$files($${TRANSLATIONS_PATH}/*_uk_UA.qm) \
                $$[QT_INSTALL_TRANSLATIONS]/qtbase_uk.qm \
                $$[QT_INSTALL_TRANSLATIONS]/qt_uk.qm \
                $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_uk.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_uk_UA.path = "$$RESOURCES_DIR/translations/uk_UA.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_uk_UA
        }

        exists($${TRANSLATIONS_PATH}/valentina_de_DE.qm){
            TRANSLATION_de_DE.files += \
                $$files($${TRANSLATIONS_PATH}/*_de_DE.qm) \
                $$[QT_INSTALL_TRANSLATIONS]/qtbase_de.qm \
                $$[QT_INSTALL_TRANSLATIONS]/qt_de.qm \
                $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_de.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_de_DE.path = "$$RESOURCES_DIR/translations/de_DE.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_de_DE
        }

        exists($${TRANSLATIONS_PATH}/valentina_cs_CZ.qm){
            TRANSLATION_cs_CZ.files += \
                $$files($${TRANSLATIONS_PATH}/*_cs_CZ.qm) \
                $$[QT_INSTALL_TRANSLATIONS]/qtbase_cs.qm \
                $$[QT_INSTALL_TRANSLATIONS]/qt_cs.qm \
                $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_cs.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_cs_CZ.path = "$$RESOURCES_DIR/translations/cs_CZ.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_cs_CZ
        }

        exists($${TRANSLATIONS_PATH}/valentina_he_IL.qm){
            TRANSLATION_he_IL.files += \
                $$files($${TRANSLATIONS_PATH}/*_he_IL.qm) \
                $$[QT_INSTALL_TRANSLATIONS]/qtbase_he.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_he_IL.path = "$$RESOURCES_DIR/translations/he_IL.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_he_IL
        }

        exists($${TRANSLATIONS_PATH}/valentina_fr_FR.qm){
            TRANSLATION_fr_FR.files += \
                $$files($${TRANSLATIONS_PATH}/*_fr_FR.qm) \
                $$[QT_INSTALL_TRANSLATIONS]/qtbase_fr.qm \
                $$[QT_INSTALL_TRANSLATIONS]/qt_fr.qm \
                $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_fr.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_fr_FR.path = "$$RESOURCES_DIR/translations/fr_FR.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_fr_FR
        }

        exists($${TRANSLATIONS_PATH}/valentina_it_IT.qm){
            TRANSLATION_it_IT.files += \
                $$files($${TRANSLATIONS_PATH}/*_it_IT.qm) \
                $$[QT_INSTALL_TRANSLATIONS]/qtbase_it.qm \
                $$[QT_INSTALL_TRANSLATIONS]/qt_it.qm \
                $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_it.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_it_IT.path = "$$RESOURCES_DIR/translations/it_IT.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_it_IT
        }

        exists($${TRANSLATIONS_PATH}/valentina_nl_NL.qm){
            TRANSLATION_nl_NL.files += \
                $$files($${TRANSLATIONS_PATH}/*_nl_NL.qm) \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_nl_NL.path = "$$RESOURCES_DIR/translations/nl_NL.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_nl_NL
        }

        exists($${TRANSLATIONS_PATH}/valentina_id_ID.qm){
            TRANSLATION_id_ID.files += \
                $$files($${TRANSLATIONS_PATH}/*_id_ID.qm) \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_id_ID.path = "$$RESOURCES_DIR/translations/id_ID.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_id_ID
        }

        exists($${TRANSLATIONS_PATH}/valentina_es_ES.qm){
            TRANSLATION_es_ES.files += \
                $$files($${TRANSLATIONS_PATH}/*_es_ES.qm) \
                $$[QT_INSTALL_TRANSLATIONS]/qt_es.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_es_ES.path = "$$RESOURCES_DIR/translations/es_ES.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_es_ES
        }

        exists($${TRANSLATIONS_PATH}/valentina_fi_FI.qm){
            TRANSLATION_fi_FI.files += \
                $$files($${TRANSLATIONS_PATH}/*_fi_FI.qm) \
                $$[QT_INSTALL_TRANSLATIONS]/qtbase_fi.qm \
                $$[QT_INSTALL_TRANSLATIONS]/qt_fi.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_fi_FI.path = "$$RESOURCES_DIR/translations/fi_FI.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_fi_FI
        }

        exists($${TRANSLATIONS_PATH}/valentina_en_US.qm){
            TRANSLATION_en_US.files += \
                $$files($${TRANSLATIONS_PATH}/*_en_US.qm) \
                $$[QT_INSTALL_TRANSLATIONS]/qtbase_en.qm \
                $$[QT_INSTALL_TRANSLATIONS]/qt_en.qm \
                $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_en.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_en_US.path = "$$RESOURCES_DIR/translations/en_US.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_en_US
        }

        exists($${TRANSLATIONS_PATH}/valentina_en_CA.qm){
            TRANSLATION_en_CA.files += \
                $$files($${TRANSLATIONS_PATH}/*_en_CA.qm) \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_en_CA.path = "$$RESOURCES_DIR/translations/en_CA.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_en_CA
        }

        exists($${TRANSLATIONS_PATH}/valentina_en_IN.qm){
            TRANSLATION_en_IN.files += \
                $$files($${TRANSLATIONS_PATH}/*_en_IN.qm) \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_en_IN.path = "$$RESOURCES_DIR/translations/en_IN.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_en_IN
        }

        exists($${TRANSLATIONS_PATH}/valentina_ro_RO.qm){
            TRANSLATION_ro_RO.files += \
                $$files($${TRANSLATIONS_PATH}/*_ro_RO.qm) \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_ro_RO.path = "$$RESOURCES_DIR/translations/ro_RO.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_ro_RO
        }

        exists($${TRANSLATIONS_PATH}/valentina_zh_CN.qm){
            TRANSLATION_zh_CN.files += \
                $$files($${TRANSLATIONS_PATH}/*_zh_CN.qm) \
                $$[QT_INSTALL_TRANSLATIONS]/qt_zh_CN.qm \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_zh_CN.path = "$$RESOURCES_DIR/translations/zh_CN.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_zh_CN
        }

        exists($${TRANSLATIONS_PATH}/valentina_pt_BR.qm){
            TRANSLATION_pt_BR.files += \
                $$files($${TRANSLATIONS_PATH}/*_pt_BR.qm) \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_pt_BR.path = "$$RESOURCES_DIR/translations/pt_BR.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_pt_BR
        }

        exists($${TRANSLATIONS_PATH}/valentina_el_GR.qm){
            TRANSLATION_el_GR.files += \
                $$files($${TRANSLATIONS_PATH}/*_el_GR.qm) \
                $${TRANSLATIONS_PATH}/Localizable.strings
            TRANSLATION_el_GR.path = "$$RESOURCES_DIR/translations/el_GR.lproj"
            QMAKE_BUNDLE_DATA += TRANSLATION_el_GR
        }
}
