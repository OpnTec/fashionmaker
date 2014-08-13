/************************************************************************
 **
 **  @file   vtoolpointofintersection.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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
#include "../../geometry/vpointf.h"

const QString VToolPointOfIntersection::ToolType = QStringLiteral("pointOfIntersection");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolPointOfIntersection constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param firstPointId id first line point.
 * @param secondPointId id second line point.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolPointOfIntersection::VToolPointOfIntersection(VPattern *doc, VContainer *data, const quint32 &id,
                                                   const quint32 &firstPointId, const quint32 &secondPointId,
                                                   const Source &typeCreation, QGraphicsItem *parent)
    :VToolPoint(doc, data, id, parent), firstPointId(firstPointId), secondPointId(secondPointId)
{
    ignoreFullUpdate = true;
    if (typeCreation == Source::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolPointOfIntersection::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogPointOfIntersection *dialogTool = qobject_cast<DialogPointOfIntersection*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogTool->setFirstPointId(firstPointId);
    dialogTool->setSecondPointId(secondPointId);
    dialogTool->setPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool from GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @return the created tool
 */
VToolPointOfIntersection *VToolPointOfIntersection::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc,
                                                           VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogPointOfIntersection *dialogTool = qobject_cast<DialogPointOfIntersection*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const quint32 firstPointId = dialogTool->getFirstPointId();
    const quint32 secondPointId = dialogTool->getSecondPointId();
    const QString pointName = dialogTool->getPointName();
    VToolPointOfIntersection *point = nullptr;
    point = Create(0, pointName, firstPointId, secondPointId, 5, 10, scene, doc, data, Document::FullParse,
                   Source::FromGui);
    if (point != nullptr)
    {
        point->dialog=dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param _id tool id, 0 if tool doesn't exist yet.
 * @param pointName point name.
 * @param firstPointId id first line point.
 * @param secondPointId id second line point.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 * @return the created tool
 */
VToolPointOfIntersection *VToolPointOfIntersection::Create(const quint32 _id, const QString &pointName,
                                                           const quint32 &firstPointId, const quint32 &secondPointId,
                                                           const qreal &mx, const qreal &my, VMainGraphicsScene *scene,
                                                           VPattern *doc, VContainer *data, const Document &parse,
                                                           const Source &typeCreation)
{
    const VPointF *firstPoint = data->GeometricObject<const VPointF *>(firstPointId);
    const VPointF *secondPoint = data->GeometricObject<const VPointF *>(secondPointId);

    QPointF point(firstPoint->x(), secondPoint->y());
    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(new VPointF(point, pointName, mx, my));
    }
    else
    {
        data->UpdateGObject(id, new VPointF(point, pointName, mx, my));
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
        connect(scene, &VMainGraphicsScene::DisableItem, point, &VToolPoint::Disable);
        doc->AddTool(id, point);
        doc->IncrementReferens(firstPointId);
        doc->IncrementReferens(secondPointId);
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolPointOfIntersection::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        firstPointId = domElement.attribute(AttrFirstPoint, "").toUInt();
        secondPointId = domElement.attribute(AttrSecondPoint, "").toUInt();
    }
    VToolPoint::RefreshPointGeometry(*VDrawTool::data.GeometricObject<const VPointF *>(id));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowContextMenu show context menu.
 * @param event context menu event.
 */
void VToolPointOfIntersection::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogPointOfIntersection>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolPointOfIntersection::RemoveReferens()
{
    doc->DecrementReferens(firstPointId);
    doc->DecrementReferens(secondPointId);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolPointOfIntersection::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogPointOfIntersection>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VToolPointOfIntersection::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrName, point->name());
    doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));

    doc->SetAttribute(domElement, AttrFirstPoint, firstPointId);
    doc->SetAttribute(domElement, AttrSecondPoint, secondPointId);

    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
 */
void VToolPointOfIntersection::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrName, point->name());
        doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
        doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));
        doc->SetAttribute(domElement, AttrFirstPoint, firstPointId);
        doc->SetAttribute(domElement, AttrSecondPoint, secondPointId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolPointOfIntersection::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogPointOfIntersection *dialogTool = qobject_cast<DialogPointOfIntersection*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrFirstPoint, QString().setNum(dialogTool->getFirstPointId()));
    doc->SetAttribute(domElement, AttrSecondPoint, QString().setNum(dialogTool->getSecondPointId()));
}
