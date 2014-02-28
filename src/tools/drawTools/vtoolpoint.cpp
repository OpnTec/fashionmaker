/************************************************************************
 **
 **  @file   vtoolpoint.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#include "vtoolpoint.h"

const QString VToolPoint::TagName = QStringLiteral("point");

VToolPoint::VToolPoint(VPattern *doc, VContainer *data, quint32 id, QGraphicsItem *parent):VDrawTool(doc, data, id),
    QGraphicsEllipseItem(parent), radius(toPixel(2)), namePoint(0), lineName(0)
{
    namePoint = new VGraphicsSimpleTextItem(this);
    Q_CHECK_PTR(namePoint);
    connect(namePoint, &VGraphicsSimpleTextItem::ShowContextMenu, this, &VToolPoint::ShowContextMenu);
    namePoint->setBrush(Qt::black);
    lineName = new QGraphicsLineItem(this);
    Q_CHECK_PTR(lineName);
    lineName->setPen(QPen(Qt::black));
    connect(namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this, &VToolPoint::NameChangePosition);
    this->setBrush(QBrush(Qt::NoBrush));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setAcceptHoverEvents(true);
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<const VPointF *>(id));
}

void VToolPoint::NameChangePosition(const QPointF &pos)
{
    VPointF *point = new VPointF(*VAbstractTool::data.GeometricObject<const VPointF *>(id));
    Q_CHECK_PTR(point);
    QPointF p = pos - this->pos();
    point->setMx(p.x());
    point->setMy(p.y());
    RefreshLine();
    UpdateNamePosition(point->mx(), point->my());
    VAbstractTool::data.UpdateGObject(id, point);
}

void VToolPoint::UpdateNamePosition(qreal mx, qreal my)
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrMx, toMM(mx));
        doc->SetAttribute(domElement, AttrMy, toMM(my));
        emit toolhaveChange();
    }
}

void VToolPoint::ChangedActivDraw(const QString &newName)
{
    bool selectable = false;
    if (nameActivDraw == newName)
    {
        selectable = true;
        currentColor = baseColor;
    }
    else
    {
        selectable = false;
        currentColor = Qt::gray;
    }
    this->setPen(QPen(currentColor, toPixel(widthHairLine)/factor));
    this->setFlag(QGraphicsItem::ItemIsSelectable, selectable);
    this->setAcceptHoverEvents (selectable);
    namePoint->setFlag(QGraphicsItem::ItemIsMovable, selectable);
    namePoint->setFlag(QGraphicsItem::ItemIsSelectable, selectable);
    namePoint->setFlag(QGraphicsItem::ItemSendsGeometryChanges, selectable);
    namePoint->setBrush(QBrush(currentColor));
    namePoint->setAcceptHoverEvents(selectable);
    lineName->setPen(QPen(currentColor, toPixel(widthHairLine)/factor));
    VDrawTool::ChangedActivDraw(newName);
}

void VToolPoint::ShowTool(quint32 id, Qt::GlobalColor color, bool enable)
{
    ShowItem(this, id, color, enable);
}

void VToolPoint::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<const VPointF *>(id));
}

void VToolPoint::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    Q_UNUSED(event);
}

void VToolPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit ChoosedTool(id, Scene::Point);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VToolPoint::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, toPixel(widthMainLine)/factor));
}

void VToolPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, toPixel(widthHairLine)/factor));
}

void VToolPoint::RefreshPointGeometry(const VPointF &point)
{
    this->setPen(QPen(currentColor, toPixel(widthHairLine)/factor));
    QRectF rec = QRectF(0, 0, radius*2/factor, radius*2/factor);
    rec.translate(-rec.center().x(), -rec.center().y());
    this->setRect(rec);
    this->setPos(point.toQPointF());
    disconnect(namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this,
               &VToolPoint::NameChangePosition);
    QFont font = namePoint->font();
    font.setPointSize(static_cast<qint32>(namePoint->FontSize()/factor));
    namePoint->setFont(font);
    namePoint->setText(point.name());
    namePoint->setPos(QPointF(point.mx(), point.my()));
    connect(namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this,
            &VToolPoint::NameChangePosition);
    RefreshLine();
}

void VToolPoint::RefreshLine()
{
    QRectF nameRec = namePoint->sceneBoundingRect();
    QPointF p1, p2;
    LineIntersectCircle(QPointF(), radius/factor, QLineF(QPointF(), nameRec.center()- scenePos()), p1, p2);
    QPointF pRec = LineIntersectRect(nameRec, QLineF(scenePos(), nameRec.center()));
    lineName->setLine(QLineF(p1, pRec - scenePos()));
    if (currentColor == Qt::gray)
    {
        lineName->setPen(QPen(currentColor, toPixel(widthHairLine)/factor));
    }
    else
    {
        lineName->setPen(QPen(Qt::black, toPixel(widthHairLine)/factor));
    }
    if (QLineF(p1, pRec - scenePos()).length() <= toPixel(4))
    {
        lineName->setVisible(false);
    }
    else
    {
        lineName->setVisible(true);
    }
}

QVariant VToolPoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedChange)
    {
        if (value == true)
        {
            // do stuff if selected
            this->setFocus();
        }
        else
        {
            // do stuff if not selected
        }
    }

    return QGraphicsItem::itemChange(change, value);
}

void VToolPoint::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Delete:
            DeleteTool(this);
            break;
        default:
            break;
    }
    QGraphicsItem::keyReleaseEvent ( event );
}
