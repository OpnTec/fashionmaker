# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

SOURCES += \
    $$PWD/vobjengine.cpp \
    $$PWD/vobjpaintdevice.cpp \
    $$PWD/delaunay.cpp \
    $$PWD/predicates.cpp

*msvc*:SOURCES += $$PWD/stable.cpp

HEADERS += \
    $$PWD/vobjengine.h \
    $$PWD/delaunay.h \
    $$PWD/vobjpaintdevice.h \
    $$PWD/stable.h
