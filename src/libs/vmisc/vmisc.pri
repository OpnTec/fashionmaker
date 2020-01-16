# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

SOURCES += \
    $$PWD/def.cpp \
    $$PWD/vsettings.cpp \
    $$PWD/vabstractapplication.cpp \
    $$PWD/projectversion.cpp \
    $$PWD/vcommonsettings.cpp \
    $$PWD/vtapesettings.cpp \
    $$PWD/commandoptions.cpp \
    $$PWD/qxtcsvmodel.cpp \
    $$PWD/vtablesearch.cpp \
    $$PWD/dialogs/dialogexporttocsv.cpp \
    $$PWD/literals.cpp \
    $$PWD/vmodifierkey.cpp

*msvc*:SOURCES += $$PWD/stable.cpp

contains(DEFINES, APPIMAGE) {
    SOURCES += \
        $$PWD/binreloc.c \
        $$PWD/appimage.cpp
}

HEADERS += \
    $$PWD/compatibility.h \
    $$PWD/stable.h \
    $$PWD/def.h \
    $$PWD/vmath.h \
    $$PWD/vsettings.h \
    $$PWD/vabstractapplication.h \
    $$PWD/projectversion.h \
    $$PWD/vcommonsettings.h \
    $$PWD/vtapesettings.h \
    $$PWD/debugbreak.h \
    $$PWD/vlockguard.h \
    $$PWD/vsysexits.h \
    $$PWD/commandoptions.h \
    $$PWD/qxtcsvmodel.h \
    $$PWD/vtablesearch.h \
    $$PWD/diagnostic.h \
    $$PWD/dialogs/dialogexporttocsv.h \
    $$PWD/customevents.h \
    $$PWD/defglobal.h \
    $$PWD/backport/qoverload.h \
    $$PWD/testvapplication.h \
    $$PWD/literals.h \
    $$PWD/qt_dispatch/qt_dispatch.h \
    $$PWD/vdatastreamenum.h \
    $$PWD/vmodifierkey.h \
    $$PWD/typedef.h \
    $$PWD/backport/qscopeguard.h

contains(DEFINES, APPIMAGE) {
    SOURCES += \
        $$PWD/binreloc.h \
        $$PWD/appimage.h
}

# Qt's versions
# 5.2.0, 5.2.1
# 5.3.0, 5.3.1, 5.3.2


contains(QT_VERSION, ^5\\.[0-2]\\.[0-2]$) { # Since Qt 5.3.0
    HEADERS +=

    SOURCES +=
}

FORMS    += \
    $$PWD/dialogs/dialogexporttocsv.ui
