TEMPLATE = subdirs
SUBDIRS = \
    tape \
    valentina

macx{# For making app bundle tape must exist before valentina.app will be created
    valentina.depends = tape
}
