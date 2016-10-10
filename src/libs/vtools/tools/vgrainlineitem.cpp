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
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QDebug>

#include "../vmisc/def.h"
#include "../vmisc/vmath.h"

#include "vgrainlineitem.h"

#define ARROW_ANGLE                     0.35
#define ARROW_LENGTH                    15
#define RECT_WIDTH                      30
#define RESIZE_RECT_SIZE                10
#define ACTIVE_Z                        10
#define INACTIVE_Z                      5

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGrainlineItem::VGrainlineItem constructor
 * @param pParent pointer to the parent item
 */
VGrainlineItem::VGrainlineItem(QGraphicsItem* pParent)
    :QGraphicsObject(pParent), m_eMode(VGrainlineItem::mNormal), m_dRotation(0), m_dLength(0), m_rectBoundingBox(),
      m_polyBound(), m_ptStartPos(), m_ptStartMove(), m_dScale(1), m_polyResize()
{
    m_rectBoundingBox.setTopLeft(QPointF(0, 0));
    setAcceptHoverEvents(true);
    Reset();
    UpdateRectangle();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGrainlineItem::~VGrainlineItem destructor
 */
VGrainlineItem::~VGrainlineItem()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGrainlineItem::paint paints the item content
 * @param pP pointer to the painter object
 * @param pOption not used
 * @param pWidget not used
 */
void VGrainlineItem::paint(QPainter* pP, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget)
{
    Q_UNUSED(pOption);
    Q_UNUSED(pWidget);
    pP->save();
    QColor clr = Qt::black;
    pP->setPen(QPen(clr, 3));
    QPointF pt1(0, 0);
    QPointF pt2;

    pt2.setX(pt1.x() + m_dLength * cos(m_dRotation));
    pt2.setY(pt1.y() + m_dLength * sin(m_dRotation));

    pP->setRenderHints(QPainter::Antialiasing);
    // line
    pP->drawLine(pt1, pt2);

    // first arrow
    QPolygonF poly;
    poly << pt1;
    QPointF ptA;
    qreal dArrLen = ARROW_LENGTH*m_dScale;
    ptA.setX(pt1.x() + dArrLen*cos(m_dRotation + ARROW_ANGLE));
    ptA.setY(pt1.y() + dArrLen*sin(m_dRotation + ARROW_ANGLE));
    poly << ptA;
    ptA.setX(pt1.x() + dArrLen*cos(m_dRotation - ARROW_ANGLE));
    ptA.setY(pt1.y() + dArrLen*sin(m_dRotation - ARROW_ANGLE));
    poly << ptA;
    pP->setBrush(clr);
    pP->drawPolygon(poly);
    // second arrow
    poly.clear();
    poly << pt2;
    ptA.setX(pt2.x() + dArrLen*cos(M_PI + m_dRotation + ARROW_ANGLE));
    ptA.setY(pt2.y() + dArrLen*sin(M_PI + m_dRotation + ARROW_ANGLE));
    poly << ptA;
    ptA.setX(pt2.x() + dArrLen*cos(M_PI + m_dRotation - ARROW_ANGLE));
    ptA.setY(pt2.y() + dArrLen*sin(M_PI + m_dRotation - ARROW_ANGLE));
    poly << ptA;
    pP->drawPolygon(poly);

    if (m_eMode != mNormal)
    {
        pP->setPen(QPen(clr, 2, Qt::DashLine));
        pP->setBrush(Qt::NoBrush);
        // bounding polygon
        pP->drawPolygon(m_polyBound);

        pP->setBrush(clr);
        pP->drawPolygon(m_polyResize);

        pP->setBrush(Qt::NoBrush);
        if (m_eMode == mResize)
        {
            pP->drawLine(m_polyBound.at(0), m_polyBound.at(2));
            pP->drawLine(m_polyBound.at(1), m_polyBound.at(3));
        }

    }
    pP->restore();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGrainlineItem::UpdateGeometry updates the item with grainline parameters
 * @param ptPos position of one grainline's end
 * @param dRotation rotation of the grainline in [degrees]
 * @param dLength length of the grainline in user's units
 */
void VGrainlineItem::UpdateGeometry(const QPointF& ptPos, qreal dRotation, qreal dLength)
{
    m_dRotation = qDegreesToRadians(dRotation);
    m_dLength = dLength;

    qreal dX;
    qreal dY;
    QPointF pt = ptPos;
    if (IsContained(pt, dX, dY) == false)
    {
        pt.setX(pt.x() + dX);
        pt.setY(pt.y() + dY);
    }
    setPos(pt);
    UpdateRectangle();
    UpdateBox();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGrainlineItem::boundingRect returns the bounding rect around the grainline
 * @return bounding rect
 */
QRectF VGrainlineItem::boundingRect() const
{
    return m_rectBoundingBox;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGrainlineItem::Reset resets the item parameters.
 */
void VGrainlineItem::Reset()
{
    m_eMode = mNormal;
    setZValue(INACTIVE_Z);
    UpdateBox();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGrainlineItem::IsIdle returns the idle flag.
 * @return true, if item mode is normal and false otherwise.
 */
bool VGrainlineItem::IsIdle() const
{
    return m_eMode == mNormal;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGrainlineItem::IsContained checks, if both ends of the grainline, starting at pt, are contained in
 * parent widget.
 * @param pt starting point of the grainline.
 * @param dX horizontal translation needed to put the arrow inside parent item
 * @param dY vertical translation needed to put the arrow inside parent item
 * @return true, if both ends of the grainline, starting at pt, are contained in the parent widget and
 * false otherwise.
 */
bool VGrainlineItem::IsContained(const QPointF& pt, qreal &dX, qreal &dY) const
{
    dX = 0;
    dY = 0;
    QPointF apt[2];
    apt[0] = pt;
    apt[1].setX(pt.x() + m_dLength * cos(m_dRotation));
    apt[1].setY(pt.y() + m_dLength * sin(m_dRotation));
    // single point differences
    qreal dPtX;
    qreal dPtY;
    bool bInside = true;

    QRectF rectParent = parentItem()->boundingRect();
    for (int i = 0; i < 2; ++i)
    {
        dPtX = 0;
        dPtY = 0;
        if (rectParent.contains(apt[i]) == false)
        {
            if (apt[i].x() < rectParent.left())
            {
                dPtX = rectParent.left() - apt[i].x();
            }
            else if (apt[i].x() > rectParent.right())
            {
                dPtX = rectParent.right() - apt[i].x();
            }
            if (apt[i].y() < rectParent.top())
            {
                dPtY = rectParent.top() - apt[i].y();
            }
            else if (apt[i].y() > rectParent.bottom())
            {
                dPtY = rectParent.bottom() - apt[i].y();
            }

            if (fabs(dPtX) > fabs(dX))
            {
                dX = dPtX;
            }
            if (fabs(dPtY) > fabs(dY))
            {
                dY = dPtY;
            }

            bInside = false;
        }
    }
    return bInside;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGrainlineItem::SetScale sets the scale for keeping the arrows of constant size
 * @param dScale scale factor
 */
void VGrainlineItem::SetScale(qreal dScale)
{
    m_dScale = dScale;
    UpdateRectangle();
    UpdateBox();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGrainlineItem::mousePressEvent handles left button mouse press events
 * @param pME pointer to QGraphicsSceneMouseEvent object
 */
void VGrainlineItem::mousePressEvent(QGraphicsSceneMouseEvent* pME)
{
    if (pME->button() == Qt::LeftButton)
    {
        m_ptStartPos = pos();
        m_ptStartMove = pME->scenePos();
        m_dStartLength = m_dLength;

        if (m_polyResize.containsPoint(pME->pos(), Qt::OddEvenFill) == true)
        {
            m_eMode = mResize;
            SetOverrideCursor(Qt::SizeFDiagCursor);
        }
        else
        {
            m_eMode = mMove;
            SetOverrideCursor(cursorArrowCloseHand, 1, 1);
        }
    }
    setZValue(ACTIVE_Z);
    UpdateBox();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGrainlineItem::mouseMoveEvent handles mouse move events, making sure that the item is moved properly
 * @param pME pointer to QGraphicsSceneMouseEvent object
 */
void VGrainlineItem::mouseMoveEvent(QGraphicsSceneMouseEvent* pME)
{
    QPointF ptDiff = pME->scenePos() - m_ptStartMove;
    if (m_eMode == mMove)
    {
        QPointF pt = m_ptStartPos + ptDiff;
        qreal dX;
        qreal dY;
        if (IsContained(pt, dX, dY) == false)
        {
            pt.setX(pt.x() + dX);
            pt.setY(pt.y() + dY);
        }
        setPos(pt);
        UpdateBox();
    }
    else if (m_eMode == mResize)
    {
        qreal dLen = qSqrt(ptDiff.x()*ptDiff.x() + ptDiff.y()*ptDiff.y());
        qreal dAng = qAtan2(ptDiff.y(), ptDiff.x());
        dLen = dLen*qCos(dAng - m_dRotation);
        qreal dPrevLen = m_dLength;
        // try with new length
        m_dLength = m_dStartLength + dLen;
        qreal dX;
        qreal dY;
        if (IsContained(m_ptStartPos, dX, dY) == false)
        {
            m_dLength = dPrevLen;
        }
        UpdateRectangle();
        UpdateBox();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGrainlineItem::mouseReleaseEvent handles mouse release events and emits the proper signal if the item was
 * moved
 * @param pME pointer to QGraphicsSceneMouseEvent object
 */
void VGrainlineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* pME)
{
    if (pME->button() == Qt::LeftButton)
    {
        if (m_eMode == mMove)
        {
            RestoreOverrideCursor(cursorArrowCloseHand);
        }
        else if (m_eMode == mResize)
        {
            RestoreOverrideCursor(Qt::SizeFDiagCursor);
        }

        qreal dD = fabs(pME->scenePos().x() - m_ptStartMove.x()) + fabs(pME->scenePos().y() - m_ptStartMove.y());
        bool bShort = (dD < 2);

        if (m_eMode == mMove || m_eMode == mResize)
        {
            if (bShort == true)
            {
                // TODO: switch to rotate mode
            }
            else
            {
                if (m_eMode == mMove)
                {
                    emit SignalMoved(pos());
                }
                else
                {
                    emit SignalResized(m_dLength);
                }
                UpdateBox();
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGrainlineItem::UpdateBox updates the item
 */
void VGrainlineItem::UpdateBox()
{
    update(m_rectBoundingBox);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGrainlineItem::UpdateRectangle updates the polygon for the box around active item
 * and the bounding rectangle
 */
void VGrainlineItem::UpdateRectangle()
{
    m_polyBound.clear();
    QPointF pt1(0, 0);
    QPointF pt2;

    pt2.setX(pt1.x() + m_dLength * cos(m_dRotation));
    pt2.setY(pt1.y() + m_dLength * sin(m_dRotation));

    QPointF ptA;
    ptA.setX(pt1.x() + RECT_WIDTH*cos(m_dRotation + M_PI/2));
    ptA.setY(pt1.y() + RECT_WIDTH*sin(m_dRotation + M_PI/2));
    m_polyBound << ptA;
    ptA.setX(pt1.x() + RECT_WIDTH*cos(m_dRotation - M_PI/2));
    ptA.setY(pt1.y() + RECT_WIDTH*sin(m_dRotation - M_PI/2));
    m_polyBound << ptA;
    ptA.setX(pt2.x() + RECT_WIDTH*cos(m_dRotation - M_PI/2));
    ptA.setY(pt2.y() + RECT_WIDTH*sin(m_dRotation - M_PI/2));
    m_polyBound << ptA;
    ptA.setX(pt2.x() + RECT_WIDTH*cos(m_dRotation + M_PI/2));
    ptA.setY(pt2.y() + RECT_WIDTH*sin(m_dRotation + M_PI/2));
    m_polyBound << ptA;
    m_rectBoundingBox = m_polyBound.boundingRect();
    setTransformOriginPoint(m_rectBoundingBox.center());

    m_polyResize.clear();
    ptA = m_polyBound.at(2);
    m_polyResize << ptA;
    double dSize = m_dScale * RESIZE_RECT_SIZE;

    ptA.setX(ptA.x() + dSize*cos(m_dRotation + M_PI/2));
    ptA.setY(ptA.y() + dSize*sin(m_dRotation + M_PI/2));
    m_polyResize << ptA;

    ptA.setX(ptA.x() - dSize*cos(m_dRotation));
    ptA.setY(ptA.y() - dSize*sin(m_dRotation));
    m_polyResize << ptA;

    ptA.setX(ptA.x() + dSize*cos(m_dRotation - M_PI/2));
    ptA.setY(ptA.y() + dSize*sin(m_dRotation - M_PI/2));
    m_polyResize << ptA;

    prepareGeometryChange();
}
