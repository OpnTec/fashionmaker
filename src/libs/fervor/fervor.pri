# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

SOURCES += \
    $$PWD/fvupdatewindow.cpp \
    $$PWD/fvupdater.cpp \
    $$PWD/fvavailableupdate.cpp

*msvc*:SOURCES += $$PWD/stable.cpp

HEADERS += \
    $$PWD/fvupdatewindow.h \
    $$PWD/fvupdater.h \
    $$PWD/fvavailableupdate.h \
    $$PWD/stable.h

FORMS += \
    $$PWD/fvupdatewindow.ui
