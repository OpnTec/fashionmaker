# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

SOURCES += \
    $$PWD/vcontainer.cpp \
    $$PWD/calculator.cpp \
    $$PWD/vnodedetail.cpp \
    $$PWD/vtranslatevars.cpp \
    $$PWD/variables/varcradius.cpp \
    $$PWD/variables/vcurveangle.cpp \
    $$PWD/variables/vcurvelength.cpp \
    $$PWD/variables/vcurvevariable.cpp \
    $$PWD/variables/vincrement.cpp \
    $$PWD/variables/vinternalvariable.cpp \
    $$PWD/variables/vlineangle.cpp \
    $$PWD/variables/vlinelength.cpp \
    $$PWD/variables/vmeasurement.cpp \
    $$PWD/variables/vvariable.cpp \
    $$PWD/vformula.cpp \
    $$PWD/vgrainlinegeometry.cpp \
    $$PWD/variables/vcurveclength.cpp \
    $$PWD/variables/vellipticalarcradius.cpp \
    $$PWD/vpiece.cpp \
    $$PWD/vpiecenode.cpp \
    $$PWD/vpiecepath.cpp \
    $$PWD/vpiecelabeldata.cpp \
    $$PWD/vpatternlabeldata.cpp

win32-msvc*:SOURCES += $$PWD/stable.cpp

HEADERS += \
    $$PWD/vcontainer.h \
    $$PWD/stable.h \
    $$PWD/calculator.h \
    $$PWD/variables.h \
    $$PWD/vnodedetail.h \
    $$PWD/vnodedetail_p.h \
    $$PWD/vtranslatevars.h \
    $$PWD/variables/varcradius.h \
    $$PWD/variables/varcradius_p.h \
    $$PWD/variables/vcurveangle.h \
    $$PWD/variables/vcurvelength.h \
    $$PWD/variables/vcurvevariable.h \
    $$PWD/variables/vcurvevariable_p.h \
    $$PWD/variables/vincrement.h \
    $$PWD/variables/vincrement_p.h \
    $$PWD/variables/vinternalvariable.h \
    $$PWD/variables/vinternalvariable_p.h \
    $$PWD/variables/vlineangle.h \
    $$PWD/variables/vlineangle_p.h \
    $$PWD/variables/vlinelength.h \
    $$PWD/variables/vlinelength_p.h \
    $$PWD/variables/vmeasurement.h \
    $$PWD/variables/vmeasurement_p.h \
    $$PWD/variables/vvariable.h \
    $$PWD/variables/vvariable_p.h \
    $$PWD/vformula.h \
    $$PWD/vgrainlinegeometry.h \
    $$PWD/variables/vcurveclength.h \
    $$PWD/variables/vellipticalarcradius.h \
    $$PWD/vpiece.h \
    $$PWD/vpiece_p.h \
    $$PWD/vpiecenode.h \
    $$PWD/vpiecenode_p.h \
    $$PWD/vpiecepath.h \
    $$PWD/vpiecepath_p.h \
    $$PWD/vpiecelabeldata.h \
    $$PWD/vpatternlabeldata.h
