#-------------------------------------------------
#
# Project created by QtCreator 2013-06-18T12:36:43
#
#-------------------------------------------------

# Use out-of-source builds (shadow builds)

QT       += core gui widgets xml svg printsupport

TARGET = Valentina
TEMPLATE = app
CONFIG -= debug_and_release debug_and_release_target

SOURCES += main.cpp\
        mainwindow.cpp \
    dialogs/dialogsinglepoint.cpp \
    widgets/vgraphicssimpletextitem.cpp \
    xml/vdomdocument.cpp \
    container/vpointf.cpp \
    container/vcontainer.cpp \
    tools/drawTools/vtoolpoint.cpp \
    container/calculator.cpp \
    dialogs/dialogincrements.cpp \
    container/vstandarttablecell.cpp \
    container/vincrementtablerow.cpp \
    widgets/doubledelegate.cpp \
    dialogs/dialogendline.cpp \
    tools/drawTools/vtoolendline.cpp \
    tools/drawTools/vtoolline.cpp \
    tools/vabstracttool.cpp \
    dialogs/dialogline.cpp \
    tools/drawTools/vtoolalongline.cpp \
    dialogs/dialogtool.cpp \
    dialogs/dialogalongline.cpp \
    tools/drawTools/vtoolshoulderpoint.cpp \
    dialogs/dialogshoulderpoint.cpp \
    tools/drawTools/vtoolnormal.cpp \
    dialogs/dialognormal.cpp \
    tools/drawTools/vtoolbisector.cpp \
    dialogs/dialogbisector.cpp \
    tools/drawTools/vtoollinepoint.cpp \
    tools/drawTools/vtoollineintersect.cpp \
    dialogs/dialoglineintersect.cpp \
    geometry/vspline.cpp \
    tools/drawTools/vtoolsinglepoint.cpp \
    geometry/varc.cpp \
    widgets/vcontrolpointspline.cpp \
    tools/drawTools/vtoolspline.cpp \
    dialogs/dialogspline.cpp \
    tools/drawTools/vtoolarc.cpp \
    dialogs/dialogarc.cpp \
    geometry/vsplinepoint.cpp \
    geometry/vsplinepath.cpp \
    tools/drawTools/vtoolsplinepath.cpp \
    dialogs/dialogsplinepath.cpp \
    widgets/vmaingraphicsscene.cpp \
    widgets/vmaingraphicsview.cpp \
    tools/vdatatool.cpp \
    xml/vtoolrecord.cpp \
    dialogs/dialoghistory.cpp \
    tools/drawTools/vtoolpointofcontact.cpp \
    dialogs/dialogpointofcontact.cpp \
    geometry/vnodedetail.cpp \
    geometry/vdetail.cpp \
    dialogs/dialogdetail.cpp \
    tools/vtooldetail.cpp \
    widgets/vtablegraphicsview.cpp \
    widgets/vitem.cpp \
    tablewindow.cpp \
    tools/nodeDetails/vnodearc.cpp \
    tools/nodeDetails/vnodepoint.cpp \
    tools/nodeDetails/vnodespline.cpp \
    tools/nodeDetails/vnodesplinepath.cpp \
    tools/drawTools/vdrawtool.cpp \
    tools/nodeDetails/vabstractnode.cpp \
    tools/modelingTools/vmodelingtool.cpp \
    tools/modelingTools/vmodelingalongline.cpp \
    tools/modelingTools/vmodelingarc.cpp \
    tools/modelingTools/vmodelingbisector.cpp \
    tools/modelingTools/vmodelingendline.cpp \
    tools/modelingTools/vmodelingline.cpp \
    tools/modelingTools/vmodelinglineintersect.cpp \
    tools/modelingTools/vmodelinglinepoint.cpp \
    tools/modelingTools/vmodelingnormal.cpp \
    tools/modelingTools/vmodelingpoint.cpp \
    tools/modelingTools/vmodelingpointofcontact.cpp \
    tools/modelingTools/vmodelingshoulderpoint.cpp \
    tools/modelingTools/vmodelingspline.cpp \
    tools/modelingTools/vmodelingsplinepath.cpp

