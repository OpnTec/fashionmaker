/************************************************************************
 **
 **  @file   vmodelingline.cpp
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

#include "vmodelingline.h"

const QString VModelingLine::TagName = QStringLiteral("line");

VModelingLine::VModelingLine(VDomDocument *doc, VContainer *data, qint64 id, qint64 firstPoint,
                             qint64 secondPoint, const Tool::Sources &typeCreation, QGraphicsItem *parent):
    VModelingTool(doc, data, id), QGraphicsLineItem(parent), firstPoint(firstPoint),
    secondPoint(secondPoint), dialogLine(QSharedPointer<DialogLine>())
{
    ignoreFullUpdate = true;
    //Лінія
    VPointF first = data->GetPointModeling(firstPoint);
    VPointF second = data->GetPointModeling(secondPoint);
    this->setLine(QLineF(first.toQPointF(), second.toQPointF()));
    this->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);

    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
}

void VModelingLine::setDialog()
{
    dialogLine->setFirstPoint(firstPoint);
    dialogLine->setSecondPoint(secondPoint);
}

VModelingLine *VModelingLine::Create(QSharedPointer<DialogLine> &dialog, VDomDocument *doc, VContainer *data)
{
    qint64 firstPoint = dialog->getFirstPoint();
    qint64 secondPoint = dialog->getSecondPoint();
    return Create(0, firstPoint, secondPoint, doc, data, Document::FullParse, Tool::FromGui);
}

VModelingLine *VModelingLine::Create(const qint64 &_id, const qint64 &firstPoint, const qint64 &secondPoint,
                                     VDomDocument *doc, VContainer *data, const Document::Documents &parse,
                                     const Tool::Sources &typeCreation)
{
    VModelingLine *line = 0;
    Q_ASSERT(doc != 0);
    Q_ASSERT(data != 0);
    qint64 id = _id;
    if (typeCreation == Tool::FromGui)
    {
        id = data->getNextId();
    }
    else
    {
        if (parse != Document::FullParse)
        {
            data->UpdateId(id);
            doc->UpdateToolData(id, data);
        }
    }
    data->AddLine(firstPoint, secondPoint, Draw::Modeling);
    if (parse == Document::FullParse)
    {
        line = new VModelingLine(doc, data, id, firstPoint, secondPoint, typeCreation);
        doc->AddTool(id, line);
        doc->IncrementReferens(firstPoint);
        doc->IncrementReferens(secondPoint);
    }
    return line;
}

void VModelingLine::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        firstPoint = domElement.attribute(AttrFirstPoint, "").toLongLong();
        secondPoint = domElement.attribute(AttrSecondPoint, "").toLongLong();
    }
    VPointF first = VAbstractTool::data.GetPointModeling(firstPoint);
    VPointF second = VAbstractTool::data.GetPointModeling(secondPoint);
    this->setLine(QLineF(first.toQPointF(), second.toQPointF()));
}

void VModelingLine::FullUpdateFromGui(int result)
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

void VModelingLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogLine, this, event);
}

void VModelingLine::AddToFile()
{
    QDomElement domElement = doc->createElement(TagName);
    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrFirstPoint, firstPoint);
    AddAttribute(domElement, AttrSecondPoint, secondPoint);

    AddToModeling(domElement);
}

void VModelingLine::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine));
}

void VModelingLine::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine));
}

void VModelingLine::RemoveReferens()
{
    doc->DecrementReferens(firstPoint);
    doc->DecrementReferens(secondPoint);
}
