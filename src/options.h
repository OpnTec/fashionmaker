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
#include <QString>

#define SceneSize 50000

extern const QString translationsPath;

namespace Valentina
{
    /**
     * @brief The Scene enum
     */
    enum Scene { Point, Line, Spline, Arc, SplinePath, Detail };
    Q_DECLARE_FLAGS(Scenes, Scene)

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
        CutArcTool,
        ArcTool,
        SplinePathTool,
        CutSplinePathTool,
        PointOfContact,
        DetailTool,
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

    /**
     * @brief The Draw enum
     */
    enum Draw { Calculation, Modeling };
    Q_DECLARE_FLAGS(Draws, Draw)

    /**
     * @brief The Unit enum
     */
    enum Unit { Mm, Cm, In };
    Q_DECLARE_FLAGS(Units, Unit)
}
Q_DECLARE_OPERATORS_FOR_FLAGS( Valentina::Scenes )
Q_DECLARE_OPERATORS_FOR_FLAGS( Valentina::Tools )
Q_DECLARE_OPERATORS_FOR_FLAGS( Valentina::Sources )
Q_DECLARE_OPERATORS_FOR_FLAGS( Valentina::Draws )
Q_DECLARE_OPERATORS_FOR_FLAGS( Valentina::Units )

extern Valentina::Units patternUnit;

namespace Pattern
{
    /**
     * @brief The Scene enum
     */
    enum Measurement { Standard, Individual };
    Q_DECLARE_FLAGS(Measurements, Measurement)
}
Q_DECLARE_OPERATORS_FOR_FLAGS( Pattern::Measurements )

extern Pattern::Measurements patternType;

#define PrintDPI 96.0

inline double toPixel(double unit)
{
    double result = 0;
    switch (patternUnit)
    {
        case Valentina::Mm:
            result = (unit / 25.4) * PrintDPI;
            break;
        case Valentina::Cm:
            result = ((unit * 10.0) / 25.4) * PrintDPI;
            break;
        case Valentina::In:
            result = unit * PrintDPI;
            break;
        default:
            break;
    }
    return result;
}

inline double fromPixel(double pix)
{
    double result = 0;
    switch (patternUnit)
    {
        case Valentina::Mm:
            result = (pix / PrintDPI) * 25.4;
            break;
        case Valentina::Cm:
            result = ((pix / PrintDPI) * 25.4) / 10.0;
            break;
        case Valentina::In:
            result = pix / PrintDPI;
            break;
        default:
            break;
    }
    return result;
}

#define widthMainLine 1.2 //mm
#define widthHairLine widthMainLine/3

#endif // OPTIONS_H