HEADERS  += mainwindow.h \
    widgets/vmaingraphicsscene.h \
    dialogs/dialogsinglepoint.h \
    options.h \
    widgets/vgraphicssimpletextitem.h \
    xml/vdomdocument.h \
    container/vpointf.h \
    container/vcontainer.h \
    tools/drawTools/vtoolpoint.h \
    container/calculator.h \
    dialogs/dialogincrements.h \
    container/vstandarttablecell.h \
    container/vincrementtablerow.h \
    widgets/doubledelegate.h \
    dialogs/dialogendline.h \
    tools/drawTools/vtoolendline.h \
    tools/drawTools/vtoolline.h \
    tools/vabstracttool.h \
    dialogs/dialogline.h \
    tools/drawTools/vtoolalongline.h \
    dialogs/dialogtool.h \
    dialogs/dialogalongline.h \
    tools/drawTools/vtoolshoulderpoint.h \
    dialogs/dialogshoulderpoint.h \
    tools/drawTools/vtoolnormal.h \
    dialogs/dialognormal.h \
    tools/drawTools/vtoolbisector.h \
    dialogs/dialogbisector.h \
    tools/drawTools/vtoollinepoint.h \
    tools/drawTools/vtoollineintersect.h \
    dialogs/dialoglineintersect.h \
    geometry/vspline.h \
    tools/drawTools/vtoolsinglepoint.h \
    geometry/varc.h \
    widgets/vcontrolpointspline.h \
    tools/drawTools/vtoolspline.h \
    dialogs/dialogspline.h \
    tools/drawTools/vtoolarc.h \
    dialogs/dialogarc.h \
    geometry/vsplinepoint.h \
    geometry/vsplinepath.h \
    tools/drawTools/vtoolsplinepath.h \
    dialogs/dialogsplinepath.h \
    widgets/vmaingraphicsview.h \
    tools/vdatatool.h \
    xml/vtoolrecord.h \
    dialogs/dialoghistory.h \
    tools/drawTools/vtoolpointofcontact.h \
    dialogs/dialogpointofcontact.h \
    geometry/vnodedetail.h \
    geometry/vdetail.h \
    dialogs/dialogdetail.h \
    tools/vtooldetail.h \
    widgets/vtablegraphicsview.h \
    widgets/vitem.h \
    tablewindow.h \
    tools/tools.h \
    tools/drawTools/drawtools.h \
    tools/nodeDetails/nodedetails.h \
    tools/nodeDetails/vnodearc.h \
    tools/nodeDetails/vnodepoint.h \
    tools/nodeDetails/vnodespline.h \
    tools/nodeDetails/vnodesplinepath.h \
    stable.h \
    tools/drawTools/vdrawtool.h \
    tools/nodeDetails/vabstractnode.h \
    dialogs/dialogs.h \
    tools/modelingTools/modelingtools.h \
    tools/modelingTools/vmodelingtool.h \
    tools/modelingTools/vmodelingalongline.h \
    tools/modelingTools/vmodelingarc.h \
    tools/modelingTools/vmodelingbisector.h \
    tools/modelingTools/vmodelingendline.h \
    tools/modelingTools/vmodelingline.h \
    tools/modelingTools/vmodelinglineintersect.h \
    tools/modelingTools/vmodelinglinepoint.h \
    tools/modelingTools/vmodelingnormal.h \
    tools/modelingTools/vmodelingpoint.h \
    tools/modelingTools/vmodelingpointofcontact.h \
    tools/modelingTools/vmodelingshoulderpoint.h \
    tools/modelingTools/vmodelingspline.h \
    tools/modelingTools/vmodelingsplinepath.h

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
    dialogs/dialoghistory.ui \
    dialogs/dialogpointofcontact.ui \
    dialogs/dialogdetail.ui \
    tablewindow.ui

RESOURCES += \
    icon.qrc \
    cursor.qrc

# directory for executable file
DESTDIR = bin

# files created moc
MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj

# files created rcc
RCC_DIR = rcc

# files created uic
UI_DIR = uic

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = stable.h

CONFIG(debug, debug|release){
    # Debug
    QMAKE_CXXFLAGS += -isystem "/usr/include/qt5" -isystem "/usr/include/qt5/QtWidgets" \
                      -isystem "/usr/include/qt5/QtXml" -isystem "/usr/include/qt5/QtGui" \
                      -isystem "/usr/include/qt5/QtCore" -O0 -Wall -Wextra -pedantic -Weffc++ \
                      -isystem "$$OUT_PWD/uic" \
                      -Woverloaded-virtual -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast \
                      -Wconversion -Wsign-conversion -Winit-self -Wunreachable-code -std=c++0x
}else{
    # Release
    QMAKE_CXXFLAGS += -O1 -std=c++0x
}
