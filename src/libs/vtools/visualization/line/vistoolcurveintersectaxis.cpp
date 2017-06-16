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

#include <QColor>
#include <QGraphicsLineItem>
#include <QGraphicsPathItem>
#include <QLineF>
#include <QPainterPath>
#include <QPointF>
#include <QSharedPointer>
#include <Qt>
#include <new>

#include "../../tools/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolcurveintersectaxis.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../visualization.h"
#include "visline.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolCurveIntersectAxis::VisToolCurveIntersectAxis(const VContainer *data, QGraphicsItem *parent)
    : VisLine(data, parent), axisPointId(NULL_ID), angle(-1), point(nullptr), basePoint(nullptr), baseLine(nullptr),
      axisLine(nullptr), visCurve(nullptr)
{
    this->mainColor = Qt::red;

    visCurve = InitItem<VCurvePathItem>(Qt::darkGreen, this);
    basePoint = InitPoint(supportColor, this);
    baseLine = InitItem<QGraphicsLineItem>(supportColor, this);
    axisLine = InitItem<QGraphicsLineItem>(supportColor, this); //-V656
    point = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCurveIntersectAxis::RefreshGeometry()
{
    if (object1Id > NULL_ID)
    {
        const QSharedPointer<VAbstractCurve> curve = Visualization::data->GeometricObject<VAbstractCurve>(object1Id);
        DrawPath(visCurve, curve->GetPath(PathDirection::Show), supportColor, Qt::SolidLine, Qt::RoundCap);

        if (axisPointId > NULL_ID)
        {
            QLineF axis;
            const QSharedPointer<VPointF> first = Visualization::data->GeometricObject<VPointF>(axisPointId);
            if (VFuzzyComparePossibleNulls(angle, -1))
            {
                axis = Axis(static_cast<QPointF>(*first), Visualization::scenePos);
            }
            else
            {
                axis = Axis(static_cast<QPointF>(*first), angle);
            }
            DrawPoint(basePoint, static_cast<QPointF>(*first), mainColor);
            DrawLine(axisLine, axis, supportColor, Qt::DashLine);

            QPointF p = VToolCurveIntersectAxis::FindPoint(static_cast<QPointF>(*first), axis.angle(), curve);
            QLineF axis_line(static_cast<QPointF>(*first), p);
            DrawLine(this, axis_line, mainColor, lineStyle);

            DrawPoint(point, p, mainColor);

            Visualization::toolTip = tr("<b>Intersection curve and axis</b>: angle = %1°; <b>Shift</b> - "
                                        "sticking angle, <b>Enter</b> - finish creation")
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
    angle = FindVal(expression, Visualization::data->PlainVariables());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCurveIntersectAxis::setAxisPointId(const quint32 &value)
{
    axisPointId = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCurveIntersectAxis::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const qreal scale = SceneScale(scene());

    ScalePoint(point, scale);
    ScalePoint(basePoint, scale);
    ScalePenWidth(baseLine, scale);
    ScalePenWidth(axisLine, scale);
    ScalePenWidth(visCurve, scale);

    VisLine::paint(painter, option, widget);
}
