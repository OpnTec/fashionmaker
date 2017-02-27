/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 1, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

#ifndef VPIECEITEM_H
#define VPIECEITEM_H

#include <QtCore/qglobal.h>
#include <QGraphicsObject>

class VPieceItem : public QGraphicsObject
{
    Q_OBJECT
public:
    enum MoveType {Movable, OnlyResizable, OnlyRotatable, NotMovable, Error};

    explicit VPieceItem(QGraphicsItem* pParent = nullptr);
    virtual ~VPieceItem();

    virtual QRectF boundingRect() const Q_DECL_OVERRIDE;

    virtual void Update() =0;

    void Reset();
    bool IsIdle() const;

    double GetAngle(const QPointF &pt) const;

    MoveType GetMoveType() const;
    void     SetMoveType(const MoveType &moveType);

signals:
    void SignalMoved(const QPointF &ptPos);

protected:
    enum Mode
    {
        mNormal,
        mMove,
        mResize,
        mRotate
    };
    QRectF   m_rectBoundingBox;
    Mode     m_eMode;
    bool     m_bReleased;
    QPointF  m_ptRotCenter;
    MoveType m_moveType;

    qreal m_inactiveZ;

private:
    Q_DISABLE_COPY(VPieceItem)
};

#endif // VPIECEITEM_H
