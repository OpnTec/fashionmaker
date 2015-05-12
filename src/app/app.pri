# Suport subdirectories. Just better project code tree.
include(container/container.pri)
include(dialogs/dialogs.pri)
include(geometry/geometry.pri)
include(tools/tools.pri)
include(widgets/widgets.pri)
include(xml/xml.pri)
include(undocommands/undocommands.pri)
include(visualization/visualization.pri)
include(core/core.pri)

# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

# Some source files
SOURCES += \
        $$PWD/main.cpp \
        $$PWD/mainwindow.cpp \
        $$PWD/tablewindow.cpp \
        $$PWD/stable.cpp \
        $$PWD/version.cpp \
        $$PWD/options.cpp \
        $$PWD/mainwindowsnogui.cpp

# Some header files
HEADERS  += \
        $$PWD/mainwindow.h \
        $$PWD/options.h \
        $$PWD/tablewindow.h \
        $$PWD/stable.h \
        $$PWD/version.h \
        $$PWD/mainwindowsnogui.h

# Main forms
FORMS    += \
        $$PWD/mainwindow.ui \
        $$PWD/tablewindow.ui


