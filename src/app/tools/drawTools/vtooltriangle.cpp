/************************************************************************
 **
 **  @file   vtooltriangle.cpp
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

#include "vtooltriangle.h"
#include "../../dialogs/tools/dialogtriangle.h"
#include "../../geometry/vpointf.h"

#include <QtMath>

const QString VToolTriangle::ToolType = QStringLiteral("triangle");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolTriangle constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param axisP1Id id first axis point.
 * @param axisP2Id id second axis point.
 * @param firstPointId id first triangle point, what lies on the hypotenuse.
 * @param secondPointId id second triangle point, what lies on the hypotenuse.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolTriangle::VToolTriangle(VPattern *doc, VContainer *data, const quint32 &id, const quint32 &axisP1Id,
                             const quint32 &axisP2Id, const quint32 &firstPointId, const quint32 &secondPointId,
                             const Source &typeCreation, QGraphicsItem *parent)
    :VToolPoint(doc, data, id, parent), axisP1Id(axisP1Id), axisP2Id(axisP2Id), firstPointId(firstPointId),
      secondPointId(secondPointId)
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
void VToolTriangle::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogTriangle *dialogTool = qobject_cast<DialogTriangle*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->setAxisP1Id(axisP1Id);
    dialogTool->setAxisP2Id(axisP2Id);
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
VToolTriangle* VToolTriangle::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogTriangle *dialogTool = qobject_cast<DialogTriangle*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const quint32 axisP1Id = dialogTool->getAxisP1Id();
    const quint32 axisP2Id = dialogTool->getAxisP2Id();
    const quint32 firstPointId = dialogTool->getFirstPointId();
    const quint32 secondPointId = dialogTool->getSecondPointId();
    const QString pointName = dialogTool->getPointName();
    VToolTriangle* point = nullptr;
    point = Create(0, pointName, axisP1Id, axisP2Id, firstPointId, secondPointId, 5, 10, scene, doc, data,
                   Document::FullParse, Source::FromGui);
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
 * @param axisP1Id id first axis point.
 * @param axisP2Id id second axis point.
 * @param firstPointId id first triangle point, what lies on the hypotenuse.
 * @param secondPointId id second triangle point, what lies on the hypotenuse.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 * @return the created tool
 */
