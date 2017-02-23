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

#include "vpieceitem.h"
#include "../vpatterndb/floatItemData/vgrainlinedata.h"

class VGrainlineItem : public VPieceItem
{
    Q_OBJECT
public:
    explicit VGrainlineItem(QGraphicsItem* pParent = nullptr);
    virtual ~VGrainlineItem();

    virtual void paint(QPainter* pP, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget) Q_DECL_OVERRIDE;
    void         UpdateGeometry(const QPointF& ptPos, qreal dRotation, qreal dLength, ArrowType eAT);

    bool IsContained(const QPointF &pt, qreal dRot, qreal &dX, qreal &dY) const;
    void SetScale(qreal dScale);

signals:
    void SignalResized(qreal dLength);
    void SignalRotated(qreal dRot, const QPointF& ptNewPos);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* pME) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* pME) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* pME) Q_DECL_OVERRIDE;
    virtual void Update() Q_DECL_OVERRIDE;
    void         UpdateRectangle();

    QPointF Rotate(const QPointF& pt, const QPointF& ptCenter, qreal dAng) const;
    QPointF GetInsideCorner(int i, qreal dDist) const;

private:
    Q_DISABLE_COPY(VGrainlineItem)
    qreal                         m_dRotation;
    qreal                         m_dStartRotation;
    qreal                         m_dLength;
    QPolygonF                     m_polyBound;
    QPointF                       m_ptStartPos;
    QPointF                       m_ptStartMove;
    qreal                         m_dScale;
    QPolygonF                     m_polyResize;
    qreal                         m_dStartLength;
    QPointF                       m_ptStart;
    QPointF                       m_ptFinish;
    QPointF                       m_ptCenter;
    qreal                         m_dAngle;
    ArrowType                     m_eArrowType;
};

#endif // VGRAINLINEITEM_H
