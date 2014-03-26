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
#include "../../dialogs/tools/dialogsinglepoint.h"

const QString VToolSinglePoint::ToolType = QStringLiteral("single");

VToolSinglePoint::VToolSinglePoint (VPattern *doc, VContainer *data, quint32 id, const Valentina::Sources &typeCreation,
                                    QGraphicsItem * parent )
    :VToolPoint(doc, data, id, parent)
{
    baseColor = Qt::red;
    currentColor = baseColor;
    ignoreFullUpdate = true;
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, false);
    setColorLabel(Qt::black);
    if (typeCreation == Valentina::FromGui)
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
    Q_CHECK_PTR(dialog);
    DialogSinglePoint *dialogTool = qobject_cast<DialogSinglePoint*>(dialog);
    Q_CHECK_PTR(dialogTool);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogTool->setData(p->name(), p->toQPointF());
}

void VToolSinglePoint::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrName, point->name());
    doc->SetAttribute(domElement, AttrX, qApp->fromPixel(point->x()));
    doc->SetAttribute(domElement, AttrY, qApp->fromPixel(point->y()));
    doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));

    AddToCalculation(domElement);
}

void VToolSinglePoint::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrName, point->name());
        doc->SetAttribute(domElement, AttrX, QString().setNum(qApp->fromPixel(point->x())));
        doc->SetAttribute(domElement, AttrY, QString().setNum(qApp->fromPixel(point->y())));
        doc->SetAttribute(domElement, AttrMx, QString().setNum(qApp->fromPixel(point->mx())));
        doc->SetAttribute(domElement, AttrMy, QString().setNum(qApp->fromPixel(point->my())));
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
            doc->SetAttribute(domElement, AttrX, QString().setNum(qApp->fromPixel(newPos.x())));
            doc->SetAttribute(domElement, AttrY, QString().setNum(qApp->fromPixel(newPos.y())));

            QList<QGraphicsView*> list = this->scene()->views();
            VAbstractTool::NewSceneRect(this->scene(), list[0]);

            //I don't now why but signal does not work.
            doc->FullUpdateTree();
            emit toolhaveChange();
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

void VToolSinglePoint::SaveDialog(QDomElement &domElement)
{
    Q_CHECK_PTR(dialog);
    DialogSinglePoint *dialogTool = qobject_cast<DialogSinglePoint*>(dialog);
    Q_CHECK_PTR(dialogTool);
    QPointF p = dialogTool->getPoint();
    QString name = dialogTool->getName();
    doc->SetAttribute(domElement, AttrName, name);
    doc->SetAttribute(domElement, AttrX, QString().setNum(qApp->fromPixel(p.x())));
    doc->SetAttribute(domElement, AttrY, QString().setNum(qApp->fromPixel(p.y())));
}

void VToolSinglePoint::setColorLabel(const Qt::GlobalColor &color)
{
    namePoint->setBrush(color);
    lineName->setPen(QPen(color, qApp->toPixel(qApp->widthHairLine())/factor));
}

void VToolSinglePoint::contextMenuEvent ( QGraphicsSceneContextMenuEvent * event )
{
    ContextMenu<DialogSinglePoint>(this, event, false);
}

void  VToolSinglePoint::FullUpdateFromFile()
{
    VPointF point = *VAbstractTool::data.GeometricObject<const VPointF *>(id);
    RefreshPointGeometry(point);
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
    ContextMenu<DialogSinglePoint>(this, event, false);
}
