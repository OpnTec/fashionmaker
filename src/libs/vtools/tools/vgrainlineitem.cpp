/************************************************************************
 **
 **  @file   vgrainlineitem.h
 **  @author Bojan Kverh
 **  @date   September 10, 2016
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

#include <math.h>

#include <QPainter>
#include <QtMath>

#include "vgrainlineitem.h"

#define ARROW_ANGLE                     0.5
#define ARROW_LENGTH                    0.05
#define RECT_WIDTH                      30

//---------------------------------------------------------------------------------------------------------------------
VGrainlineItem::VGrainlineItem(QGraphicsItem* pParent)
    :QGraphicsObject(pParent), m_eMode(mNormal), m_ptPos(QPointF(0, 0)), m_dRotation(0), m_dLength(0),
      m_rectBoundingBox()
{
    m_rectBoundingBox.setTopLeft(QPointF(0, 0));
    m_rectBoundingBox.setSize(QSizeF(200, 200));
}

//---------------------------------------------------------------------------------------------------------------------
VGrainlineItem::~VGrainlineItem()
{}

//---------------------------------------------------------------------------------------------------------------------
void VGrainlineItem::paint(QPainter* pP, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget)
{
    Q_UNUSED(pOption);
    Q_UNUSED(pWidget);
    QColor clr = Qt::black;
    pP->setPen(QPen(clr, 3));
    QPointF pt1 = m_ptPos;
    QPointF pt2 = pt1;

    pP->setRenderHints(QPainter::Antialiasing);
    pt2.setX(pt2.x() + m_dLength * cos(m_dRotation));
    pt2.setY(pt2.y() + m_dLength * sin(m_dRotation));
    pP->drawLine(pt1, pt2);

    QPolygonF poly;
    poly << pt1;
    QPointF ptA;
    ptA.setX(pt1.x() + m_dLength*ARROW_LENGTH*cos(m_dRotation + ARROW_ANGLE));
    ptA.setY(pt1.y() + m_dLength*ARROW_LENGTH*sin(m_dRotation + ARROW_ANGLE));
    poly << ptA;
    ptA.setX(pt1.x() + m_dLength*ARROW_LENGTH*cos(m_dRotation - ARROW_ANGLE));
    ptA.setY(pt1.y() + m_dLength*ARROW_LENGTH*sin(m_dRotation - ARROW_ANGLE));
    poly << ptA;
    pP->setBrush(clr);
    pP->drawPolygon(poly);

    poly.clear();
    poly << pt2;
    ptA.setX(pt2.x() + m_dLength*ARROW_LENGTH*cos(M_PI + m_dRotation + ARROW_ANGLE));
    ptA.setY(pt2.y() + m_dLength*ARROW_LENGTH*sin(M_PI + m_dRotation + ARROW_ANGLE));
    poly << ptA;
    ptA.setX(pt2.x() + m_dLength*ARROW_LENGTH*cos(M_PI + m_dRotation - ARROW_ANGLE));
    ptA.setY(pt2.y() + m_dLength*ARROW_LENGTH*sin(M_PI + m_dRotation - ARROW_ANGLE));
    poly << ptA;
    pP->drawPolygon(poly);

    if (m_eMode != mNormal)
    {
        pP->setPen(QPen(clr, 2, Qt::DashLine));
        pP->setBrush(Qt::NoBrush);
        poly.clear();
        ptA.setX(pt1.x() + RECT_WIDTH*cos(m_dRotation + M_PI/2));
        ptA.setY(pt1.y() + RECT_WIDTH*sin(m_dRotation + M_PI/2));
        poly << ptA;
        ptA.setX(pt1.x() + RECT_WIDTH*cos(m_dRotation - M_PI/2));
        ptA.setY(pt1.y() + RECT_WIDTH*sin(m_dRotation - M_PI/2));
        poly << ptA;
        ptA.setX(pt2.x() + RECT_WIDTH*cos(m_dRotation - M_PI/2));
        ptA.setY(pt2.y() + RECT_WIDTH*sin(m_dRotation - M_PI/2));
        poly << ptA;
        ptA.setX(pt2.x() + RECT_WIDTH*cos(m_dRotation + M_PI/2));
        ptA.setY(pt2.y() + RECT_WIDTH*sin(m_dRotation + M_PI/2));
        poly << ptA;
        pP->drawPolygon(poly);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VGrainlineItem::UpdateGeometry(const QPointF& ptPos, qreal dRotation, qreal dLength)
{
    m_ptPos = ptPos;
    m_dRotation = qDegreesToRadians(dRotation);
    m_dLength = dLength;
    m_eMode = mMove;
    update(m_rectBoundingBox);
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VGrainlineItem::boundingRect() const
{
    return m_rectBoundingBox;
}
