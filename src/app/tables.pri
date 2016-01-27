# Keep path to all files with standard measurements we support right now
INSTALL_STANDARD_MEASHUREMENTS += $$PWD/share/tables/standard/GOST_man_ru.vst

# Keep path to all template files we have right now
INSTALL_STANDARD_TEMPLATES += $$PWD/share/tables/templates/template_all_measurements.vit

copyToDestdir($$INSTALL_STANDARD_MEASHUREMENTS, $$shell_path($${OUT_PWD}/$$DESTDIR/tables/standard))
