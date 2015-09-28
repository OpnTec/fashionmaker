# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

SOURCES += \
    $$PWD/stable.cpp \
    $$PWD/def.cpp \
    $$PWD/undoevent.cpp \
    $$PWD/backport/qcommandlineoption.cpp \
    $$PWD/backport/qcommandlineparser.cpp \
    $$PWD/vsettings.cpp \
    $$PWD/vabstractapplication.cpp \
    $$PWD/projectversion.cpp \
    $$PWD/vcommonsettings.cpp \
    $$PWD/vtapesettings.cpp

HEADERS += \
    $$PWD/stable.h \
    $$PWD/def.h \
    $$PWD/logging.h \
    $$PWD/vmath.h \
    $$PWD/undoevent.h \
    $$PWD/backport/qcommandlineoption.h \
    $$PWD/backport/qcommandlineparser.h \
    $$PWD/vsettings.h \
    $$PWD/vabstractapplication.h \
    $$PWD/projectversion.h \
    $$PWD/vcommonsettings.h \
    $$PWD/vtapesettings.h \
    $$PWD/debugbreak.h \
    $$PWD/vlockguard.h \
    $$PWD/vsysexits.h
