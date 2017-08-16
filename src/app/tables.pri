# Keep path to all files with multisize measurements we support right now
INSTALL_MULTISIZE_MEASUREMENTS += $$PWD/share/tables/multisize/GOST_man_ru.vst

# Keep path to all template files we have right now
INSTALL_STANDARD_TEMPLATES += \
    $$PWD/share/tables/templates/template_all_measurements.vit \
    $$PWD/share/tables/templates/t_Aldrich_Women.vit

# Keep path to all label templates we provide right now
INSTALL_LABEL_TEMPLATES += $$PWD/share/labels/def_pattern_label.xml

copyToDestdir($$INSTALL_MULTISIZE_MEASUREMENTS, $$shell_path($${OUT_PWD}/$$DESTDIR/tables/multisize))
copyToDestdir($$INSTALL_LABEL_TEMPLATES, $$shell_path($${OUT_PWD}/$$DESTDIR/labels))
