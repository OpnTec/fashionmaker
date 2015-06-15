TEMPLATE = subdirs
SUBDIRS = \
    qmuparser \
    vpropertyexplorer \
    ifc \
    vobj \
    vlayout \
    vgeometry \
    vpatterndb \
    vmisc \
    vwidgets

vgeometry.depends = ifc
