/************************************************************************
 **
 **  @file   vplacelabelitem.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 10, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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
#include "vplacelabelitem.h"
#include "vplacelabelitem_p.h"
#include "../vpatterndb/vcontainer.h"
#include "varc.h"

#include <qnumeric.h>
#include <QPolygonF>
#include <QTransform>

//---------------------------------------------------------------------------------------------------------------------
VPlaceLabelItem::VPlaceLabelItem()
    : VPointF(), d(new VPlaceLabelItemData)
{
    setType(GOType::PlaceLabel);
    setMode(Draw::Modeling);
}

//---------------------------------------------------------------------------------------------------------------------
VPlaceLabelItem::VPlaceLabelItem(const VPlaceLabelItem &item)
    : VPointF(item), d(item.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VPlaceLabelItem::~VPlaceLabelItem()
{}

//---------------------------------------------------------------------------------------------------------------------
QString VPlaceLabelItem::GetWidthFormula() const
{
    return d->width;
}

//---------------------------------------------------------------------------------------------------------------------
QString &VPlaceLabelItem::GetWidthFormula()
{
    return d->width;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPlaceLabelItem::GetWidth() const
{
    return d->wValue;
}

//---------------------------------------------------------------------------------------------------------------------
void VPlaceLabelItem::SetWidth(qreal value, const QString &formula)
{
    d->wValue = value;
    d->width = formula;
}

//---------------------------------------------------------------------------------------------------------------------
QString VPlaceLabelItem::GetHeightFormula() const
{
    return d->height;
}

//---------------------------------------------------------------------------------------------------------------------
QString &VPlaceLabelItem::GetHeightFormula()
{
    return d->height;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPlaceLabelItem::GetHeight() const
{
    return d->hValue;
}

//---------------------------------------------------------------------------------------------------------------------
void VPlaceLabelItem::SetHeight(qreal value, const QString &formula)
{
    d->hValue = value;
    d->height = formula;
}

//---------------------------------------------------------------------------------------------------------------------
QString VPlaceLabelItem::GetAngleFormula() const
{
    return d->angle;
}

//---------------------------------------------------------------------------------------------------------------------
QString &VPlaceLabelItem::GetAngleFormula()
{
    return d->angle;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPlaceLabelItem::GetAngle() const
{
    return d->aValue;
}

//---------------------------------------------------------------------------------------------------------------------
void VPlaceLabelItem::SetAngle(qreal value, const QString &formula)
{
    d->aValue = value;
    d->angle = formula;
}

//---------------------------------------------------------------------------------------------------------------------
QString VPlaceLabelItem::GetVisibilityTrigger() const
{
    return d->visibilityTrigger;
}

//---------------------------------------------------------------------------------------------------------------------
QString &VPlaceLabelItem::GetVisibilityTrigger()
{
    return d->visibilityTrigger;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPlaceLabelItem::IsVisible() const
{
    bool visible = true;

    if (qIsInf(d->isVisible) || qIsNaN(d->isVisible))
    {
        qWarning() << QObject::tr("Visibility trigger contains error and will be ignored");
    }
    else if (qFuzzyIsNull(d->isVisible))
    {
        visible = false;
    }
    return visible;
}

//---------------------------------------------------------------------------------------------------------------------
void VPlaceLabelItem::SetVisibilityTrigger(qreal visible, const QString &formula)
{
    d->visibilityTrigger = formula;
    d->isVisible = visible;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPlaceLabelItem::GetCorrectionAngle() const
{
    return d->correctionAngle;
}

//---------------------------------------------------------------------------------------------------------------------
void VPlaceLabelItem::SetCorrectionAngle(qreal value)
{
    d->correctionAngle = value;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VPlaceLabelItem::GetCenterPoint() const
{
    return d->centerPoint;
}

//---------------------------------------------------------------------------------------------------------------------
void VPlaceLabelItem::SetCenterPoint(quint32 id)
{
    d->centerPoint = id;
}

//---------------------------------------------------------------------------------------------------------------------
PlaceLabelType VPlaceLabelItem::GetLabelType() const
{
    return d->type;
}

//---------------------------------------------------------------------------------------------------------------------
void VPlaceLabelItem::SetLabelType(PlaceLabelType type)
{
    d->type = type;
}

//---------------------------------------------------------------------------------------------------------------------
QTransform VPlaceLabelItem::RotationMatrix() const
{
    QTransform t;
    t.translate(x(), y());
    t.rotate(-d->aValue-d->correctionAngle);
    t.translate(-x(), -y());
    return t;
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VPlaceLabelItem::Box() const
{
    return QRectF(0, 0, d->wValue, d->hValue);
}

//---------------------------------------------------------------------------------------------------------------------
VPlaceLabelItem &VPlaceLabelItem::operator=(const VPlaceLabelItem &item)
{
    if ( &item == this )
    {
        return *this;
    }
    VPointF::operator=(item);
    d = item.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
PlaceLabelImg VPlaceLabelItem::LabelShape() const
{
    QTransform t = RotationMatrix();

    auto SegmentShape = [t, this]()
    {
        QPolygonF shape;
        shape << QPointF(x(), y() - d->hValue/2.0) << QPointF(x(), y() + d->hValue/2.0);

        return PlaceLabelImg({t.map(shape)});
    };

    auto RectangleShape = [t, this]()
    {
        QRectF rect(QPointF(x() - d->wValue/2.0, y() - d->hValue/2.0),
                    QPointF(x() + d->wValue/2.0, y() + d->hValue/2.0));

        QPolygonF shape;
        shape << rect.topLeft() << rect.topRight() << rect.bottomRight() << rect.bottomLeft() << rect.topLeft();

        return PlaceLabelImg({t.map(shape)});
    };

    auto CrossShape = [t, this]()
    {
        QPolygonF shape1;
        shape1 << QPointF(x(), y() - d->hValue/2.0)
               << QPointF(x(), y() + d->hValue/2.0);

        QPolygonF shape2;
        shape2 << QPointF(x() - d->wValue/2.0, y())
               << QPointF(x() + d->wValue/2.0, y());

        return PlaceLabelImg({t.map(shape1), t.map(shape2)});
    };

    auto TshapedShape = [t, this]()
    {
        QPointF center2(x(), y() + d->hValue/2.0);

        QPolygonF shape1;
        shape1 << QPointF(x(), y()) << center2;

        QPolygonF shape2;
        shape2 << QPointF(center2.x() - d->wValue/2.0, center2.y())
               << QPointF(center2.x() + d->wValue/2.0, center2.y());

        return PlaceLabelImg({t.map(shape1), t.map(shape2)});
    };

    auto DoubletreeShape = [t, this]()
    {
        QRectF rect(QPointF(x() - d->wValue/2.0, y() - d->hValue/2.0),
                    QPointF(x() + d->wValue/2.0, y() + d->hValue/2.0));

        QPolygonF shape1;
        shape1 << rect.topLeft() << rect.bottomRight();

        QPolygonF shape2;
        shape2 << rect.topRight() << rect.bottomLeft();

        return PlaceLabelImg({t.map(shape1), t.map(shape2)});
    };

    auto CornerShape = [t, this]()
    {
        QPolygonF shape1;
        shape1 << QPointF(x(), y()) << QPointF(x(), y() + d->hValue/2.0);

        QPolygonF shape2;
        shape2 << QPointF(x() - d->wValue/2.0, y()) << QPointF(x(), y());

        return PlaceLabelImg({t.map(shape1), t.map(shape2)});
    };

    auto TriangleShape = [t, this]()
    {
        QRectF rect(QPointF(x() - d->wValue/2.0, y() - d->hValue/2.0),
                    QPointF(x() + d->wValue/2.0, y() + d->hValue/2.0));

        QPolygonF shape;
        shape << rect.topLeft() << rect.topRight() << rect.bottomRight() << rect.topLeft();

        return PlaceLabelImg({t.map(shape)});
    };

    auto HshapedShape = [t, this]()
    {
        const QPointF center1 (x(), y() - d->hValue/2.0);
        const QPointF center2 (x(), y() + d->hValue/2.0);

        QPolygonF shape1;
        shape1 << center1 << center2;

        QPolygonF shape2;
        shape2 << QPointF(center1.x() - d->wValue/2.0, center1.y())
               << QPointF(center1.x() + d->wValue/2.0, center1.y());

        QPolygonF shape3;
        shape3 << QPointF(center2.x() - d->wValue/2.0, center2.y())
               << QPointF(center2.x() + d->wValue/2.0, center2.y());

        return PlaceLabelImg({t.map(shape1), t.map(shape2), t.map(shape3)});
    };

    auto ButtonShape = [t, this]()
    {
        const qreal radius = qMin(d->wValue/2.0, d->hValue/2.0);
        QPolygonF shape1;
        shape1 << QPointF(x(), y() - radius)
               << QPointF(x(), y() + radius);

        QPolygonF shape2;
        shape2 << QPointF(x() - radius, y())
               << QPointF(x() + radius, y());

        const qreal circleSize = 0.85;
        VArc arc(*this, radius*circleSize, 0, 360);
        arc.SetApproximationScale(10);
        QPolygonF shape3(arc.GetPoints());
        if (not shape3.isClosed() && not shape3.isEmpty())
        {
            shape3 << shape3.first();
        }

        return PlaceLabelImg({t.map(shape1), t.map(shape2), t.map(shape3)});
    };

    auto CircleShape = [t, this]()
    {
        const qreal radius = qMin(d->wValue/2.0, d->hValue/2.0);
        VArc arc(*this, radius, 0, 360);
        arc.SetApproximationScale(10);
        QPolygonF circle(arc.GetPoints());
        if (not circle.isClosed() && not circle.isEmpty())
        {
            circle << circle.first();
        }

        return PlaceLabelImg({t.map(circle)});
    };

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
    switch(d->type)
    {
        case PlaceLabelType::Segment:
            return SegmentShape();
        case PlaceLabelType::Rectangle:
            return RectangleShape();
        case PlaceLabelType::Cross:
            return CrossShape();
        case PlaceLabelType::Tshaped:
            return TshapedShape();
        case PlaceLabelType::Doubletree:
            return DoubletreeShape();
        case PlaceLabelType::Corner:
            return CornerShape();
        case PlaceLabelType::Triangle:
            return TriangleShape();
        case PlaceLabelType::Hshaped:
            return HshapedShape();
        case PlaceLabelType::Button:
            return ButtonShape();
        case PlaceLabelType::Circle:
            return CircleShape();
    }
QT_WARNING_POP

    return PlaceLabelImg();
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VPlaceLabelItem::LabelShapePath() const
{
    return LabelShapePath(LabelShape());
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VPlaceLabelItem::LabelShapePath(const PlaceLabelImg &shape)
{
    QPainterPath path;
    for (auto &p : shape)
    {
        if (not p.isEmpty())
        {
            path.moveTo(p.first());
            path.addPolygon(p);
        }
    }
    return path;
}
