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
    dialogs/dialogsinglepoint.cpp \
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
    dialogs/dialogline.cpp \
    tools/vtoolalongline.cpp \
    dialogs/dialogtool.cpp \
    dialogs/dialogalongline.cpp \
    tools/vtoolshoulderpoint.cpp \
    dialogs/dialogshoulderpoint.cpp \
    tools/vtoolnormal.cpp \
    dialogs/dialognormal.cpp \
    tools/vtoolbisector.cpp \
    dialogs/dialogbisector.cpp \
    tools/vtoollinepoint.cpp \
    tools/vtoollineintersect.cpp \
    dialogs/dialoglineintersect.cpp \
    geometry/vspline.cpp \
    tools/vtoolsinglepoint.cpp \
    geometry/varc.cpp \
    widgets/vcontrolpointspline.cpp \
    tools/vtoolspline.cpp \
    dialogs/dialogspline.cpp \
    tools/vtoolarc.cpp \
    dialogs/dialogarc.cpp \
    geometry/vsplinepoint.cpp \
    geometry/vsplinepath.cpp \
    tools/vtoolsplinepath.cpp \
    dialogs/dialogsplinepath.cpp \
    widgets/vmaingraphicsscene.cpp \
    widgets/vmaingraphicsview.cpp \
    tools/vdatatool.cpp \
    xml/vtoolrecord.cpp \
    dialogs/dialoghistory.cpp

HEADERS  += mainwindow.h \
    widgets/vmaingraphicsscene.h \
    dialogs/dialogsinglepoint.h \
    options.h \
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
    dialogs/dialogline.h \
    tools/vtoolalongline.h \
    dialogs/dialogtool.h \
    dialogs/dialogalongline.h \
    tools/vtoolshoulderpoint.h \
    dialogs/dialogshoulderpoint.h \
    tools/vtoolnormal.h \
    dialogs/dialognormal.h \
    tools/vtoolbisector.h \
    dialogs/dialogbisector.h \
    tools/vtoollinepoint.h \
    tools/vtoollineintersect.h \
    dialogs/dialoglineintersect.h \
    geometry/vspline.h \
    tools/vtoolsinglepoint.h \
    geometry/varc.h \
    widgets/vcontrolpointspline.h \
    tools/vtoolspline.h \
    dialogs/dialogspline.h \
    tools/vtoolarc.h \
    dialogs/dialogarc.h \
    geometry/vsplinepoint.h \
    geometry/vsplinepath.h \
    tools/vtoolsplinepath.h \
    dialogs/dialogsplinepath.h \
    widgets/vmaingraphicsview.h \
    tools/vdatatool.h \
    xml/vtoolrecord.h \
    dialogs/dialoghistory.h

FORMS    += mainwindow.ui \
    dialogs/dialogsinglepoint.ui \
    dialogs/dialogincrements.ui \
    dialogs/dialogendline.ui \
    dialogs/dialogline.ui \
    dialogs/dialogalongline.ui \
    dialogs/dialogshoulderpoint.ui \
    dialogs/dialognormal.ui \
    dialogs/dialogbisector.ui \
    dialogs/dialoglineintersect.ui \
    dialogs/dialogspline.ui \
    dialogs/dialogarc.ui \
    dialogs/dialogsplinepath.ui \
    dialogs/dialoghistory.ui

RESOURCES += \
    icon.qrc \
    cursor.qrc

CONFIG(debug, debug|release){
    # Debug
    QMAKE_CXXFLAGS += -isystem "/usr/include/qt5" -isystem "/usr/include/qt5/QtWidgets" \
                      -isystem "/usr/include/qt5/QtXml" -isystem "/usr/include/qt5/QtGui" \
                      -isystem "/usr/include/qt5/QtCore" -O0 -Wall -Wextra -pedantic -Weffc++ \
                      -Woverloaded-virtual -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast \
                      -Wconversion -Wsign-conversion -Winit-self -Wunreachable-code -std=c++0x
}else{
    # Release
    QMAKE_CXXFLAGS += -O1 -std=c++0x
}
