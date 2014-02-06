/************************************************************************
 **
 **  @file   vtoolheight.cpp
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

#include "vtoolheight.h"
#include "../../dialogs/dialogheight.h"

const QString VToolHeight::ToolType = QStringLiteral("height");

VToolHeight::VToolHeight(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &typeLine,
                         const qint64 &basePointId, const qint64 &p1LineId, const qint64 &p2LineId,
                         const Tool::Sources &typeCreation, QGraphicsItem * parent)
    :VToolLinePoint(doc, data, id, typeLine, QString(), basePointId, 0, parent), p1LineId(p1LineId), p2LineId(p2LineId)
{
    ignoreFullUpdate = true;
    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

void VToolHeight::setDialog()
{
    Q_CHECK_PTR(dialog);
    DialogHeight *dialogTool = qobject_cast<DialogHeight*>(dialog);
    Q_CHECK_PTR(dialogTool);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogTool->setTypeLine(typeLine);
    dialogTool->setBasePointId(basePointId, id);
    dialogTool->setP1LineId(p1LineId, id);
    dialogTool->setP2LineId(p2LineId, id);
    dialogTool->setPointName(p->name());
}

void VToolHeight::Create(DialogTool *dialog, VMainGraphicsScene *scene, VDomDocument *doc,
                         VContainer *data)
{
    Q_CHECK_PTR(dialog);
    DialogHeight *dialogTool = qobject_cast<DialogHeight*>(dialog);
    Q_CHECK_PTR(dialogTool);
    disconnect(doc, &VDomDocument::FullUpdateFromFile, dialogTool, &DialogHeight::UpdateList);
    QString pointName = dialogTool->getPointName();
    QString typeLine = dialogTool->getTypeLine();
    qint64 basePointId = dialogTool->getBasePointId();
    qint64 p1LineId = dialogTool->getP1LineId();
    qint64 p2LineId = dialogTool->getP2LineId();
    Create(0, pointName, typeLine, basePointId, p1LineId, p2LineId, 5, 10, scene, doc, data,
           Document::FullParse, Tool::FromGui);
}

void VToolHeight::Create(const qint64 _id, const QString &pointName, const QString &typeLine,
                         const qint64 &basePointId, const qint64 &p1LineId, const qint64 &p2LineId,
                         const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VDomDocument *doc,
                         VContainer *data, const Document::Documents &parse, const Tool::Sources &typeCreation)
{
    const VPointF *basePoint = data->GeometricObject<const VPointF *>(basePointId);
    const VPointF *p1Line = data->GeometricObject<const VPointF *>(p1LineId);
    const VPointF *p2Line = data->GeometricObject<const VPointF *>(p2LineId);

    QPointF pHeight = FindPoint(QLineF(p1Line->toQPointF(), p2Line->toQPointF()), basePoint->toQPointF());
    qint64 id = _id;
    if (typeCreation == Tool::FromGui)
    {
        id = data->AddGObject(new VPointF(pHeight.x(), pHeight.y(), pointName, mx, my));
        data->AddLine(basePointId, id);
        data->AddLine(p1LineId, id);
        data->AddLine(p2LineId, id);
    }
    else
    {
        data->UpdateGObject(id, new VPointF(pHeight.x(), pHeight.y(), pointName, mx, my));
        data->AddLine(basePointId, id);
        data->AddLine(p1LineId, id);
        data->AddLine(p2LineId, id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::Height, doc);
    if (parse == Document::FullParse)
    {
        VToolHeight *point = new VToolHeight(doc, data, id, typeLine, basePointId, p1LineId, p2LineId,
                                             typeCreation);
        scene->addItem(point);
        connect(point, &VToolPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolPoint::SetFactor);
        doc->AddTool(id, point);
        doc->IncrementReferens(basePointId);
        doc->IncrementReferens(p1LineId);
        doc->IncrementReferens(p2LineId);
    }
}

QPointF VToolHeight::FindPoint(const QLineF &line, const QPointF &point)
{
    return VAbstractTool::ClosestPoint(line, point);
}

void VToolHeight::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        typeLine = domElement.attribute(AttrTypeLine, "");
        basePointId = domElement.attribute(AttrBasePoint, "").toLongLong();
        p1LineId = domElement.attribute(AttrP1Line, "").toLongLong();
        p2LineId = domElement.attribute(AttrP2Line, "").toLongLong();
    }
    RefreshGeometry();

}

void VToolHeight::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogHeight>(this, event);
}

void VToolHeight::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogHeight>(this, event);
}

void VToolHeight::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    SetAttribute(domElement, AttrId, id);
    SetAttribute(domElement, AttrType, ToolType);
    SetAttribute(domElement, AttrName, point->name());
    SetAttribute(domElement, AttrMx, toMM(point->mx()));
    SetAttribute(domElement, AttrMy, toMM(point->my()));

    SetAttribute(domElement, AttrTypeLine, typeLine);
    SetAttribute(domElement, AttrBasePoint, basePointId);
    SetAttribute(domElement, AttrP1Line, p1LineId);
    SetAttribute(domElement, AttrP2Line, p2LineId);

    AddToCalculation(domElement);

}

void VToolHeight::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        SetAttribute(domElement, AttrName, point->name());
        SetAttribute(domElement, AttrMx, toMM(point->mx()));
        SetAttribute(domElement, AttrMy, toMM(point->my()));
        SetAttribute(domElement, AttrTypeLine, typeLine);
        SetAttribute(domElement, AttrBasePoint, basePointId);
        SetAttribute(domElement, AttrP1Line, p1LineId);
        SetAttribute(domElement, AttrP2Line, p2LineId);
    }
}

void VToolHeight::SaveDialog(QDomElement &domElement)
{
    Q_CHECK_PTR(dialog);
    DialogHeight *dialogTool = qobject_cast<DialogHeight*>(dialog);
    Q_CHECK_PTR(dialogTool);
    SetAttribute(domElement, AttrName, dialogTool->getPointName());
    SetAttribute(domElement, AttrTypeLine, dialogTool->getTypeLine());
    SetAttribute(domElement, AttrBasePoint, QString().setNum(dialogTool->getBasePointId()));
    SetAttribute(domElement, AttrP1Line, QString().setNum(dialogTool->getP1LineId()));
    SetAttribute(domElement, AttrP2Line, QString().setNum(dialogTool->getP2LineId()));
}
