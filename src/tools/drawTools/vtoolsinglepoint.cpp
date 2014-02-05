/************************************************************************
 **
 **  @file   vtoolsinglepoint.cpp
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

#include "vtoolsinglepoint.h"

const QString VToolSinglePoint::ToolType = QStringLiteral("single");

VToolSinglePoint::VToolSinglePoint (VDomDocument *doc, VContainer *data, qint64 id, const Tool::Sources &typeCreation,
                                    QGraphicsItem * parent )
    :VToolPoint(doc, data, id, parent), dialogSinglePoint(QSharedPointer<DialogSinglePoint>())
{
    baseColor = Qt::red;
    currentColor = baseColor;
    ignoreFullUpdate = true;
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, false);
    setColorLabel(Qt::black);
    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

void VToolSinglePoint::setDialog()
{
    Q_ASSERT(dialogSinglePoint.isNull() == false);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogSinglePoint->setData(p->name(), p->toQPointF());
}

void VToolSinglePoint::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    SetAttribute(domElement, AttrId, id);
    SetAttribute(domElement, AttrType, ToolType);
    SetAttribute(domElement, AttrName, point->name());
    SetAttribute(domElement, AttrX, toMM(point->x()));
    SetAttribute(domElement, AttrY, toMM(point->y()));
    SetAttribute(domElement, AttrMx, toMM(point->mx()));
    SetAttribute(domElement, AttrMy, toMM(point->my()));

    AddToCalculation(domElement);
}

void VToolSinglePoint::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        SetAttribute(domElement, AttrName, point->name());
        SetAttribute(domElement, AttrX, QString().setNum(toMM(point->x())));
        SetAttribute(domElement, AttrY, QString().setNum(toMM(point->y())));
        SetAttribute(domElement, AttrMx, QString().setNum(toMM(point->mx())));
        SetAttribute(domElement, AttrMy, QString().setNum(toMM(point->my())));
    }
}

QVariant VToolSinglePoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene())
    {
        // value - this is new position.
        QPointF newPos = value.toPointF();
        QRectF rect = scene()->sceneRect();
        if (rect.contains(newPos) == false)
        {
            // Save element into rect of scene.
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
            return newPos;
        }
    }
    if (change == ItemPositionHasChanged && scene())
    {
        // value - this is new position.
        QPointF newPos = value.toPointF();
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            SetAttribute(domElement, AttrX, QString().setNum(toMM(newPos.x())));
            SetAttribute(domElement, AttrY, QString().setNum(toMM(newPos.y())));

            QList<QGraphicsView*> list = this->scene()->views();
            VAbstractTool::NewSceneRect(this->scene(), list[0]);

            //I don't now why but signal does not work.
            doc->FullUpdateTree();
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

void VToolSinglePoint::decrementReferens()
{
    if (_referens > 1)
    {
        --_referens;
    }
}

void VToolSinglePoint::setColorLabel(const Qt::GlobalColor &color)
{
    namePoint->setBrush(color);
    lineName->setPen(QPen(color, widthHairLine/factor));
}

void VToolSinglePoint::contextMenuEvent ( QGraphicsSceneContextMenuEvent * event )
{
    ContextMenu(dialogSinglePoint, this, event, false);
}

void  VToolSinglePoint::FullUpdateFromFile()
{
    VPointF point = *VAbstractTool::data.GeometricObject<const VPointF *>(id);
    RefreshPointGeometry(point);
}

void VToolSinglePoint::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QPointF p = dialogSinglePoint->getPoint();
        QString name = dialogSinglePoint->getName();
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            SetAttribute(domElement, AttrName, name);
            SetAttribute(domElement, AttrX, QString().setNum(toMM(p.x())));
            SetAttribute(domElement, AttrY, QString().setNum(toMM(p.y())));
            //I don't now why but signal does not work.
            doc->FullUpdateTree();
            emit toolhaveChange();
        }
    }
    dialogSinglePoint.clear();
}

void VToolSinglePoint::ChangedActivDraw(const QString &newName)
{
    if (nameActivDraw == newName)
    {
        this->setFlag(QGraphicsItem::ItemIsSelectable, true);
        VToolPoint::ChangedActivDraw(newName);
        setColorLabel(Qt::black);
    }
    else
    {
        this->setFlag(QGraphicsItem::ItemIsSelectable, false);
        VToolPoint::ChangedActivDraw(newName);
        setColorLabel(Qt::gray);
    }
}

void VToolSinglePoint::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshPointGeometry(*(VAbstractTool::data.GeometricObject<const VPointF *>(id)));
}

void VToolSinglePoint::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogSinglePoint, this, event, false);
}
