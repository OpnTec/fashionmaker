#-------------------------------------------------
#
# Project created by QtCreator 2014-08-26T14:18:08
#
#-------------------------------------------------

QT       += widgets

QT       -= gui

TARGET = vpropertyexplorer
TEMPLATE = lib

CONFIG += c++11

DEFINES += VPROPERTYEXPLORER_LIBRARY

# directory for executable file
DESTDIR = bin

# files created moc
MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj

SOURCES += \
    vproperty.cpp \
    vpropertydelegate.cpp \
    vpropertyfactorymanager.cpp \
    vpropertyformview.cpp \
    vpropertyformwidget.cpp \
    vpropertymodel.cpp \
    vpropertyset.cpp \
    vpropertytreeview.cpp \
    vserializedproperty.cpp \
    plugins/vwidgetproperty.cpp \
    plugins/vemptyproperty.cpp \
    plugins/vboolproperty.cpp \
    plugins/vshortcutproperty.cpp \
    plugins/vcolorproperty.cpp \
    plugins/vshortcutpropertyeditor.cpp \
    plugins/venumproperty.cpp \
    plugins/vfileproperty.cpp \
    plugins/vcolorpropertyeditor.cpp \
    plugins/vfilepropertyeditor.cpp \
    plugins/vnumberproperty.cpp \
    plugins/Vector3d/vvector3dproperty.cpp \
    vstandardpropertyfactory.cpp \
    plugins/vstringproperty.cpp \
    plugins/vpointfproperty.cpp

HEADERS +=\
        vpropertyexplorer_global.h \
    vpropertyfactorymanager_p.h \
    vpropertytreeview_p.h \
    vpropertyset_p.h \
    vabstractpropertyfactory.h \
    vfileproperty_p.h \
    vwidgetproperty_p.h \
    vpropertymodel_p.h \
    vstandardpropertyfactory.h \
    vpropertyformview_p.h \
    vpropertytreeview.h \
    vpropertyformwidget_p.h \
    vpropertydelegate.h \
    vproperty_p.h \
    vpropertyformwidget.h \
    vpropertyformview.h \
    vpropertyset.h \
    vpropertymodel.h \
    vproperty.h \
    plugins/vwidgetproperty.h \
    plugins/vcolorproperty.h \
    plugins/vboolproperty.h \
    plugins/vcolorpropertyeditor.h \
    plugins/vshortcutpropertyeditor.h \
    plugins/vemptyproperty.h \
    plugins/vshortcutproperty.h \
    plugins/venumproperty.h \
    plugins/vfilepropertyeditor.h \
    plugins/vfileproperty.h \
    plugins/vnumberproperty.h \
    plugins/Vector3d/vvector3dproperty.h \
    vpropertyfactorymanager.h \
    vserializedproperty.h \
    plugins/vstringproperty.h \
    plugins/vpointfproperty.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

QMAKE_CXXFLAGS += -gdwarf-3
