/************************************************************************
 **
 **  @file   vgrainlineitem.cpp
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
#include <QGraphicsScene>
#include <QGraphicsView>

#include "../vmisc/def.h"
#include "../vmisc/vmath.h"

#include "vgrainlineitem.h"

#define ARROW_ANGLE                     M_PI/9
#define ARROW_LENGTH                    15
#define RECT_WIDTH                      30
#define RESIZE_RECT_SIZE                10
#define ROTATE_CIRC_R                   7
#define ACTIVE_Z                        10
#define LINE_PEN_WIDTH                  3

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGrainlineItem::VGrainlineItem constructor
 * @param pParent pointer to the parent item
 */
VGrainlineItem::VGrainlineItem(QGraphicsItem* pParent)
    : VPieceItem(pParent),
      m_dRotation(0),
      m_dStartRotation(0),
      m_dLength(0),
      m_polyBound(),
      m_ptStartPos(),
      m_ptStartMove(),
      m_dScale(1),
      m_polyResize(),
      m_dStartLength(0),
      m_ptStart(),
      m_ptFinish(),
      m_ptCenter(),
      m_dAngle(0),
      m_eArrowType(ArrowType::atBoth),
      m_penWidth(LINE_PEN_WIDTH)
{
    setAcceptHoverEvents(true);
    m_inactiveZ = 5;
    Reset();
    UpdateRectangle();
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VGrainlineItem::shape() const
{
    if (m_eMode == mNormal)
    {
        return MainShape();
    }
    else
    {
        QPainterPath path;
        path.addPolygon(m_polyBound);
        return path;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGrainlineItem::paint paints the item content
 * @param pP pointer to the painter object
 * @param pOption not used
 * @param pWidget not used
 */
void VGrainlineItem::paint(QPainter* pP, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget)
{
    Q_UNUSED(pOption)
    Q_UNUSED(pWidget)
    pP->save();
    QColor clr = Qt::black;
    pP->setPen(QPen(clr, m_penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    pP->setRenderHints(QPainter::Antialiasing);
    // line
    const QLineF mainLine = MainLine();
    pP->drawLine(mainLine.p1(), mainLine.p2());

    pP->setBrush(clr);

    m_dScale = GetScale();
    qreal dArrLen = ARROW_LENGTH*m_dScale;
    if (m_eArrowType != ArrowType::atRear)
    {
        // first arrow
        pP->drawPolygon(FirstArrow(dArrLen));
    }
    if (m_eArrowType != ArrowType::atFront)
    {
        // second arrow
        pP->drawPolygon(SecondArrow(dArrLen));
    }

    if (m_eMode != mNormal)
    {
        pP->setPen(QPen(clr, 2, Qt::DashLine));
        pP->setBrush(Qt::NoBrush);
        // bounding polygon
        pP->drawPolygon(m_polyBound);
        pP->setPen(QPen(clr, 3));

        if (m_eMode != mRotate)
        {
            pP->setBrush(clr);
            UpdatePolyResize();
            pP->drawPolygon(m_polyResize);
        }

        pP->setBrush(Qt::NoBrush);
        if (m_eMode == mResize)
        {
            pP->setPen(clr);
            pP->drawLine(m_polyBound.at(0), m_polyBound.at(2));
            pP->drawLine(m_polyBound.at(1), m_polyBound.at(3));
        }

        if (m_eMode == mRotate)
        {
            QPointF ptC = (m_polyBound.at(0) + m_polyBound.at(2))/2;
            qreal dRad = m_dScale * ROTATE_CIRC_R;
            pP->setBrush(clr);
            pP->drawEllipse(ptC, dRad, dRad);

            pP->setBrush(Qt::NoBrush);
            pP->save();
            pP->translate(ptC);
            pP->rotate(qRadiansToDegrees(-m_dRotation));
            int iX = int(qRound(m_dLength/2 - 0.5*dRad));
            int iY = int(qRound(RECT_WIDTH - 0.5*dRad));
            int iR = int(qRound(dRad*3));
            pP->drawArc(iX - iR, iY - iR, iR, iR, 0*16, -90*16);
            pP->drawArc(-iX, iY - iR, iR, iR, 270*16, -90*16);
            pP->drawArc(-iX, -iY, iR, iR, 180*16, -90*16);
            pP->drawArc(iX - iR, -iY, iR, iR, 90*16, -90*16);
            pP->restore();
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
void VGrainlineItem::UpdateGeometry(const QPointF& ptPos, qreal dRotation, qreal dLength, ArrowType eAT)
{
    m_dRotation = qDegreesToRadians(dRotation);
    m_dLength = dLength;

    qreal dX;
    qreal dY;
    QPointF pt = ptPos;
    if (IsContained(pt, m_dRotation, dX, dY) == false)
    {
        pt.setX(pt.x() + dX);
        pt.setY(pt.y() + dY);
    }
    setPos(pt);
    m_eArrowType = eAT;

    UpdateRectangle();
    Update();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGrainlineItem::IsContained checks, if both ends of the grainline, starting at pt, are contained in
 * parent widget.
 * @param pt starting point of the grainline.
 * @param dRot rotation of the grainline in [rad]
 * @param dX horizontal translation needed to put the arrow inside parent item
 * @param dY vertical translation needed to put the arrow inside parent item
 * @return true, if both ends of the grainline, starting at pt, are contained in the parent widget and
 * false otherwise.
 */
bool VGrainlineItem::IsContained(const QPointF& pt, qreal dRot, qreal &dX, qreal &dY) const
{
    dX = 0;
    dY = 0;
    QPointF apt[2];
    apt[0] = pt;
    apt[1].setX(pt.x() + m_dLength * cos(dRot));
    apt[1].setY(pt.y() - m_dLength * sin(dRot));
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
 * @brief VGrainlineItem::mousePressEvent handles left button mouse press events
 * @param pME pointer to QGraphicsSceneMouseEvent object
 */
void VGrainlineItem::mousePressEvent(QGraphicsSceneMouseEvent* pME)
{
    if (pME->button() == Qt::LeftButton && pME->type() != QEvent::GraphicsSceneMouseDoubleClick)
    {
        if (m_moveType == NotMovable)
        {
            pME->ignore();
            return;
        }

        m_ptStartPos = pos();
        m_ptStartMove = pME->scenePos();
        m_dStartLength = m_dLength;
        m_dStartRotation = m_dRotation;
        m_dAngle = GetAngle(mapToParent(pME->pos()));
        m_ptRotCenter = m_ptCenter;

        if ((m_moveType & AllModifications ) == AllModifications)
        {
            AllUserModifications(pME->pos());
            setZValue(ACTIVE_Z);
            Update();
        }
        else if (m_moveType & IsRotatable)
        {
            if (m_moveType & IsResizable)
            {
                AllUserModifications(pME->pos());
            }
            else if (m_moveType & IsMovable)
            {
                UserRotateAndMove();
            }
            else
            {
                m_eMode = mRotate;
                SetOverrideCursor(cursorArrowCloseHand, 1, 1);
            }
            setZValue(ACTIVE_Z);
            Update();
        }
        else if (m_moveType & IsResizable)
        {
            if (m_moveType & IsRotatable)
            {
                AllUserModifications(pME->pos());
            }
            else if (m_moveType & IsMovable)
            {
                UserMoveAndResize(pME->pos());
            }
            setZValue(ACTIVE_Z);
            Update();
        }
        else if (m_moveType & IsMovable)
        {
            if (m_moveType & IsRotatable)
            {
                UserRotateAndMove();
            }
            else if (m_moveType & IsResizable)
            {
                UserMoveAndResize(pME->pos());
            }
            else
            {
                m_eMode = mMove;
                SetOverrideCursor(cursorArrowCloseHand, 1, 1);
            }

            setZValue(ACTIVE_Z);
            Update();
        }
        else
        {
            pME->ignore();
            return;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGrainlineItem::mouseMoveEvent handles mouse move events, making sure that the item is moved properly
 * @param pME pointer to QGraphicsSceneMouseEvent object
 */
void VGrainlineItem::mouseMoveEvent(QGraphicsSceneMouseEvent* pME)
{
    QPointF ptDiff = pME->scenePos() - m_ptStartMove;
    qreal dX;
    qreal dY;
    if (m_eMode == mMove && m_moveType & IsMovable)
    {
        QPointF pt = m_ptStartPos + ptDiff;
        if (IsContained(pt, m_dRotation, dX, dY) == false)
        {
            pt.setX(pt.x() + dX);
            pt.setY(pt.y() + dY);
        }
        setPos(pt);
        Update();
    }
    else if (m_eMode == mResize && m_moveType & IsResizable)
    {
        qreal dLen = qSqrt(ptDiff.x()*ptDiff.x() + ptDiff.y()*ptDiff.y());
        qreal dAng = qAtan2(-ptDiff.y(), ptDiff.x());
        dLen = -dLen*qCos(dAng - m_dRotation);
        qreal dPrevLen = m_dLength;
        // try with new length
        if (not (m_moveType & IsMovable))
        {
            dLen *= 2;
        }
        m_dLength = m_dStartLength + dLen;

        QPointF pos;

        if (m_moveType & IsMovable)
        {
            QLineF grainline(this->pos().x(), this->pos().y(),
                             this->pos().x() + dPrevLen, this->pos().y());
            grainline.setAngle(qRadiansToDegrees(m_dRotation));
            grainline = QLineF(grainline.p2(), grainline.p1());
            grainline.setLength(m_dLength);
            pos = grainline.p2();
        }
        else
        {
            QLineF grainline(m_ptCenter.x(), m_ptCenter.y(),
                             m_ptCenter.x() + m_dLength / 2.0, m_ptCenter.y());

            grainline.setAngle(qRadiansToDegrees(m_dRotation));
            grainline = QLineF(grainline.p2(), grainline.p1());
            grainline.setLength(m_dLength);

            pos = grainline.p2();
        }

        qreal dX;
        qreal dY;
        if (IsContained(pos, m_dRotation, dX, dY) == false)
        {
            m_dLength = dPrevLen;
        }
        else
        {
            setPos(pos);
        }

        UpdateRectangle();
        Update();
    }
    else if (m_eMode == mRotate && m_moveType & IsRotatable)
    {
        // prevent strange angle changes due to singularities
        qreal dLen = qSqrt(ptDiff.x()*ptDiff.x() + ptDiff.y()*ptDiff.y());
        if (dLen < 2)
        {
            return;
        }

        if (fabs(m_dAngle) < 0.01)
        {
            m_dAngle = GetAngle(mapToParent(pME->pos()));
            return;
        }

        qreal dAng = GetAngle(mapToParent(pME->pos())) - m_dAngle;
        QPointF ptNewPos = Rotate(m_ptStartPos, m_ptRotCenter, dAng);
        if (IsContained(ptNewPos, m_dStartRotation + dAng, dX, dY) == true)
        {
            setPos(ptNewPos);
            m_dRotation = m_dStartRotation + dAng;
            UpdateRectangle();
            Update();
        }
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
        if (m_eMode == mMove || m_eMode == mRotate)
        {
            RestoreOverrideCursor(cursorArrowCloseHand);
        }
        else if (m_eMode == mResize)
        {
            RestoreOverrideCursor(Qt::SizeFDiagCursor);
        }

        QPointF ptDiff = pME->scenePos() - m_ptStartMove;
        qreal dLen = qSqrt(ptDiff.x()*ptDiff.x() + ptDiff.y()*ptDiff.y());
        bool bShort = (dLen < 2);

        if (m_eMode == mMove || m_eMode == mResize)
        {
            if (bShort == true)
            {
                if (m_bReleased == true && m_moveType & IsRotatable)
                {
                    m_eMode = mRotate;
                    Update();
                }
            }
            else
            {
                if (m_eMode == mMove && m_moveType & IsMovable)
                {
                    emit SignalMoved(pos());
                }
                else if (m_moveType & IsResizable)
                {
                    emit SignalResized(m_dLength);
                }
                Update();
            }
        }
        else
        {
            if (bShort == true)
            {
                m_eMode = mMove;
            }
            else if (m_moveType & IsRotatable)
            {
                emit SignalRotated(m_dRotation, m_ptStart);
            }
            Update();
        }
        m_bReleased = true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VGrainlineItem::hoverEnterEvent(QGraphicsSceneHoverEvent *pME)
{
    m_penWidth = LINE_PEN_WIDTH + 1;
    VPieceItem::hoverEnterEvent(pME);
}

//---------------------------------------------------------------------------------------------------------------------
void VGrainlineItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *pME)
{
    m_penWidth = LINE_PEN_WIDTH;
    VPieceItem::hoverLeaveEvent(pME);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGrainlineItem::UpdateBox updates the item
 */
void VGrainlineItem::Update()
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
    QPointF pt1(0, 0);
    QPointF pt2(pt1.x() + m_dLength * cos(m_dRotation), pt1.y() - m_dLength * sin(m_dRotation));

    m_ptStart = mapToParent(pt1);
    m_ptFinish = mapToParent(pt2);
    m_ptCenter = (m_ptStart + m_ptFinish)/2;

    m_polyBound.clear();
    m_polyBound << QPointF(pt1.x() + RECT_WIDTH*cos(m_dRotation + M_PI/2),
                           pt1.y() - RECT_WIDTH*sin(m_dRotation + M_PI/2));
    m_polyBound << QPointF(pt1.x() + RECT_WIDTH*cos(m_dRotation - M_PI/2),
                           pt1.y() - RECT_WIDTH*sin(m_dRotation - M_PI/2));
    m_polyBound << QPointF(pt2.x() + RECT_WIDTH*cos(m_dRotation - M_PI/2),
                           pt2.y() - RECT_WIDTH*sin(m_dRotation - M_PI/2));
    m_polyBound << QPointF(pt2.x() + RECT_WIDTH*cos(m_dRotation + M_PI/2),
                           pt2.y() - RECT_WIDTH*sin(m_dRotation + M_PI/2));
    m_rectBoundingBox = m_polyBound.boundingRect();
    setTransformOriginPoint(m_rectBoundingBox.center());

    UpdatePolyResize();
    prepareGeometryChange();
}

//---------------------------------------------------------------------------------------------------------------------
double VGrainlineItem::GetAngle(const QPointF &pt) const
{
    return -VPieceItem::GetAngle(pt);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGrainlineItem::Rotate rotates point pt around ptCenter by angle dAng [rad]
 * and returns the resulting point
 * @param pt point to rotate
 * @param ptCenter center of rotation
 * @param dAng angle of rotation
 * @return point, which is a result of rotating pt around ptCenter by angle dAng
 */
QPointF VGrainlineItem::Rotate(const QPointF& pt, const QPointF& ptCenter, qreal dAng) const
{
    QPointF ptRel = pt - ptCenter;
    QPointF ptFinal;
    ptFinal.setX(ptRel.x()*qCos(dAng) + ptRel.y()*qSin(dAng));
    ptFinal.setY(-ptRel.x()*qSin(dAng) + ptRel.y()*qCos(dAng));
    ptFinal += ptCenter;
    return ptFinal;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGrainlineItem::GetInsideCorner calculates a point inside the bounding polygon,
 * dDist away of i-th point in each direction
 * @param i index of corner
 * @param dDist distance
 * @return resulting point
 */
QPointF VGrainlineItem::GetInsideCorner(int i, qreal dDist) const
{
    QPointF pt1 = m_polyBound.at((i + 1) % m_polyBound.count()) - m_polyBound.at(i);
    QPointF pt2 = m_polyBound.at((i + m_polyBound.count() - 1) % m_polyBound.count()) - m_polyBound.at(i);

    pt1 = dDist*pt1/qSqrt(pt1.x()*pt1.x() + pt1.y()*pt1.y());
    pt2 = dDist*pt2/qSqrt(pt2.x()*pt2.x() + pt2.y()*pt2.y());

    return m_polyBound.at(i) + pt1 + pt2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetScale gets the scale for keeping the arrows of constant size
 */
qreal VGrainlineItem::GetScale() const
{
    if (scene()->views().count() > 0)
    {
        const QPoint pt0 = scene()->views().at(0)->mapFromScene(0, 0);
        const QPoint pt = scene()->views().at(0)->mapFromScene(0, 100);
        const QPoint p = pt - pt0;
        qreal dScale = qSqrt(QPoint::dotProduct(p, p));
        dScale = 100.0/dScale;
        if (dScale < 1.0)
        {
            dScale = 1.0;
        }
        return dScale;
    }

    return 1.0;
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VGrainlineItem::MainLine() const
{
    QPointF pt1;
    QPointF pt2(pt1.x() + m_dLength * cos(m_dRotation), pt1.y() - m_dLength * sin(m_dRotation));
    return QLineF(pt1, pt2);
}

//---------------------------------------------------------------------------------------------------------------------
QPolygonF VGrainlineItem::FirstArrow(qreal dArrLen) const
{
    const QPointF pt2 = MainLine().p2();
    QPolygonF poly;
    poly << pt2;
    poly << QPointF(pt2.x() + dArrLen*cos(M_PI + m_dRotation + ARROW_ANGLE),
                    pt2.y() - dArrLen*sin(M_PI + m_dRotation + ARROW_ANGLE));
    poly << QPointF(pt2.x() + dArrLen*cos(M_PI + m_dRotation - ARROW_ANGLE),
                    pt2.y() - dArrLen*sin(M_PI + m_dRotation - ARROW_ANGLE));
    return poly;
}

//---------------------------------------------------------------------------------------------------------------------
QPolygonF VGrainlineItem::SecondArrow(qreal dArrLen) const
{
    const QPointF pt1 = MainLine().p1();
    QPolygonF poly;
    poly << pt1;
    poly << QPointF(pt1.x() + dArrLen*cos(m_dRotation + ARROW_ANGLE),
                    pt1.y() - dArrLen*sin(m_dRotation + ARROW_ANGLE));
    poly << QPointF(pt1.x() + dArrLen*cos(m_dRotation - ARROW_ANGLE),
                    pt1.y() - dArrLen*sin(m_dRotation - ARROW_ANGLE));
    return poly;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VGrainlineItem::MainShape() const
{
    QPainterPath path;
    const QLineF mainLine = MainLine();
    QPainterPath linePath;
    linePath.moveTo(mainLine.p1());
    linePath.lineTo(mainLine.p2());
    linePath.closeSubpath();

    QPainterPathStroker stroker;
    stroker.setWidth(m_penWidth);
    path.addPath((stroker.createStroke(linePath) + linePath).simplified());
    path.closeSubpath();

    const qreal dArrLen = ARROW_LENGTH*GetScale();
    if (m_eArrowType != ArrowType::atRear)
    {
        // first arrow
        QPainterPath polyPath;
        polyPath.addPolygon(FirstArrow(dArrLen));
        path.addPath((stroker.createStroke(polyPath) + polyPath).simplified());
        path.closeSubpath();
    }

    if (m_eArrowType != ArrowType::atFront)
    {
        // second arrow
        QPainterPath polyPath;
        polyPath.addPolygon(SecondArrow(dArrLen));
        path.addPath((stroker.createStroke(polyPath) + polyPath).simplified());
        path.closeSubpath();
    }
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
void VGrainlineItem::AllUserModifications(const QPointF &pos)
{
    if (m_eMode != mRotate)
    {
        UserMoveAndResize(pos);
    }
    else
    {
        SetOverrideCursor(cursorArrowCloseHand, 1, 1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VGrainlineItem::UserRotateAndMove()
{
    if (m_eMode != mRotate)
    {
        m_eMode = mMove;
    }
    SetOverrideCursor(cursorArrowCloseHand, 1, 1);
}

//---------------------------------------------------------------------------------------------------------------------
void VGrainlineItem::UserMoveAndResize(const QPointF &pos)
{
    if (m_polyResize.containsPoint(pos, Qt::OddEvenFill) == true)
    {
        m_eMode = mResize;
        SetOverrideCursor(Qt::SizeFDiagCursor);
    }
    else
    {
        m_eMode = mMove; // block later if need
        SetOverrideCursor(cursorArrowCloseHand, 1, 1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VGrainlineItem::UpdatePolyResize()
{
    m_polyResize.clear();
    QPointF ptA = m_polyBound.at(1);
    m_polyResize << ptA;
    const double dSize = m_dScale * RESIZE_RECT_SIZE;

    ptA.setX(ptA.x() - dSize*cos(m_dRotation - M_PI/2));
    ptA.setY(ptA.y() + dSize*sin(m_dRotation - M_PI/2));
    m_polyResize << ptA;

    ptA.setX(ptA.x() + dSize*cos(m_dRotation));
    ptA.setY(ptA.y() - dSize*sin(m_dRotation));
    m_polyResize << ptA;

    ptA.setX(ptA.x() - dSize*cos(m_dRotation + M_PI/2));
    ptA.setY(ptA.y() + dSize*sin(m_dRotation + M_PI/2));
    m_polyResize << ptA;
}
