# Keep path to all files with multisize measurements we support right now
INSTALL_MULTISIZE_MEASUREMENTS += $$PWD/share/tables/multisize/GOST_man_ru.vst

# Keep path to all template files we have right now
INSTALL_STANDARD_TEMPLATES += \
    $$PWD/share/tables/templates/template_all_measurements.vit \
    $$PWD/share/tables/templates/t_Aldrich_Women.vit

copyToDestdir($$INSTALL_MULTISIZE_MEASUREMENTS, $$shell_path($${OUT_PWD}/$$DESTDIR/tables/multisize))
