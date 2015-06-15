TEMPLATE = subdirs
SUBDIRS = \
    qmuparser \
    vpropertyexplorer \
    ifc \
    vobj \
    vlayout \
    vgeometry \
    vpatterndb \
    vmisc

vgeometry.depends = ifc
