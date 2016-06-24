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

#include <QGraphicsTextItem>

class VTextGraphicsItem : public QGraphicsTextItem
{
    Q_OBJECT

    enum Mode {
        mNormal,
        mActivated,
        mMove,
        mResize
    };

public:
    VTextGraphicsItem(QGraphicsItem* pParent = 0);
    ~VTextGraphicsItem();

    void                paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void                Reset();

    QRectF              boundingRect() const;
    void                SetHTML(const QString& qsHtml);

signals:
    void                SignalMoved(QPointF ptPos);
    void                SignalResized(qreal dTW, int iFontSize);

protected:
    void                mousePressEvent(QGraphicsSceneMouseEvent* pME);
    void                mouseMoveEvent(QGraphicsSceneMouseEvent* pME);
    void                mouseReleaseEvent(QGraphicsSceneMouseEvent* pME);

    void                Update();
    void                SetResizeArea();

    void                UpdateFont();

private:
    Mode                m_eMode;
    QPointF             m_ptStart;
    QRectF              m_rectResize;
    int                 m_iMinH;
    QRectF              m_rectBoundingBox;
};

#endif // VTEXTGRAPHICSITEM_H
