# Suport subdirectories. Just better project code tree.
include(exception/exception.pri)
include(xml/xml.pri)

# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

HEADERS += \
    $$PWD/stable.h \
    $$PWD/ifcdef.h

SOURCES += \
    $$PWD/ifcdef.cpp

win32-msvc*:SOURCES += $$PWD/stable.cpp
