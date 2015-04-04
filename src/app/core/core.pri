# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

HEADERS += \
    $$PWD/vapplication.h \
    $$PWD/undoevent.h \
    $$PWD/vsettings.h \
    $$PWD/backport/qcommandlineoption.h \
    $$PWD/backport/qcommandlineparser.h \
    $$PWD/backport/qlockfile.h \
    $$PWD/backport/qlockfile_p.h \
    $$PWD/backport/qfilesystementry_p.h \
    $$PWD/backport/qfsfileengine_p.h \
    $$PWD/backport/qabstractfileengine_p.h \
    $$PWD/backport/qresource_p.h \
    $$PWD/backport/qresource.h \
    $$PWD/backport/qfilesystemmetadata_p.h \
    $$PWD/backport/qfilesystemengine_p.h \
    $$PWD/backport/qsystemerror_p.h \
    $$PWD/backport/qfsfileengine_iterator_p.h \
    $$PWD/backport/qfilesystemiterator_p.h \
    $$PWD/backport/qfileinfo_p.h \
    $$PWD/backport/qmutexpool_p.h \
    $$PWD/backport/qsystemlibrary_p.h \

unix:!macx {
    HEADERS += \
        $$PWD/backport/qcore_unix_p.h
}

unix:macx {
    HEADERS += \
        $$PWD/backport/qcore_mac_p.h
}

SOURCES += \
    $$PWD/vapplication.cpp \
    $$PWD/undoevent.cpp \
    $$PWD/vsettings.cpp \
    $$PWD/backport/qcommandlineoption.cpp \
    $$PWD/backport/qcommandlineparser.cpp \
    $$PWD/backport/qlockfile.cpp \
    $$PWD/backport/qfilesystementry.cpp \
    $$PWD/backport/qfsfileengine.cpp \
    $$PWD/backport/qabstractfileengine.cpp \
    $$PWD/backport/qfilesystemengine.cpp \
    $$PWD/backport/qsystemerror.cpp \
    $$PWD/backport/qfsfileengine_iterator.cpp \
    $$PWD/backport/qmutexpool.cpp

unix:!macx {
    SOURCES += \
        $$PWD/backport/qlockfile_unix.cpp \
        $$PWD/backport/qfilesystemiterator_unix.cpp \
        $$PWD/backport/qfsfileengine_unix.cpp \
        $$PWD/backport/qfilesystemengine_unix.cpp \
        $$PWD/backport/qcore_unix.cpp
}

unix:macx {
    SOURCES += \
        $$PWD/backport/qcore_mac.cpp
}

win32 {
    SOURCES += \
        $$PWD/backport/qlockfile_win.cpp \
        $$PWD/backport/qfilesystemiterator_win.cpp \
        $$PWD/backport/qfsfileengine_win.cpp \
        $$PWD/backport/qfilesystemengine_win.cpp
}
