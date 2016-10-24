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

#ifndef VGRAINLINEITEM_H
#define VGRAINLINEITEM_H

#include <QGraphicsObject>

#include "../vpatterndb/vgrainlinegeometry.h"

class QGraphicsObject;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

class VGrainlineItem : public QGraphicsObject
{
    Q_OBJECT

    enum Mode {
        mNormal,
        mMove,
        mResize,
        mRotate
    };

public:
    explicit VGrainlineItem(QGraphicsItem* pParent = nullptr);
    virtual ~VGrainlineItem();

    void                    paint(QPainter* pP, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget);
    void                    UpdateGeometry(const QPointF& ptPos, qreal dRotation, qreal dLength, bool bFA, bool bRA);

    QRectF                  boundingRect() const;
    void                    Reset();
    bool                    IsIdle() const;
    bool                    IsContained(const QPointF &pt, qreal dRot, qreal &dX, qreal &dY) const;
    void                    SetScale(qreal dScale);

protected:
    void                    mousePressEvent(QGraphicsSceneMouseEvent* pME);
    void                    mouseMoveEvent(QGraphicsSceneMouseEvent* pME);
    void                    mouseReleaseEvent(QGraphicsSceneMouseEvent* pME);
    void                    UpdateBox();
    void                    UpdateRectangle();

    qreal                   GetAngle(const QPointF& pt) const;
    QPointF                 Rotate(const QPointF& pt, const QPointF& ptCenter, qreal dAng) const;
    QPointF                 GetInsideCorner(int i, qreal dDist) const;

signals:
    void                    SignalMoved(const QPointF& ptPos);
    void                    SignalResized(qreal dLength);
    void                    SignalRotated(qreal dRot, const QPointF& ptNewPos);

private:
    Mode                    m_eMode;
    bool                    m_bReleased;
    qreal                   m_dRotation;
    qreal                   m_dStartRotation;
    qreal                   m_dLength;
    QRectF                  m_rectBoundingBox;
    QPolygonF               m_polyBound;
    QPointF                 m_ptStartPos;
    QPointF                 m_ptStartMove;
    qreal                   m_dScale;
    QPolygonF               m_polyResize;
    qreal                   m_dStartLength;
    QPointF                 m_ptStart;
    QPointF                 m_ptFinish;
    QPointF                 m_ptCenter;
    QPointF                 m_ptRotCenter;
    qreal                   m_dAngle;
    bool                    m_bFrontArrow;
    bool                    m_bRearArrow;
};

#endif // VGRAINLINEITEM_H
