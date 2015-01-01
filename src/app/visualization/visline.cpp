/************************************************************************
 **
 **  @file   visline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 7, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#include "visline.h"
#include "../tools/drawTools/vdrawtool.h"
#include <QtCore/qmath.h>

//---------------------------------------------------------------------------------------------------------------------
VisLine::VisLine(const VContainer *data, QGraphicsItem *parent)
    :Visualization(data), QGraphicsLineItem(parent)
{
    this->setZValue(1);// Show on top real tool
    InitPen();
}

//---------------------------------------------------------------------------------------------------------------------
VisLine::~VisLine()
{}

//---------------------------------------------------------------------------------------------------------------------
qreal VisLine::CorrectAngle(const qreal &angle) const
{
    qreal ang = angle;
    if (angle > 360)
    {
        ang = angle - 360 * qFloor(angle/360);
    }

    switch (qFloor((qAbs(ang)+22.5)/45))
    {
        case 0: // <22.5
            return 0;
        case 1: // <67.5
            return 45;
        case 2: // <112.5
            return 90;
        case 3: // <157.5
            return 135;
        case 4: // <202.5
            return 180;
        case 5: // <247.5
            return 225;
        case 6: // < 292.5
            return 270;
        case 7: // <337.5
            return 315;
        default: // <360
            return 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VisLine::Ray(const QPointF &firstPoint, const qreal &angle) const
{
    if (this->scene() == nullptr)
    {
        QLineF line = QLineF(firstPoint, Visualization::scenePos);
        line.setAngle(angle);
        return line.p2();// We can't find ray because item doesn't have scene. We will return cursor position on scene.
    }

    QRectF scRect = this->scene()->sceneRect();
    if (QGuiApplication::keyboardModifiers() == Qt::ShiftModifier)
    {
        return VGObject::BuildRay(firstPoint, CorrectAngle(angle), scRect);
    }
    else
    {
        return VGObject::BuildRay(firstPoint, angle, scRect);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VisLine::Ray(const QPointF &firstPoint) const
{
    QLineF line = QLineF(firstPoint, Visualization::scenePos);
    return Ray(firstPoint, line.angle());
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VisLine::Axis(const QPointF &p, const qreal &angle) const
{
    QPointF endP1 = Ray(p, angle+180);
    QPointF endP2 = Ray(p, angle);
    return QLineF(endP1, endP2);
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VisLine::Axis(const QPointF &p1, const QPointF &p2) const
{
    QLineF line(p1, p2);
    return Axis(p1, line.angle());
}

//---------------------------------------------------------------------------------------------------------------------
void VisLine::InitPen()
{
    this->setPen(QPen(mainColor, qApp->toPixel(qApp->widthHairLine())/factor, lineStyle));
}

//---------------------------------------------------------------------------------------------------------------------
void VisLine::AddOnScene()
{
    AddItem(this);
}
