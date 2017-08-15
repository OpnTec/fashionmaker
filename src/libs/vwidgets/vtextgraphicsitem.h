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

#include <QFont>
#include <QGraphicsObject>
#include <QList>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QRectF>
#include <QSizeF>
#include <QString>
#include <QtGlobal>

#include "vpieceitem.h"
#include "../vlayout/vtextmanager.h"

/**
 * @brief The VTextGraphicsItem class. This class implements text graphics item,
 * which can be dragged around, resized and rotated within the parent item. The text font
 * size will be automatically updated, so that the entire text will fit into the item.
 */
class VTextGraphicsItem : public VPieceItem
{
    Q_OBJECT
public:
    explicit VTextGraphicsItem(QGraphicsItem* pParent = nullptr);
    virtual ~VTextGraphicsItem() Q_DECL_EQ_DEFAULT;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
    virtual void Update() Q_DECL_OVERRIDE;

    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::TextGraphicsItem)};

    void SetFont(const QFont& fnt);
    int  GetFontSize() const;
    void SetSize(qreal fW, qreal fH);
    bool IsContained(QRectF rectBB, qreal dRot, qreal& dX, qreal& dY) const;
    void UpdateData(const QString& qsName, const VPieceLabelData& data);
    void UpdateData(const VAbstractPattern* pDoc);
    int  GetTextLines() const;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* pME) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* pME) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* pME) Q_DECL_OVERRIDE;
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* pHE) Q_DECL_OVERRIDE;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* pHE) Q_DECL_OVERRIDE;

    void UpdateBox();
    void CorrectLabel();

signals:
    void SignalResized(qreal iTW, int iFontSize);
    void SignalRotated(qreal dAng);
    void SignalShrink();

private:
    Q_DISABLE_COPY(VTextGraphicsItem)
    QPointF      m_ptStartPos;
    QPointF      m_ptStart;
    QSizeF       m_szStart;
    double       m_dRotation;
    double       m_dAngle;
    QRectF       m_rectResize;
    VTextManager m_tm;

    void AllUserModifications(const QPointF &pos);
    void UserRotateAndMove();
    void UserMoveAndResize(const QPointF &pos);
};

#endif // VTEXTGRAPHICSITEM_H
