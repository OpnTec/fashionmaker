/************************************************************************
 **
 **  @file   options.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#ifndef OPTIONS_H
#define OPTIONS_H

#include <QFlags>

#define PrintDPI 96
#define PaperSize 50000
#define toPixel(mm) ((mm / 25.4) * PrintDPI)
#define toMM(pix) ((pix / PrintDPI) * 25.4)
#define widthMainLine 1.2
#define widthHairLine widthMainLine/3

namespace Scene
{
    /**
     * @brief The Scene enum
     */
    enum Scene { Point, Line, Spline, Arc, SplinePath, Detail };
    Q_DECLARE_FLAGS(Scenes, Scene)
}
Q_DECLARE_OPERATORS_FOR_FLAGS( Scene::Scenes )

namespace Tool
{
    /**
     * @brief The Tool enum
     */
    enum Tool
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
        CutSplineTool,
        ArcTool,
        SplinePathTool,
        CutSplinePathTool,
        PointOfContact,
        Detail,
        NodePoint,
        NodeArc,
        NodeSpline,
        NodeSplinePath,
        Height,
        Triangle,
        PointOfIntersection,
        UnionDetails
    };
    Q_DECLARE_FLAGS(Tools, Tool)

    /**
     * @brief The Source enum
     */
    enum Source { FromGui, FromFile, FromTool };
    Q_DECLARE_FLAGS(Sources, Source)
}
Q_DECLARE_OPERATORS_FOR_FLAGS( Tool::Tools )
Q_DECLARE_OPERATORS_FOR_FLAGS( Tool::Sources )

namespace Draw
{
    /**
     * @brief The Draw enum
     */
    enum Draw { Calculation, Modeling };
    Q_DECLARE_FLAGS(Draws, Draw)
}
Q_DECLARE_OPERATORS_FOR_FLAGS(Draw::Draws)

#endif // OPTIONS_H
