/************************************************************************
 **
 **  @file   vistoolellipticalarc.cpp
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   24 10, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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
#include "vistoolellipticalarc.h"
#include <QPainterPath>
#include <QPointF>
#include <QSharedPointer>
#include <Qt>
#include <new>

#include "../ifc/ifcdef.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vellipticalarc.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../visualization.h"
#include "vispath.h"

VisToolEllipticalArc::VisToolEllipticalArc(const VContainer *data, QGraphicsItem *parent)
    :VisPath(data, parent), arcCenter(nullptr), radius1(0), radius2(0), f1(0), f2(0), rotationAngle(0)
{
    arcCenter = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolEllipticalArc::RefreshGeometry()
{
    if (object1Id > NULL_ID)
    {
        const QSharedPointer<VPointF> first = Visualization::data->GeometricObject<VPointF>(object1Id);
        DrawPoint(arcCenter, static_cast<QPointF>(*first), supportColor);

        if (not qFuzzyIsNull(radius1) && not qFuzzyIsNull(radius2) && f1 >= 0 && f2 >= 0 && rotationAngle >= 0)
        {
            VEllipticalArc elArc = VEllipticalArc(*first, radius1, radius2, f1, f2, rotationAngle);
            DrawPath(this, elArc.GetPath(), elArc.DirectionArrows(), mainColor, lineStyle, Qt::RoundCap);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolEllipticalArc::setRadius1(const QString &expression)
{
    radius1 = FindLength(expression, Visualization::data->DataVariables());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolEllipticalArc::setRadius2(const QString &expression)
{
    radius2 = FindLength(expression, Visualization::data->DataVariables());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolEllipticalArc::setF1(const QString &expression)
{
    f1 = FindVal(expression, Visualization::data->DataVariables());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolEllipticalArc::setF2(const QString &expression)
{
    f2 = FindVal(expression, Visualization::data->DataVariables());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolEllipticalArc::setRotationAngle(const QString &expression)
{
    rotationAngle = FindVal(expression, Visualization::data->DataVariables());
}
