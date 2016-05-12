/************************************************************************
 **
 **  @file   vsimplecurve.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 5, 2016
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

#include "vsimplecurve.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vabstractcurve.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>

//---------------------------------------------------------------------------------------------------------------------
VSimpleCurve::VSimpleCurve(quint32 id, const QColor &currentColor, Unit patternUnit, qreal *factor, QObject *parent)
    : VAbstractSimple(id, currentColor, patternUnit, factor, parent),
      QGraphicsPathItem(),
      m_curve()
{
    this->setBrush(QBrush(Qt::NoBrush));
    SetPen(this, currentColor, WidthHairLine(patternUnit));
    this->setAcceptHoverEvents(true);
}

//---------------------------------------------------------------------------------------------------------------------
VSimpleCurve::~VSimpleCurve()
{
}

//---------------------------------------------------------------------------------------------------------------------
void VSimpleCurve::SetCurrentColor(const QColor &value)
{
    SetSimpleCurrentColor(this, value);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimpleCurve::ChangedActivDraw(bool flag)
{
    SimpleChangedActivDraw(this, flag);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimpleCurve::RefreshGeometry(const QSharedPointer<VAbstractCurve> &curve)
{
    m_curve = curve;
    if (not curve.isNull())
    {
        setPath(curve->GetPath(PathDirection::Hide));
    }
    else
    {
        qWarning() << tr("VSimpleCurve::RefreshGeometry: pointer to curve is null.");
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VSimpleCurve::CurveChoosed()
{
    emit Choosed(id);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimpleCurve::CurveSelected(bool selected)
{
    emit Selected(selected, id);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimpleCurve::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Special for not selectable item first need to call standard mousePressEvent then accept event
    QGraphicsPathItem::mousePressEvent(event);
    if (selectionType == SelectionType::ByMouseRelease)
    {
        event->accept();// Special for not selectable item first need to call standard mousePressEvent then accept event
    }
    else
    {
        if (event->button() == Qt::LeftButton)
        {
            emit Choosed(id);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VSimpleCurve::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (selectionType == SelectionType::ByMouseRelease)
    {
        if (event->button() == Qt::LeftButton)
        {
            emit Choosed(id);
        }
    }
    QGraphicsPathItem::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimpleCurve::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    SetPen(this, currentColor, WidthMainLine(patternUnit));
    QGraphicsPathItem::hoverEnterEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimpleCurve::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    SetPen(this, currentColor, WidthHairLine(patternUnit));
    QGraphicsPathItem::hoverLeaveEvent(event);
}
