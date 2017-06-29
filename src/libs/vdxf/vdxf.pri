# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

SOURCES += \
    $$PWD/vdxfengine.cpp \
    $$PWD/vdxfpaintdevice.cpp \
    $$PWD/libdxfrw/intern/drw_dbg.cpp \
    $$PWD/libdxfrw/intern/drw_textcodec.cpp \
    $$PWD/libdxfrw/intern/dwgbuffer.cpp \
    $$PWD/libdxfrw/intern/dwgreader.cpp \
    $$PWD/libdxfrw/intern/dwgreader15.cpp \
    $$PWD/libdxfrw/intern/dwgreader18.cpp \
    $$PWD/libdxfrw/intern/dwgreader21.cpp \
    $$PWD/libdxfrw/intern/dwgreader24.cpp \
    $$PWD/libdxfrw/intern/dwgreader27.cpp \
    $$PWD/libdxfrw/intern/dwgutil.cpp \
    $$PWD/libdxfrw/intern/dxfreader.cpp \
    $$PWD/libdxfrw/intern/dxfwriter.cpp \
    $$PWD/libdxfrw/intern/rscodec.cpp \
    $$PWD/libdxfrw/drw_classes.cpp \
    $$PWD/libdxfrw/drw_entities.cpp \
    $$PWD/libdxfrw/drw_header.cpp \
    $$PWD/libdxfrw/drw_objects.cpp \
    $$PWD/libdxfrw/libdwgr.cpp \
    $$PWD/libdxfrw/libdxfrw.cpp \
    $$PWD/dxiface.cpp \
    $$PWD/dxfdef.cpp

win32-msvc*:SOURCES += $$PWD/stable.cpp

HEADERS += \
    $$PWD/stable.h \
    $$PWD/vdxfengine.h \
    $$PWD/vdxfpaintdevice.h \
    $$PWD/dxfdef.h \
    $$PWD/libdxfrw/intern/drw_cptable932.h \
    $$PWD/libdxfrw/intern/drw_cptable936.h \
    $$PWD/libdxfrw/intern/drw_cptable949.h \
    $$PWD/libdxfrw/intern/drw_cptable950.h \
    $$PWD/libdxfrw/intern/drw_cptables.h \
    $$PWD/libdxfrw/intern/drw_dbg.h \
    $$PWD/libdxfrw/intern/drw_textcodec.h \
    $$PWD/libdxfrw/intern/dwgbuffer.h \
    $$PWD/libdxfrw/intern/dwgreader.h \
    $$PWD/libdxfrw/intern/dwgreader15.h \
    $$PWD/libdxfrw/intern/dwgreader18.h \
    $$PWD/libdxfrw/intern/dwgreader21.h \
    $$PWD/libdxfrw/intern/dwgreader24.h \
    $$PWD/libdxfrw/intern/dwgreader27.h \
    $$PWD/libdxfrw/intern/dwgutil.h \
    $$PWD/libdxfrw/intern/dxfreader.h \
    $$PWD/libdxfrw/intern/dxfwriter.h \
    $$PWD/libdxfrw/intern/rscodec.h \
    $$PWD/libdxfrw/drw_base.h \
    $$PWD/libdxfrw/drw_classes.h \
    $$PWD/libdxfrw/drw_entities.h \
    $$PWD/libdxfrw/drw_header.h \
    $$PWD/libdxfrw/drw_interface.h \
    $$PWD/libdxfrw/drw_objects.h \
    $$PWD/libdxfrw/libdwgr.h \
    $$PWD/libdxfrw/libdxfrw.h \
    $$PWD/libdxfrw/main_doc.h \
    $$PWD/dxiface.h
