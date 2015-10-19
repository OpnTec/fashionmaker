# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

SOURCES += \
    $$PWD/dxflib/dl_dxf.cpp \
    $$PWD/dxflib/dl_writer_ascii.cpp \
    $$PWD/vdxfengine.cpp \
    $$PWD/vdxfpaintdevice.cpp \
    $$PWD/dxflib/dl_writer.cpp

win32-msvc*:SOURCES += $$PWD/stable.cpp

HEADERS += \
    $$PWD/stable.h \
    $$PWD/dxflib/dl_attributes.h \
    $$PWD/dxflib/dl_codes.h \
    $$PWD/dxflib/dl_creationadapter.h \
    $$PWD/dxflib/dl_creationinterface.h \
    $$PWD/dxflib/dl_dxf.h \
    $$PWD/dxflib/dl_entities.h \
    $$PWD/dxflib/dl_extrusion.h \
    $$PWD/dxflib/dl_global.h \
    $$PWD/dxflib/dl_writer.h \
    $$PWD/dxflib/dl_writer_ascii.h \
    $$PWD/vdxfengine.h \
    $$PWD/vdxfpaintdevice.h \
    $$PWD/dxfdef.h
