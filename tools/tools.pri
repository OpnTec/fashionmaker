HEADERS += \
    tools/vtooldetail.h \
    tools/vdatatool.h \
    tools/vabstracttool.h \
    tools/tools.h

SOURCES += \
    tools/vtooldetail.cpp \
    tools/vdatatool.cpp \
    tools/vabstracttool.cpp

include(drawTools/drawTools.pri)
include(modelingTools/modelingTools.pri)
include(nodeDetails/nodeDetails.pri)
