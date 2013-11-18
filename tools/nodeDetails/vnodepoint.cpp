/************************************************************************
 **
 **  @file   vnodepoint.cpp
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

#include "vnodepoint.h"

const QString VNodePoint::TagName = QStringLiteral("point");
const QString VNodePoint::ToolType = QStringLiteral("modeling");

VNodePoint::VNodePoint(VDomDocument *doc, VContainer *data, qint64 id, qint64 idPoint, Draw::Draws typeobject,
                       const Tool::Sources &typeCreation, QGraphicsItem *parent)
    :VAbstractNode(doc, data, id, idPoint, typeobject), QGraphicsEllipseItem(parent), radius(toPixel(1.5)),
      namePoint(0), lineName(0)
{
    namePoint = new VGraphicsSimpleTextItem(this);
    lineName = new QGraphicsLineItem(this);
    connect(namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this,
            &VNodePoint::NameChangePosition);
    this->setPen(QPen(Qt::black, widthHairLine));
    this->setBrush(QBrush(Qt::NoBrush));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);
    RefreshPointGeometry(VAbstractTool::data.GetPointModeling(id));
    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
}

void VNodePoint::Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idPoint, const Draw::Draws &typeobject,
                        const Document::Documents &parse, const Tool::Sources &typeCreation)
{
    if (parse == Document::FullParse)
    {
        VNodePoint *point = new VNodePoint(doc, data, id, idPoint, typeobject, typeCreation);
        Q_ASSERT(point != 0);
        doc->AddTool(id, point);
        doc->IncrementReferens(idPoint);
    }
    else
    {
        doc->UpdateToolData(id, data);
    }
}

void VNodePoint::FullUpdateFromFile()
{
    RefreshPointGeometry(VAbstractTool::data.GetPointModeling(id));
}

void VNodePoint::AddToFile()
{
    VPointF point = VAbstractTool::data.GetPointModeling(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrIdObject, idNode);
    if (typeobject == Draw::Calculation)
    {
        AddAttribute(domElement, AttrTypeObject, TypeObjectCalculation);
    }
    else
    {
        AddAttribute(domElement, AttrTypeObject, TypeObjectModeling);
    }
    AddAttribute(domElement, AttrMx, toMM(point.mx()));
    AddAttribute(domElement, AttrMy, toMM(point.my()));

    AddToModeling(domElement);
}

void VNodePoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit ChoosedTool(id, Scene::Point);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VNodePoint::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine));
}

void VNodePoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine));
}


void VNodePoint::NameChangePosition(const QPointF &pos)
{
    VPointF point = VAbstractTool::data.GetPointModeling(id);
    QPointF p = pos - this->pos();
    point.setMx(p.x());
    point.setMy(p.y());
    RefreshLine();
    UpdateNamePosition(point.mx(), point.my());
    VAbstractTool::data.UpdatePoint(id, point);
}

void VNodePoint::UpdateNamePosition(qreal mx, qreal my)
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        domElement.setAttribute(AttrMx, QString().setNum(toMM(mx)));
        domElement.setAttribute(AttrMy, QString().setNum(toMM(my)));
        emit toolhaveChange();
    }
}

void VNodePoint::RefreshPointGeometry(const VPointF &point)
{
    QRectF rec = QRectF(0, 0, radius*2, radius*2);
    rec.translate(-rec.center().x(), -rec.center().y());
    this->setRect(rec);
    this->setPos(point.toQPointF());
    disconnect(namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this,
               &VNodePoint::NameChangePosition);
    namePoint->setText(point.name());
    namePoint->setPos(QPointF(point.mx(), point.my()));
    connect(namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this,
            &VNodePoint::NameChangePosition);

    RefreshLine();
}

void VNodePoint::RefreshLine()
{
    QRectF nameRec = namePoint->sceneBoundingRect();
    QPointF p1, p2;
    LineIntersectCircle(QPointF(), radius, QLineF(QPointF(), nameRec.center()- scenePos()), p1, p2);
    QPointF pRec = LineIntersectRect(nameRec, QLineF(scenePos(), nameRec.center()));
    lineName->setLine(QLineF(p1, pRec - scenePos()));
    if (QLineF(p1, pRec - scenePos()).length() <= toPixel(4))
    {
        lineName->setVisible(false);
    }
    else
    {
        lineName->setVisible(true);
    }
}
