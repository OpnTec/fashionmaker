# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

HEADERS += \
    $$PWD/stable.h \
    $$PWD/vlayoutgenerator.h \
    $$PWD/vlayoutdef.h \
    $$PWD/vlayoutpaper.h \
    $$PWD/vlayoutpaper_p.h \
    $$PWD/vbank.h \
    $$PWD/vcontour.h \
    $$PWD/vcontour_p.h \
    $$PWD/vbestsquare.h \
    $$PWD/vposition.h \
    $$PWD/vtextmanager.h \
    $$PWD/vposter.h \
    $$PWD/vgraphicsfillitem.h \
    $$PWD/vabstractpiece.h \
    $$PWD/vabstractpiece_p.h \
    $$PWD/vlayoutpiece.h \
    $$PWD/vlayoutpiece_p.h

SOURCES += \
    $$PWD/vlayoutgenerator.cpp \
    $$PWD/vlayoutpaper.cpp \
    $$PWD/vbank.cpp \
    $$PWD/vcontour.cpp \
    $$PWD/vbestsquare.cpp \
    $$PWD/vposition.cpp \
    $$PWD/vtextmanager.cpp \
    $$PWD/vposter.cpp \
    $$PWD/vgraphicsfillitem.cpp \
    $$PWD/vabstractpiece.cpp \
    $$PWD/vlayoutpiece.cpp

win32-msvc*:SOURCES += $$PWD/stable.cpp
