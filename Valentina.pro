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
    container/vcontainer.cpp

HEADERS  += mainwindow.h \
    widgets/vmaingraphicsscene.h \
    dialogs/dialogsinglepoint.h \
    options.h \
    tools/vtoolsimplepoint.h \
    widgets/vgraphicssimpletextitem.h \
    xml/vdomdocument.h \
    container/vpointf.h \
    container/vcontainer.h

FORMS    += mainwindow.ui \
    dialogs/dialogsinglepoint.ui

RESOURCES += \
    icon.qrc \
    cursor.qrc
