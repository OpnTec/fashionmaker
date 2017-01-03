# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

SOURCES += \
    $$PWD/qmuparser.cpp \
    $$PWD/qmuparsertokenreader.cpp \
    $$PWD/qmuparsererror.cpp \
    $$PWD/qmuparsercallback.cpp \
    $$PWD/qmuparserbytecode.cpp \
    $$PWD/qmuparserbase.cpp \
    $$PWD/qmuparsertest.cpp \
    $$PWD/qmutranslation.cpp \
    $$PWD/qmuformulabase.cpp \
    $$PWD/qmutokenparser.cpp \
    $$PWD/qmudef.cpp

win32-msvc*:SOURCES += $$PWD/stable.cpp

HEADERS += \
    $$PWD/qmuparser.h\
    $$PWD/qmuparser_global.h \
    $$PWD/qmuparsertokenreader.h \
    $$PWD/qmuparsertoken.h \
    $$PWD/qmuparserfixes.h \
    $$PWD/qmuparsererror.h \
    $$PWD/qmuparserdef.h \
    $$PWD/qmuparsercallback.h \
    $$PWD/qmuparserbytecode.h \
    $$PWD/qmuparserbase.h \
    $$PWD/qmuparsertest.h \
    $$PWD/stable.h \
    $$PWD/qmutranslation.h \
    $$PWD/qmudef.h \
    $$PWD/qmuformulabase.h \
    $$PWD/qmutokenparser.h
