/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
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
 ****************************************************************************/

#include "vtoolsinglepoint.h"

const QString VToolSinglePoint::ToolType = QStringLiteral("single");

VToolSinglePoint::VToolSinglePoint (VDomDocument *doc, VContainer *data, qint64 id, Tool::Sources typeCreation,
                                    QGraphicsItem * parent )
    :VToolPoint(doc, data, id, parent), dialogSinglePoint(QSharedPointer<DialogSinglePoint>())
{
    ignoreFullUpdate = true;
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
}

void VToolSinglePoint::setDialog()
{
    Q_ASSERT(dialogSinglePoint.isNull() == false);
    VPointF p = VAbstractTool::data.GetPoint(id);
    dialogSinglePoint->setData(p.name(), p.toQPointF());
}

void VToolSinglePoint::AddToFile()
{
    VPointF point = VAbstractTool::data.GetPoint(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrName, point.name());
    AddAttribute(domElement, AttrX, toMM(point.x()));
    AddAttribute(domElement, AttrY, toMM(point.y()));
    AddAttribute(domElement, AttrMx, toMM(point.mx()));
    AddAttribute(domElement, AttrMy, toMM(point.my()));

    AddToCalculation(domElement);
}

QVariant VToolSinglePoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene())
    {
        // value - это новое положение.
        QPointF newPos = value.toPointF();
        QRectF rect = scene()->sceneRect();
        if (rect.contains(newPos) == false)
        {
            // Сохраняем элемент внутри прямоугольника сцены.
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
            return newPos;
        }
    }
    if (change == ItemPositionHasChanged && scene())
    {
        // value - это новое положение.
        QPointF newPos = value.toPointF();
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            domElement.setAttribute(AttrX, QString().setNum(toMM(newPos.x())));
            domElement.setAttribute(AttrY, QString().setNum(toMM(newPos.y())));
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

void VToolSinglePoint::contextMenuEvent ( QGraphicsSceneContextMenuEvent * event )
{
    ContextMenu(dialogSinglePoint, this, event, false);
}

void  VToolSinglePoint::FullUpdateFromFile()
{
    RefreshPointGeometry(VAbstractTool::data.GetPoint(id));
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
            domElement.setAttribute(AttrName, name);
            domElement.setAttribute(AttrX, QString().setNum(toMM(p.x())));
            domElement.setAttribute(AttrY, QString().setNum(toMM(p.y())));
            //I don't now why but signal does not work.
            doc->FullUpdateTree();
        }
    }
    dialogSinglePoint.clear();
}

void VToolSinglePoint::ChangedActivDraw(const QString newName)
{
    if (nameActivDraw == newName)
    {
        this->setFlag(QGraphicsItem::ItemIsSelectable, true);
        VToolPoint::ChangedActivDraw(newName);
    }
    else
    {
        this->setFlag(QGraphicsItem::ItemIsSelectable, false);
        VToolPoint::ChangedActivDraw(newName);
    }
}

void VToolSinglePoint::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshPointGeometry(VAbstractTool::data.GetPoint(id));
}
