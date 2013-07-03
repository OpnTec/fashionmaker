#-------------------------------------------------
#
# Project created by QtCreator 2013-06-18T12:36:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Valentina
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    widgets/vmaingraphicsscene.cpp \
    dialogs/dialogsinglepoint.cpp

HEADERS  += mainwindow.h \
    widgets/vmaingraphicsscene.h \
    dialogs/dialogsinglepoint.h \
    options.h

FORMS    += mainwindow.ui \
    dialogs/dialogsinglepoint.ui

RESOURCES += \
    icon.qrc \
    cursor.qrc