VToolTriangle* VToolTriangle::Create(const quint32 _id, const QString &pointName, const quint32 &axisP1Id,
                                     const quint32 &axisP2Id, const quint32 &firstPointId, const quint32 &secondPointId,
                                     const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VPattern *doc,
                                     VContainer *data, const Document &parse, const Source &typeCreation)
{
    const QSharedPointer<VPointF> axisP1 = data->GeometricObject<VPointF>(axisP1Id);
    const QSharedPointer<VPointF> axisP2 = data->GeometricObject<VPointF>(axisP2Id);
    const QSharedPointer<VPointF> firstPoint = data->GeometricObject<VPointF>(firstPointId);
    const QSharedPointer<VPointF> secondPoint = data->GeometricObject<VPointF>(secondPointId);

    QPointF point = FindPoint(axisP1->toQPointF(), axisP2->toQPointF(), firstPoint->toQPointF(),
                              secondPoint->toQPointF());
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
    VDrawTool::AddRecord(id, Tool::Triangle, doc);
    if (parse == Document::FullParse)
    {
        VToolTriangle *point = new VToolTriangle(doc, data, id, axisP1Id, axisP2Id, firstPointId,
                                                 secondPointId, typeCreation);
        scene->addItem(point);
        connect(point, &VToolTriangle::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolTriangle::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, point, &VToolPoint::Disable);
        doc->AddTool(id, point);
        doc->IncrementReferens(axisP1Id);
        doc->IncrementReferens(axisP2Id);
        doc->IncrementReferens(firstPointId);
        doc->IncrementReferens(secondPointId);
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FindPoint find point intersection two foots right triangle.
 * @param axisP1 first axis point.
 * @param axisP2 second axis point.
 * @param firstPoint first triangle point, what lies on the hypotenuse.
 * @param secondPoint second triangle point, what lies on the hypotenuse.
 * @return point intersection two foots right triangle.
 */
QPointF VToolTriangle::FindPoint(const QPointF &axisP1, const QPointF &axisP2, const QPointF &firstPoint,
                                 const QPointF &secondPoint)
{
    QLineF axis(axisP1, axisP2);
    QLineF hypotenuse(firstPoint, secondPoint);

    QPointF startPoint;
    QLineF::IntersectType intersect = axis.intersect(hypotenuse, &startPoint);
    if (intersect != QLineF::UnboundedIntersection && intersect != QLineF::BoundedIntersection)
    {
        return QPointF();
    }

    qreal step = 1;

    QLineF line;
    line.setP1(startPoint);
    line.setAngle(axis.angle());
    line.setLength(step);

    int c = qFloor(hypotenuse.length());
    while (1)
    {
        line.setLength(line.length()+step);
        int a = qFloor(QLineF(line.p2(), firstPoint).length());
        int b = qFloor(QLineF(line.p2(), secondPoint).length());
        if (c*c <= (a*a + b*b))
        {
            return line.p2();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolTriangle::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        axisP1Id = domElement.attribute(AttrAxisP1, "").toUInt();
        axisP2Id = domElement.attribute(AttrAxisP2, "").toUInt();
        firstPointId = domElement.attribute(AttrFirstPoint, "").toUInt();
        secondPointId = domElement.attribute(AttrSecondPoint, "").toUInt();
    }
    VToolPoint::RefreshPointGeometry(*VDrawTool::data.GeometricObject<VPointF>(id));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowContextMenu show context menu.
 * @param event context menu event.
 */
void VToolTriangle::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogTriangle>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolTriangle::RemoveReferens()
{
    doc->DecrementReferens(axisP1Id);
    doc->DecrementReferens(axisP2Id);
    doc->DecrementReferens(firstPointId);
    doc->DecrementReferens(secondPointId);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolTriangle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogTriangle>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolTriangle::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogTriangle *dialogTool = qobject_cast<DialogTriangle*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrAxisP1, QString().setNum(dialogTool->getAxisP1Id()));
    doc->SetAttribute(domElement, AttrAxisP2, QString().setNum(dialogTool->getAxisP2Id()));
    doc->SetAttribute(domElement, AttrFirstPoint, QString().setNum(dialogTool->getFirstPointId()));
    doc->SetAttribute(domElement, AttrSecondPoint, QString().setNum(dialogTool->getSecondPointId()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTriangle::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    QSharedPointer<VPointF> point = qSharedPointerDynamicCast<VPointF>(obj);
    SCASSERT(point.isNull() == false);

    doc->SetAttribute(tag, VDomDocument::AttrId, id);
    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrName, point->name());
    doc->SetAttribute(tag, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(tag, AttrMy, qApp->fromPixel(point->my()));

    doc->SetAttribute(tag, AttrAxisP1, axisP1Id);
    doc->SetAttribute(tag, AttrAxisP2, axisP2Id);
    doc->SetAttribute(tag, AttrFirstPoint, firstPointId);
    doc->SetAttribute(tag, AttrSecondPoint, secondPointId);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolTriangle::getSecondPointId() const
{
    return secondPointId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTriangle::setSecondPointId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        secondPointId = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolTriangle::getFirstPointId() const
{
    return firstPointId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTriangle::setFirstPointId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        firstPointId = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolTriangle::getAxisP2Id() const
{
    return axisP2Id;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTriangle::setAxisP2Id(const quint32 &value)
{
    if (value != NULL_ID)
    {
        axisP2Id = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolTriangle::getAxisP1Id() const
{
    return axisP1Id;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTriangle::setAxisP1Id(const quint32 &value)
{
    if (value != NULL_ID)
    {
        axisP1Id = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}
