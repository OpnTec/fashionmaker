# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

SOURCES += \
    $$PWD/vmeasurements.cpp \
    $$PWD/vlabeltemplate.cpp

*msvc*:SOURCES += $$PWD/stable.cpp

HEADERS += \
    $$PWD/vmeasurements.h \
    $$PWD/stable.h \
    $$PWD/vlabeltemplate.h
