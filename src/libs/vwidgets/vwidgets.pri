# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

SOURCES += \
    $$PWD/vmaingraphicsscene.cpp \
    $$PWD/vmaingraphicsview.cpp \
    $$PWD/vwidgetpopup.cpp \
    $$PWD/vcontrolpointspline.cpp \
    $$PWD/vgraphicssimpletextitem.cpp \
    $$PWD/vsimplepoint.cpp \
    $$PWD/vabstractsimple.cpp \
    $$PWD/vnobrushscalepathitem.cpp \
    $$PWD/vsimplecurve.cpp \
    $$PWD/vabstractmainwindow.cpp

win32-msvc*:SOURCES += $$PWD/stable.cpp

HEADERS += \
    $$PWD/stable.h \
    $$PWD/vmaingraphicsscene.h \
    $$PWD/vmaingraphicsview.h \
    $$PWD/vwidgetpopup.h \
    $$PWD/vcontrolpointspline.h \
    $$PWD/vgraphicssimpletextitem.h \
    $$PWD/vsimplepoint.h \
    $$PWD/vabstractsimple.h \
    $$PWD/vnobrushscalepathitem.h \
    $$PWD/vsimplecurve.h \
    $$PWD/vabstractmainwindow.h
