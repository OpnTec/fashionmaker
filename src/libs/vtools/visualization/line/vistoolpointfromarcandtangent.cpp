/************************************************************************
 **
 **  @file   vistoolpointfromarcandtangent.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   6 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

#include "vistoolpointfromarcandtangent.h"

#include <QGraphicsLineItem>
#include <QGraphicsPathItem>
#include <QLineF>
#include <QPainterPath>
#include <QSharedPointer>
#include <Qt>
#include <new>

#include "../../tools/drawTools/toolpoint/toolsinglepoint/vtoolpointfromarcandtangent.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../visualization.h"
#include "visline.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolPointFromArcAndTangent::VisToolPointFromArcAndTangent(const VContainer *data, QGraphicsItem *parent)
    : VisLine(data, parent), arcId(NULL_ID), crossPoint(CrossCirclesPoint::FirstPoint),
      point(nullptr), tangent(nullptr), arcPath(nullptr), tangentLine2(nullptr)
{
    arcPath = InitItem<QGraphicsPathItem>(Qt::darkGreen, this);
    point = InitPoint(mainColor, this);
    tangent = InitPoint(supportColor, this);
    tangentLine2 = InitItem<QGraphicsLineItem>(supportColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolPointFromArcAndTangent::~VisToolPointFromArcAndTangent()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointFromArcAndTangent::RefreshGeometry()
{
    if (object1Id > NULL_ID)// tangent point
    {
        const QSharedPointer<VPointF> tan = Visualization::data->GeometricObject<VPointF>(object1Id);
        DrawPoint(tangent, static_cast<QPointF>(*tan), supportColor);

        if (arcId > NULL_ID)// circle center
        {
            const QSharedPointer<VArc> arc = Visualization::data->GeometricObject<VArc>(arcId);
            DrawPath(arcPath, arc->GetPath(PathDirection::Show), Qt::darkGreen, Qt::SolidLine, Qt::RoundCap);

            FindRays(static_cast<QPointF>(*tan), arc.data());

            const QPointF fPoint = VToolPointFromArcAndTangent::FindPoint(static_cast<QPointF>(*tan), arc.data(),
                                                                          crossPoint);
            DrawPoint(point, fPoint, mainColor);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointFromArcAndTangent::setArcId(const quint32 &value)
{
    arcId = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointFromArcAndTangent::setCrossPoint(const CrossCirclesPoint &value)
{
    crossPoint = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointFromArcAndTangent::FindRays(const QPointF &p, const VArc *arc)
{
    QPointF p1, p2;
    const QPointF center = static_cast<QPointF>(arc->GetCenter());
    const qreal radius = arc->GetRadius();
    const int res = VGObject::ContactPoints (p, center, radius, p1, p2);

    QLineF r1Arc(center, p1);
    r1Arc.setLength(radius+10);

    QLineF r2Arc(center, p2);
    r2Arc.setLength(radius+10);

    switch(res)
    {
        case 2:
        {
            int localRes = 0;
            if (arc->IsIntersectLine(r1Arc))
            {
                ++localRes;
            }

            if (arc->IsIntersectLine(r2Arc))
            {
                ++localRes;
            }

            switch(localRes)
            {
                case 2:
                    DrawRay(this, p, p1, supportColor, Qt::DashLine);
                    DrawRay(tangentLine2, p, p2, supportColor, Qt::DashLine);
                    break;
                case 1:
                    DrawRay(this, p, p1, supportColor, Qt::DashLine);
                    tangentLine2->setVisible(false);
                    break;
                case 0:
                default:
                    this->setVisible(false);
                    tangentLine2->setVisible(false);
                    break;
            }

            break;
        }
        case 1:
            DrawRay(this, p, p1, supportColor, Qt::DashLine);
            tangentLine2->setVisible(false);
            break;
        case 3:
        case 0:
        default:
            this->setVisible(false);
            tangentLine2->setVisible(false);
            break;
    }
}
