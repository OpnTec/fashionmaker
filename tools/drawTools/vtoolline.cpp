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

#include "vtoolline.h"

const QString VToolLine::TagName = QStringLiteral("line");

VToolLine::VToolLine(VDomDocument *doc, VContainer *data, qint64 id, qint64 firstPoint, qint64 secondPoint,
                     Tool::Sources typeCreation, QGraphicsItem *parent)
    :VDrawTool(doc, data, id), QGraphicsLineItem(parent), firstPoint(firstPoint), secondPoint(secondPoint),
    dialogLine(QSharedPointer<DialogLine>())
{
    ignoreFullUpdate = true;
    //Лінія
    VPointF first = data->GetPoint(firstPoint);
    VPointF second = data->GetPoint(secondPoint);
    this->setLine(QLineF(first.toQPointF(), second.toQPointF()));
    this->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);
    this->setPen(QPen(Qt::black, widthHairLine/factor));

    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
}

void VToolLine::setDialog()
{
    dialogLine->setFirstPoint(firstPoint);
    dialogLine->setSecondPoint(secondPoint);
}

void VToolLine::Create(QSharedPointer<DialogLine> &dialog, VMainGraphicsScene *scene, VDomDocument *doc,
                       VContainer *data)
{
    qint64 firstPoint = dialog->getFirstPoint();
    qint64 secondPoint = dialog->getSecondPoint();
    Create(0, firstPoint, secondPoint, scene, doc, data, Document::FullParse, Tool::FromGui);
}

void VToolLine::Create(const qint64 &_id, const qint64 &firstPoint, const qint64 &secondPoint,
                       VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data,
                       const Document::Documents &parse, Tool::Sources typeCreation)
{
    Q_ASSERT(scene != 0);
    Q_ASSERT(doc != 0);
    Q_ASSERT(data != 0);
    qint64 id = _id;
    if (typeCreation == Tool::FromGui)
    {
        id = data->getNextId();
        data->AddLine(firstPoint, secondPoint);
    }
    else
    {
        data->UpdateId(id);
        data->AddLine(firstPoint, secondPoint);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::LineTool, doc);
    if (parse == Document::FullParse)
    {
        VToolLine *line = new VToolLine(doc, data, id, firstPoint, secondPoint, typeCreation);
        Q_ASSERT(line != 0);
        scene->addItem(line);
        connect(line, &VToolLine::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(line, &VToolLine::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
        connect(scene, &VMainGraphicsScene::NewFactor, line, &VToolLine::SetFactor);
        doc->AddTool(id, line);
        doc->IncrementReferens(firstPoint);
        doc->IncrementReferens(secondPoint);
    }
}

void VToolLine::FullUpdateFromFile()
{
    RefreshGeometry();
}

void VToolLine::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            domElement.setAttribute(AttrFirstPoint, QString().setNum(dialogLine->getFirstPoint()));
            domElement.setAttribute(AttrSecondPoint, QString().setNum(dialogLine->getSecondPoint()));
            emit FullUpdateTree();
        }
    }
    dialogLine.clear();
}

void VToolLine::ShowTool(qint64 id, Qt::GlobalColor color, bool enable)
{
    ShowItem(this, id, color, enable);
}

void VToolLine::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

void VToolLine::ChangedActivDraw(const QString newName)
{
    bool selectable = false;
    if (nameActivDraw == newName)
    {
        selectable = true;
        currentColor = Qt::black;
    }
    else
    {
        selectable = false;
        currentColor = Qt::gray;
    }
    this->setPen(QPen(currentColor, widthHairLine/factor));
    this->setAcceptHoverEvents (selectable);
    VDrawTool::ChangedActivDraw(newName);
}

void VToolLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogLine, this, event);
}

void VToolLine::AddToFile()
{
    QDomElement domElement = doc->createElement(TagName);
    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrFirstPoint, firstPoint);
    AddAttribute(domElement, AttrSecondPoint, secondPoint);

    AddToCalculation(domElement);
}

void VToolLine::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine/factor));
}

void VToolLine::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine/factor));
}

void VToolLine::RemoveReferens()
{
    doc->DecrementReferens(firstPoint);
    doc->DecrementReferens(secondPoint);
}

void VToolLine::RefreshGeometry()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        firstPoint = domElement.attribute(AttrFirstPoint, "").toLongLong();
        secondPoint = domElement.attribute(AttrSecondPoint, "").toLongLong();
    }
    VPointF first = VAbstractTool::data.GetPoint(firstPoint);
    VPointF second = VAbstractTool::data.GetPoint(secondPoint);
    this->setLine(QLineF(first.toQPointF(), second.toQPointF()));
    this->setPen(QPen(currentColor, widthHairLine/factor));
}
