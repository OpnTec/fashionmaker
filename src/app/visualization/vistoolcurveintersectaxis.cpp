/************************************************************************
 **
 **  @file   vistoolcurveintersectaxis.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 10, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#include "vistoolcurveintersectaxis.h"
#include "../container/vcontainer.h"
#include "../libs/vgeometry/vpointf.h"
#include "../tools/drawTools/vtoolcurveintersectaxis.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolCurveIntersectAxis::VisToolCurveIntersectAxis(const VContainer *data, QGraphicsItem *parent)
    : VisLine(data, parent), axisPointId(NULL_ID), angle(-1), point(nullptr), basePoint(nullptr), baseLine(nullptr),
      axisLine(nullptr), visCurve(nullptr)
{
    this->mainColor = Qt::red;

    visCurve = InitItem<QGraphicsPathItem>(Qt::darkGreen, this);
    basePoint = InitPoint(supportColor, this);
    baseLine = InitItem<QGraphicsLineItem>(supportColor, this);
    axisLine = InitItem<QGraphicsLineItem>(supportColor, this);
    point = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolCurveIntersectAxis::~VisToolCurveIntersectAxis()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCurveIntersectAxis::RefreshGeometry()
{
    if (point1Id > NULL_ID)
    {
        const QSharedPointer<VAbstractCurve> curve = Visualization::data->GeometricObject<VAbstractCurve>(point1Id);
        DrawPath(visCurve, curve->GetPath(PathDirection::Show), supportColor, Qt::SolidLine, Qt::RoundCap);

        if (axisPointId > NULL_ID)
        {
            QLineF axis;
            const QSharedPointer<VPointF> first = Visualization::data->GeometricObject<VPointF>(axisPointId);
            if (qFuzzyCompare(angle, -1))
            {
                axis = Axis(first->toQPointF(), Visualization::scenePos);
            }
            else
            {
                axis = Axis(first->toQPointF(), angle);
            }
            DrawPoint(basePoint, first->toQPointF(), mainColor);
            DrawLine(axisLine, axis, supportColor, Qt::DashLine);

            QPointF p = VToolCurveIntersectAxis::FindPoint(first->toQPointF(), axis.angle(), curve);
            QLineF axis_line(first->toQPointF(), p);
            DrawLine(this, axis_line, mainColor, lineStyle);

            DrawPoint(point, p, mainColor);

            Visualization::toolTip = QString(tr("<b>Intersection curve and axis</b>: angle = %1°; <b>Shift</b> - "
                                                "sticking angle, <b>Enter</b> - finish creation"))
                    .arg(this->line().angle());
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VisToolCurveIntersectAxis::Angle() const
{
    return QString("%1").arg(this->line().angle());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCurveIntersectAxis::SetAngle(const QString &expression)
{
    angle = FindVal(expression);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCurveIntersectAxis::setAxisPointId(const quint32 &value)
{
    axisPointId = value;
}
