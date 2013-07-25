#-------------------------------------------------
#
# Project created by QtCreator 2013-06-18T12:36:43
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Valentina
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    widgets/vmaingraphicsscene.cpp \
    dialogs/dialogsinglepoint.cpp \
    tools/vtoolsimplepoint.cpp \
    widgets/vgraphicssimpletextitem.cpp \
    xml/vdomdocument.cpp \
    container/vpointf.cpp \
    container/vcontainer.cpp \
    tools/vtoolpoint.cpp \
    container/calculator.cpp \
    dialogs/dialogincrements.cpp \
    container/vstandarttablecell.cpp \
    container/vincrementtablerow.cpp \
    widgets/delegate.cpp \
    widgets/doubledelegate.cpp \
    dialogs/dialogendline.cpp \
    tools/vtoolendline.cpp \
    tools/vtoolline.cpp \
    tools/vabstracttool.cpp \
    dialogs/dialogline.cpp

HEADERS  += mainwindow.h \
    widgets/vmaingraphicsscene.h \
    dialogs/dialogsinglepoint.h \
    options.h \
    tools/vtoolsimplepoint.h \
    widgets/vgraphicssimpletextitem.h \
    xml/vdomdocument.h \
    container/vpointf.h \
    container/vcontainer.h \
    tools/vtoolpoint.h \
    container/calculator.h \
    dialogs/dialogincrements.h \
    container/vstandarttablecell.h \
    container/vincrementtablerow.h \
    widgets/delegate.h \
    widgets/doubledelegate.h \
    dialogs/dialogendline.h \
    tools/vtoolendline.h \
    tools/vtoolline.h \
    tools/vabstracttool.h \
    dialogs/dialogline.h

FORMS    += mainwindow.ui \
    dialogs/dialogsinglepoint.ui \
    dialogs/dialogincrements.ui \
    dialogs/dialogendline.ui \
    dialogs/dialogline.ui

RESOURCES += \
    icon.qrc \
    cursor.qrc
