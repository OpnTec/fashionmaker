/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

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
        SplinePath,
        Detail
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
        Detail,
        NodePoint,
        NodeArc,
        NodeSpline,
        NodeSplinePath
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
