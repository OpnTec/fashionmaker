/************************************************************************
 **
 **  @file   vtextgraphicsitem.h
 **  @author Bojan Kverh
 **  @date   June 16, 2016
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

#ifndef VTEXTGRAPHICSITEM_H
#define VTEXTGRAPHICSITEM_H

#include <QGraphicsObject>
#include <QFont>
#include <QList>

#include "../vlayout/vtextmanager.h"

/**
 * @brief The VTextGraphicsItem class. This class implements text graphics item,
 * which can be dragged around, resized and rotated within the parent item. The text font
 * size will be automatically updated, so that the entire text will fit into the item.
 */
class VTextGraphicsItem : public QGraphicsObject
{
    Q_OBJECT

    enum Mode {
        mNormal,
        mMove,
        mResize,
        mRotate
    };

public:
    explicit VTextGraphicsItem(QGraphicsItem* pParent = nullptr);
    virtual ~VTextGraphicsItem();

    void                SetFont(const QFont& fnt);
    void                paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void                Reset();
    bool                IsIdle() const;

    int                 GetFontSize() const;
    QRectF              boundingRect() const;
    void                AddLine(const TextLine& tl);
    void                Clear();
    void                SetSize(qreal fW, qreal fH);
    void                Update();
    bool                IsContained(QRectF rectBB, qreal dRot, qreal& dX, qreal& dY) const;
    void                UpdateData(const QString& qsName, const VPatternPieceData& data);
    void                UpdateData(const VAbstractPattern* pDoc);
    int                 GetTextLines() const;

protected:
    void                mousePressEvent(QGraphicsSceneMouseEvent* pME);
    void                mouseMoveEvent(QGraphicsSceneMouseEvent* pME);
    void                mouseReleaseEvent(QGraphicsSceneMouseEvent* pME);
    void                UpdateBox();
    void                UpdateFont();

    double              GetAngle(QPointF pt) const;

signals:
    void                SignalMoved(const QPointF& ptPos);
    void                SignalResized(qreal iTW, int iFontSize);
    void                SignalRotated(qreal dAng);
    void                SignalShrink();

private:
    Mode                m_eMode;
    bool                m_bReleased;
    QPointF             m_ptStartPos;
    QPointF             m_ptStart;
    QPointF             m_ptRotCenter;
    QSizeF              m_szStart;
    double              m_dRotation;
    double              m_dAngle;
    QRectF              m_rectResize;
    int                 m_iMinH;
    QRectF              m_rectBoundingBox;
    VTextManager        m_tm;

    QRectF              GetBoundingRect(QRectF rectBB, qreal dRot) const;
};

#endif // VTEXTGRAPHICSITEM_H
