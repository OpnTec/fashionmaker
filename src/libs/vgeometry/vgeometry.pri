# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

SOURCES += \
        $$PWD/vgobject.cpp \
        $$PWD/vabstractcurve.cpp \
        $$PWD/varc.cpp \
        $$PWD/vpointf.cpp \
        $$PWD/vspline.cpp \
        $$PWD/vsplinepath.cpp \
        $$PWD/vsplinepoint.cpp

win32-msvc*:SOURCES += $$PWD/stable.cpp

HEADERS += \
        $$PWD/stable.h \
        $$PWD/vgobject.h \
        $$PWD/vgobject_p.h \
        $$PWD/vabstractcurve.h \
        $$PWD/varc.h \
        $$PWD/varc_p.h \
        $$PWD/vpointf.h \
        $$PWD/vpointf_p.h \
        $$PWD/vspline.h \
        $$PWD/vspline_p.h \
        $$PWD/vsplinepath.h \
        $$PWD/vsplinepath_p.h \
        $$PWD/vsplinepoint.h \
        $$PWD/vsplinepoint_p.h \
        $$PWD/vgeometrydef.h \
        $$PWD/vabstractcurve_p.h
