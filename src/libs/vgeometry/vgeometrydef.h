/************************************************************************
 **
 **  @file   vgeometrydef.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 5, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef VGEOMETRYDEF_H
#define VGEOMETRYDEF_H

#include <QVector>
#include <QPolygonF>

enum class GOType : char
{
    Point,
    Arc,
    EllipticalArc,
    Spline,
    SplinePath,
    CubicBezier,
    CubicBezierPath,
    PlaceLabel,
    Unknown
};
enum class SplinePointPosition : char { FirstPoint, LastPoint };

// Keep synchronized with XSD schema
enum class PlaceLabelType :  unsigned char
{
    Segment= 0,
    Rectangle = 1,
    Cross = 2,
    Tshaped = 3,
    Doubletree = 4,
    Corner = 5,
    Triangle = 6,
    Hshaped = 7,
    Button = 8
};

typedef QVector<QPolygonF> PlaceLabelImg;

struct VLayoutPlaceLabel
{
    VLayoutPlaceLabel()
        : center(),
          type(PlaceLabelType::Button),
          shape()
    {}

    QPointF        center;
    PlaceLabelType type;
    PlaceLabelImg  shape;
};

#endif // VGEOMETRYDEF_H
