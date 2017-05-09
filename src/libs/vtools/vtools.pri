include(dialogs/dialogs.pri)
include(tools/tools.pri)
include(visualization/visualization.pri)
include(undocommands/undocommands.pri)

# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

HEADERS += \
    $$PWD/stable.h

win32-msvc*:SOURCES += $$PWD/stable.cpp

