/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 9, 2016
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

#include "vistoolflippingbyline.h"
#include "../vgeometry/vpointf.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolFlippingByLine::VisToolFlippingByLine(const VContainer *data, QGraphicsItem *parent)
    : VisOperation(data, parent),
      object2Id(NULL_ID),
      point1(nullptr),
      point2(nullptr)
{
    point1 = InitPoint(supportColor2, this);
    point2 = InitPoint(supportColor2, this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolFlippingByLine::RefreshGeometry()
{
    if (objects.isEmpty())
    {
        return;
    }

    QPointF firstPoint;
    QPointF secondPoint;

    if (object1Id != NULL_ID)
    {
        firstPoint = static_cast<QPointF>(*Visualization::data->GeometricObject<VPointF>(object1Id));
        DrawPoint(point1, firstPoint, supportColor2);

        if (object2Id == NULL_ID)
        {
            secondPoint = Visualization::scenePos;
        }
        else
        {
            secondPoint = static_cast<QPointF>(*Visualization::data->GeometricObject<VPointF>(object2Id));
            DrawPoint(point2, secondPoint, supportColor2);
        }

        DrawLine(this, QLineF(firstPoint, secondPoint), supportColor2, Qt::DashLine);
    }

    RefreshFlippedObjects(firstPoint, secondPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolFlippingByLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const qreal scale = SceneScale(scene());

    ScalePoint(point1, scale);
    ScalePoint(point2, scale);

    VisOperation::paint(painter, option, widget);
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VisToolFlippingByLine::boundingRect() const
{
    QRectF rect = VisOperation::boundingRect();
    rect = rect.united(point1->boundingRect());
    rect = rect.united(point2->boundingRect());
    return rect;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolFlippingByLine::SetFirstLinePointId(quint32 value)
{
    object1Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolFlippingByLine::SetSecondLinePointId(quint32 value)
{
    object2Id = value;
}
