#-------------------------------------------------
#
# Project created by QtCreator 2014-08-26T14:18:08
#
#-------------------------------------------------

# File with common stuff for whole project
include(../../../Valentina.pri)

# Library use widgets
QT       += widgets

# We don't need gui library.
QT       -= gui

# Name of library
TARGET = vpropertyexplorer

# We want create library
TEMPLATE = lib

# We use C++11 standard
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
    plugins/vpointfproperty.cpp \
    plugins/vobjectproperty.cpp \
    stable.cpp

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
    plugins/vpointfproperty.h \
    plugins/vobjectproperty.h \
    vproperties.h \
    stable.h

# Set "make install" command for Unix-like systems.
unix{
    isEmpty(PREFIX){
        PREFIX = $$DEFAULT_PREFIX/lib
    }
}

unix:!macx{
    target.path = $$PREFIX/lib
    INSTALLS += target
}

# Set using ccache. Function enable_ccache() defined in Valentina.pri.
$$enable_ccache()

# Set precompiled headers. Function set_PCH() defined in Valentina.pri.
$$set_PCH()

CONFIG(debug, debug|release){
    # Debug mode
    unix {
        #Turn on compilers warnings.
        *-g++{
        QMAKE_CXXFLAGS += \
            # Key -isystem disable checking errors in system headers.
            -isystem "$${OUT_PWD}/$${MOC_DIR}" \
            $$GCC_DEBUG_CXXFLAGS # See Valentina.pri for more details.
        }
        clang*{
        QMAKE_CXXFLAGS += \
            # Key -isystem disable checking errors in system headers.
            -isystem "$${OUT_PWD}/$${MOC_DIR}" \
            $$CLANG_DEBUG_CXXFLAGS # See Valentina.pri for more details.
        }
    } else {
        *-g++{
        QMAKE_CXXFLAGS += $$CLANG_DEBUG_CXXFLAGS # See Valentina.pri for more details.
        }
    }

}else{
    # Release mode
    DEFINES += QT_NO_DEBUG_OUTPUT

    unix:!macx{
        # Turn on debug symbols in release mode on Unix systems.
        # On Mac OS X temporarily disabled. TODO: find way how to strip binary file.
        QMAKE_CXXFLAGS_RELEASE += -g -gdwarf-3
        # On Linux
        QMAKE_POST_LINK += objcopy --only-keep-debug $(DESTDIR)/$(TARGET) $(DESTDIR)/$(TARGET).debug &&
        QMAKE_POST_LINK += strip --strip-debug --strip-unneeded $(DESTDIR)/$(TARGET) &&
        QMAKE_POST_LINK += objcopy --add-gnu-debuglink $(DESTDIR)/$(TARGET).debug $(DESTDIR)/$(TARGET)
    }
}
