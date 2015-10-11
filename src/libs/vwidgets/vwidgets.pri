# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

SOURCES += \
    $$PWD/vmaingraphicsscene.cpp \
    $$PWD/vmaingraphicsview.cpp \
    $$PWD/vsimplecurve.cpp \
    $$PWD/vwidgetpopup.cpp \
    $$PWD/vcontrolpointspline.cpp \
    $$PWD/vgraphicssimpletextitem.cpp \
    $$PWD/vsimplepoint.cpp \
    $$PWD/vabstractsimple.cpp

win32-msvc*:SOURCES += $$PWD/stable.cpp

HEADERS += \
    $$PWD/stable.h \
    $$PWD/vmaingraphicsscene.h \
    $$PWD/vmaingraphicsview.h \
    $$PWD/vsimplecurve.h \
    $$PWD/vwidgetpopup.h \
    $$PWD/vcontrolpointspline.h \
    $$PWD/vgraphicssimpletextitem.h \
    $$PWD/vsimplepoint.h \
    $$PWD/vabstractsimple.h
