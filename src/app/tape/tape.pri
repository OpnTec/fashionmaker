# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

SOURCES  += \
    $$PWD/main.cpp \
    $$PWD/tmainwindow.cpp \
    $$PWD/mapplication.cpp \
    $$PWD/dialogs/dialogabouttape.cpp \
    $$PWD/dialogs/dialognewmeasurements.cpp \
    $$PWD/dialogs/dialogmdatabase.cpp \
    $$PWD/vlitepattern.cpp \
    $$PWD/dialogs/dialogtapepreferences.cpp \
    $$PWD/dialogs/configpages/tapepreferencesconfigurationpage.cpp \
    $$PWD/dialogs/configpages/tapepreferencespathpage.cpp

win32-msvc*:SOURCES += $$PWD/stable.cpp

HEADERS  += \
    $$PWD/tmainwindow.h \
    $$PWD/stable.h \
    $$PWD/mapplication.h \
    $$PWD/dialogs/dialogabouttape.h \
    $$PWD/dialogs/dialognewmeasurements.h \
    $$PWD/dialogs/dialogmdatabase.h \
    $$PWD/version.h \
    $$PWD/vlitepattern.h \
    $$PWD/dialogs/dialogtapepreferences.h \
    $$PWD/dialogs/configpages/tapepreferencesconfigurationpage.h \
    $$PWD/dialogs/configpages/tapepreferencespathpage.h

FORMS    += \
    $$PWD/tmainwindow.ui \
    $$PWD/dialogs/dialogabouttape.ui \
    $$PWD/dialogs/dialognewmeasurements.ui \
    $$PWD/dialogs/dialogmdatabase.ui \
    $$PWD/dialogs/dialogtapepreferences.ui \
    $$PWD/dialogs/configpages/tapepreferencesconfigurationpage.ui \
    $$PWD/dialogs/configpages/tapepreferencespathpage.ui
