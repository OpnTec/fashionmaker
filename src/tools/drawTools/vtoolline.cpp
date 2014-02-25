/************************************************************************
 **
 **  @file   vtoolline.cpp
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

#include "vtoolline.h"
#include "../../dialogs/tools/dialogline.h"

const QString VToolLine::TagName = QStringLiteral("line");

VToolLine::VToolLine(VPattern *doc, VContainer *data, qint64 id, qint64 firstPoint, qint64 secondPoint,
                     const QString &typeLine, const Tool::Sources &typeCreation, QGraphicsItem *parent)
    :VDrawTool(doc, data, id), QGraphicsLineItem(parent), firstPoint(firstPoint), secondPoint(secondPoint)
{
    this->typeLine = typeLine;
    ignoreFullUpdate = true;
    //Line
    const VPointF *first = data->GeometricObject<const VPointF *>(firstPoint);
    const VPointF *second = data->GeometricObject<const VPointF *>(secondPoint);
    this->setLine(QLineF(first->toQPointF(), second->toQPointF()));
    this->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setAcceptHoverEvents(true);
    this->setPen(QPen(Qt::black, widthHairLine/factor, LineStyle()));

    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

void VToolLine::setDialog()
{
    Q_CHECK_PTR(dialog);
    DialogLine *dialogTool = qobject_cast<DialogLine*>(dialog);
    Q_CHECK_PTR(dialogTool);
    dialogTool->setFirstPoint(firstPoint);
    dialogTool->setSecondPoint(secondPoint);
    dialogTool->setTypeLine(typeLine);
}

void VToolLine::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data)
{
    Q_CHECK_PTR(dialog);
    DialogLine *dialogTool = qobject_cast<DialogLine*>(dialog);
    Q_CHECK_PTR(dialogTool);
    qint64 firstPoint = dialogTool->getFirstPoint();
    qint64 secondPoint = dialogTool->getSecondPoint();
    QString typeLine = dialogTool->getTypeLine();
    Create(0, firstPoint, secondPoint, typeLine, scene, doc, data, Document::FullParse, Tool::FromGui);
}

void VToolLine::Create(const qint64 &_id, const qint64 &firstPoint, const qint64 &secondPoint, const QString &typeLine,
                       VMainGraphicsScene *scene, VPattern *doc, VContainer *data,
                       const Document::Documents &parse, const Tool::Sources &typeCreation)
{
    Q_CHECK_PTR(scene);
    Q_CHECK_PTR(doc);
    Q_CHECK_PTR(data);
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
        VToolLine *line = new VToolLine(doc, data, id, firstPoint, secondPoint, typeLine, typeCreation);
        Q_CHECK_PTR(line);
        scene->addItem(line);
        connect(line, &VToolLine::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
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

void VToolLine::ShowTool(qint64 id, Qt::GlobalColor color, bool enable)
{
    ShowItem(this, id, color, enable);
}

void VToolLine::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

void VToolLine::ChangedActivDraw(const QString &newName)
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
    this->setPen(QPen(currentColor, widthHairLine/factor, LineStyle()));
    this->setAcceptHoverEvents (selectable);
    VDrawTool::ChangedActivDraw(newName);
}

void VToolLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogLine>(this, event);
}

void VToolLine::AddToFile()
{
    QDomElement domElement = doc->createElement(TagName);
    doc->SetAttribute(domElement, AttrId, id);
    doc->SetAttribute(domElement, AttrFirstPoint, firstPoint);
    doc->SetAttribute(domElement, AttrSecondPoint, secondPoint);
    doc->SetAttribute(domElement, AttrTypeLine, typeLine);

    AddToCalculation(domElement);
}

void VToolLine::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrFirstPoint, firstPoint);
        doc->SetAttribute(domElement, AttrSecondPoint, secondPoint);
        doc->SetAttribute(domElement, AttrTypeLine, typeLine);
    }
}

void VToolLine::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine/factor, LineStyle()));
}

void VToolLine::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine/factor, LineStyle()));
}

void VToolLine::RemoveReferens()
{
    doc->DecrementReferens(firstPoint);
    doc->DecrementReferens(secondPoint);
}

QVariant VToolLine::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
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

void VToolLine::keyReleaseEvent(QKeyEvent *event)
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

void VToolLine::SaveDialog(QDomElement &domElement)
{
    Q_CHECK_PTR(dialog);
    DialogLine *dialogTool = qobject_cast<DialogLine*>(dialog);
    Q_CHECK_PTR(dialogTool);
    doc->SetAttribute(domElement, AttrFirstPoint, QString().setNum(dialogTool->getFirstPoint()));
    doc->SetAttribute(domElement, AttrSecondPoint, QString().setNum(dialogTool->getSecondPoint()));
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->getTypeLine());
}

void VToolLine::RefreshGeometry()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        firstPoint = doc->GetParametrLongLong(domElement, VAbstractTool::AttrFirstPoint, "0");
        secondPoint = doc->GetParametrLongLong(domElement, VAbstractTool::AttrSecondPoint, "0");
        typeLine = doc->GetParametrString(domElement, VAbstractTool::AttrTypeLine, VAbstractTool::TypeLineLine);
    }
    const VPointF *first = VAbstractTool::data.GeometricObject<const VPointF *>(firstPoint);
    const VPointF *second = VAbstractTool::data.GeometricObject<const VPointF *>(secondPoint);
    this->setLine(QLineF(first->toQPointF(), second->toQPointF()));
    this->setPen(QPen(currentColor, widthHairLine/factor, LineStyle()));
}
