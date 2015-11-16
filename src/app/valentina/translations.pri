TRANSLATIONS_PATH = ../../../share/translations

# Add to this variable all translation files that you want install with program.
# For generation *.qm file first you need create *.ts.
# See section TRANSLATIONS in file ../../share/translations/translations.pro.
# See section TRANSLATIONS in file ../../share/translations/measurements.pro.
INSTALL_TRANSLATIONS += $$files($${TRANSLATIONS_PATH}/*.qm)

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
}
