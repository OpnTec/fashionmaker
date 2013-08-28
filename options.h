#ifndef OPTIONS_H
#define OPTIONS_H

#include <QtGlobal>

#define PrintDPI 96
#define PaperSize 50000
#define toPixel(mm) ((mm / 25.4) * PrintDPI)
#define toMM(pix) ((pix / PrintDPI) * 25.4)
#define widthMainLine toPixel(0.8)
#define widthHairLine widthMainLine/3


namespace Scene{
    enum Type
    {
        Point,
        Line,
        Spline,
        Arc,
        SplinePath
    };
}

namespace Tools{
    enum Enum
    {
        ArrowTool,
        SinglePointTool,
        EndLineTool,
        LineTool,
        AlongLineTool,
        ShoulderPointTool,
        NormalTool,
        BisectorTool,
        LineIntersectTool,
        SplineTool,
        ArcTool,
        SplinePathTool,
        PointOfContact,
        Detail
    };
}

namespace Draw{
    enum Mode
    {
        Calculation,
        Modeling
    };
}

#endif // OPTIONS_H
