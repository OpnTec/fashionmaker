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

unix {
    target.path = /usr/lib
    INSTALLS += target

    *-g++{
        QMAKE_CC = ccache gcc
        QMAKE_CXX = ccache g++
    }
    clang*{
        QMAKE_CC = ccache clang
        QMAKE_CXX = ccache clang++
    }
}

unix:!macx:!clang*{
    CONFIG += precompile_header
    # Precompiled headers (PCH)
    PRECOMPILED_HEADER = stable.h
    win32-msvc* {
        PRECOMPILED_SOURCE = stable.cpp
    }
}

include(../../../Valentina.pri)

CONFIG(debug, debug|release){
    # Debug
    unix {
        *-g++{
        QMAKE_CXXFLAGS += \
            -isystem "/usr/include/qt5" \
            -isystem "/usr/include/qt5/QtWidgets" \
            -isystem "/usr/include/qt5/QtXml" \
            -isystem "/usr/include/qt5/QtGui" \
            -isystem "/usr/include/qt5/QtXmlPatterns" \
            -isystem "/usr/include/qt5/QtCore" \
            -isystem "$${OUT_PWD}/$${MOC_DIR}" \
            $$GCC_CXXFLAGS
        }
        #Turn on Clang warnings
        clang*{
        QMAKE_CXXFLAGS += \
            -isystem "/usr/include/qt5" \
            -isystem "/usr/include/qt5/QtWidgets" \
            -isystem "/usr/include/qt5/QtXml" \
            -isystem "/usr/include/qt5/QtGui" \
            -isystem "/usr/include/qt5/QtCore" \
            -isystem "/usr/include/qt5/QtXmlPatterns" \
            -isystem "$${OUT_PWD}/$${MOC_DIR}" \
            $$CLANG_CXXFLAGS
        }
    } else {
        *-g++{#Don't use additional GCC keys on Windows system.
        QMAKE_CXXFLAGS += -O0 -Wall -Wextra -pedantic
        }
    }

}else{
    # Release
    DEFINES += QT_NO_DEBUG_OUTPUT

    unix:QMAKE_CXXFLAGS_RELEASE += -g
    unix:macx{
        # On Mac
        QMAKE_POST_LINK += gobjcopy --only-keep-debug $(DESTDIR)/$(TARGET) $(DESTDIR)/$(TARGET).debug &&
        QMAKE_POST_LINK += strip --strip-debug --strip-unneeded $(DESTDIR)/$(TARGET) &&
        QMAKE_POST_LINK += gobjcopy --add-gnu-debuglink $(DESTDIR)/$(TARGET).debug $(DESTDIR)/$(TARGET)
    } else {
        # On Linux
        unix:QMAKE_POST_LINK += objcopy --only-keep-debug $(DESTDIR)/$(TARGET) $(DESTDIR)/$(TARGET).debug &&
        unix:QMAKE_POST_LINK += strip --strip-debug --strip-unneeded $(DESTDIR)/$(TARGET) &&
        unix:QMAKE_POST_LINK += objcopy --add-gnu-debuglink $(DESTDIR)/$(TARGET).debug $(DESTDIR)/$(TARGET)
    }
}
