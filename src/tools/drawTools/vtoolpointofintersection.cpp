/************************************************************************
 **
 **  @file   vtoolpointofintersection.cpp
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

#include "vtoolpointofintersection.h"
#include "../../dialogs/tools/dialogpointofintersection.h"

const QString VToolPointOfIntersection::ToolType = QStringLiteral("pointOfIntersection");

VToolPointOfIntersection::VToolPointOfIntersection(VDomDocument *doc, VContainer *data, const qint64 &id,
                                                   const qint64 &firstPointId, const qint64 &secondPointId,
                                                   const Tool::Sources &typeCreation, QGraphicsItem *parent)
    :VToolPoint(doc, data, id, parent), firstPointId(firstPointId), secondPointId(secondPointId)
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

void VToolPointOfIntersection::setDialog()
{
    Q_CHECK_PTR(dialog);
    DialogPointOfIntersection *dialogTool = qobject_cast<DialogPointOfIntersection*>(dialog);
    Q_CHECK_PTR(dialogTool);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogTool->setFirstPointId(firstPointId, id);
    dialogTool->setSecondPointId(secondPointId, id);
    dialogTool->setPointName(p->name());
}

void VToolPointOfIntersection::Create(DialogTool *dialog, VMainGraphicsScene *scene, VDomDocument *doc,
                                      VContainer *data)
{
    Q_CHECK_PTR(dialog);
    DialogPointOfIntersection *dialogTool = qobject_cast<DialogPointOfIntersection*>(dialog);
    Q_CHECK_PTR(dialogTool);
    qint64 firstPointId = dialogTool->getFirstPointId();
    qint64 secondPointId = dialogTool->getSecondPointId();
    QString pointName = dialogTool->getPointName();
    Create(0, pointName, firstPointId, secondPointId, 5, 10, scene, doc, data, Document::FullParse, Tool::FromGui);
}

void VToolPointOfIntersection::Create(const qint64 _id, const QString &pointName, const qint64 &firstPointId,
                                      const qint64 &secondPointId, const qreal &mx, const qreal &my,
                                      VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data,
                                      const Document::Documents &parse, const Tool::Sources &typeCreation)
{
    const VPointF *firstPoint = data->GeometricObject<const VPointF *>(firstPointId);
    const VPointF *secondPoint = data->GeometricObject<const VPointF *>(secondPointId);

    QPointF point(firstPoint->x(), secondPoint->y());
    qint64 id = _id;
    if (typeCreation == Tool::FromGui)
    {
        id = data->AddGObject(new VPointF(point.x(), point.y(), pointName, mx, my));
    }
    else
    {
        data->UpdateGObject(id, new VPointF(point.x(), point.y(), pointName, mx, my));
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::PointOfIntersection, doc);
    if (parse == Document::FullParse)
    {
        VToolPointOfIntersection *point = new VToolPointOfIntersection(doc, data, id, firstPointId,
                                                                       secondPointId, typeCreation);
        scene->addItem(point);
        connect(point, &VToolPointOfIntersection::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolPointOfIntersection::SetFactor);
        doc->AddTool(id, point);
        doc->IncrementReferens(firstPointId);
        doc->IncrementReferens(secondPointId);
    }
}

void VToolPointOfIntersection::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        firstPointId = domElement.attribute(AttrFirstPoint, "").toLongLong();
        secondPointId = domElement.attribute(AttrSecondPoint, "").toLongLong();
    }
    VToolPoint::RefreshPointGeometry(*VDrawTool::data.GeometricObject<const VPointF *>(id));
}

void VToolPointOfIntersection::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogPointOfIntersection>(this, event);
}

void VToolPointOfIntersection::RemoveReferens()
{
    doc->DecrementReferens(firstPointId);
    doc->DecrementReferens(secondPointId);
}

void VToolPointOfIntersection::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogPointOfIntersection>(this, event);
}

void VToolPointOfIntersection::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    SetAttribute(domElement, AttrId, id);
    SetAttribute(domElement, AttrType, ToolType);
    SetAttribute(domElement, AttrName, point->name());
    SetAttribute(domElement, AttrMx, toMM(point->mx()));
    SetAttribute(domElement, AttrMy, toMM(point->my()));

    SetAttribute(domElement, AttrFirstPoint, firstPointId);
    SetAttribute(domElement, AttrSecondPoint, secondPointId);

    AddToCalculation(domElement);
}

void VToolPointOfIntersection::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        SetAttribute(domElement, AttrName, point->name());
        SetAttribute(domElement, AttrMx, toMM(point->mx()));
        SetAttribute(domElement, AttrMy, toMM(point->my()));
        SetAttribute(domElement, AttrFirstPoint, firstPointId);
        SetAttribute(domElement, AttrSecondPoint, secondPointId);
    }
}

void VToolPointOfIntersection::SaveDialog(QDomElement &domElement)
{
    Q_CHECK_PTR(dialog);
    DialogPointOfIntersection *dialogTool = qobject_cast<DialogPointOfIntersection*>(dialog);
    Q_CHECK_PTR(dialogTool);
    SetAttribute(domElement, AttrName, dialogTool->getPointName());
    SetAttribute(domElement, AttrFirstPoint, QString().setNum(dialogTool->getFirstPointId()));
    SetAttribute(domElement, AttrSecondPoint, QString().setNum(dialogTool->getSecondPointId()));
}
