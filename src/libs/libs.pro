TEMPLATE = subdirs
SUBDIRS = \
    qmuparser \
    vpropertyexplorer \
    ifc \
    vobj \
    vlayout \
    vgeometry \
    vpatterndb

vgeometry.depends = ifc
