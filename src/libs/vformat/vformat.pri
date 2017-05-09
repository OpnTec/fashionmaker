# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

SOURCES += \
    $$PWD/vmeasurements.cpp

win32-msvc*:SOURCES += $$PWD/stable.cpp

HEADERS += \
    $$PWD/vmeasurements.h \
    $$PWD/stable.h
