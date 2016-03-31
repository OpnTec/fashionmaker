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
 **  Copyright (C) 2013-2015 Valentina project
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
#include "../../../../dialogs/tools/dialogtriangle.h"
#include "../vgeometry/vpointf.h"
#include "../../../../visualization/line/vistooltriangle.h"
#include <QtCore/qmath.h>

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
VToolTriangle::VToolTriangle(VAbstractPattern *doc, VContainer *data, const quint32 &id, const quint32 &axisP1Id,
                             const quint32 &axisP2Id, const quint32 &firstPointId, const quint32 &secondPointId,
                             const Source &typeCreation, QGraphicsItem *parent)
    :VToolSinglePoint(doc, data, id, parent), axisP1Id(axisP1Id), axisP2Id(axisP2Id), firstPointId(firstPointId),
      secondPointId(secondPointId)
{
    ToolCreation(typeCreation);
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
    dialogTool->SetAxisP1Id(axisP1Id);
    dialogTool->SetAxisP2Id(axisP2Id);
    dialogTool->SetFirstPointId(firstPointId);
    dialogTool->SetSecondPointId(secondPointId);
    dialogTool->SetPointName(p->name());
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
VToolTriangle* VToolTriangle::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                     VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogTriangle *dialogTool = qobject_cast<DialogTriangle*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const quint32 axisP1Id = dialogTool->GetAxisP1Id();
    const quint32 axisP2Id = dialogTool->GetAxisP2Id();
    const quint32 firstPointId = dialogTool->GetFirstPointId();
    const quint32 secondPointId = dialogTool->GetSecondPointId();
    const QString pointName = dialogTool->getPointName();
    VToolTriangle* point = Create(0, pointName, axisP1Id, axisP2Id, firstPointId, secondPointId, 5, 10,
                                  scene, doc, data, Document::FullParse, Source::FromGui);
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
                                     const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VAbstractPattern *doc,
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
        InitToolConnections(scene, point);
        doc->AddTool(id, point);
        doc->IncrementReferens(axisP1->getIdTool());
        doc->IncrementReferens(axisP2->getIdTool());
        doc->IncrementReferens(firstPoint->getIdTool());
        doc->IncrementReferens(secondPoint->getIdTool());
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

    qint64 c = qFloor(hypotenuse.length());
    while (1)
    {
        line.setLength(line.length()+step);
        qint64 a = qFloor(QLineF(line.p2(), firstPoint).length());
        qint64 b = qFloor(QLineF(line.p2(), secondPoint).length());
        if (c*c <= (a*a + b*b))
        {
            return line.p2();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolTriangle::RemoveReferens()
{
    const auto axisP1 = VAbstractTool::data.GetGObject(axisP1Id);
    const auto axisP2 = VAbstractTool::data.GetGObject(axisP2Id);
    const auto firstPoint = VAbstractTool::data.GetGObject(firstPointId);
    const auto secondPoint = VAbstractTool::data.GetGObject(secondPointId);

    doc->DecrementReferens(axisP1->getIdTool());
    doc->DecrementReferens(axisP2->getIdTool());
    doc->DecrementReferens(firstPoint->getIdTool());
    doc->DecrementReferens(secondPoint->getIdTool());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolTriangle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    try
    {
        ContextMenu<DialogTriangle>(this, event);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e);
        return;//Leave this method immediately!!!
    }
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
    doc->SetAttribute(domElement, AttrAxisP1, QString().setNum(dialogTool->GetAxisP1Id()));
    doc->SetAttribute(domElement, AttrAxisP2, QString().setNum(dialogTool->GetAxisP2Id()));
    doc->SetAttribute(domElement, AttrFirstPoint, QString().setNum(dialogTool->GetFirstPointId()));
    doc->SetAttribute(domElement, AttrSecondPoint, QString().setNum(dialogTool->GetSecondPointId()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTriangle::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolSinglePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrAxisP1, axisP1Id);
    doc->SetAttribute(tag, AttrAxisP2, axisP2Id);
    doc->SetAttribute(tag, AttrFirstPoint, firstPointId);
    doc->SetAttribute(tag, AttrSecondPoint, secondPointId);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTriangle::ReadToolAttributes(const QDomElement &domElement)
{
    axisP1Id = doc->GetParametrUInt(domElement, AttrAxisP1, NULL_ID_STR);
    axisP2Id = doc->GetParametrUInt(domElement, AttrAxisP2, NULL_ID_STR);
    firstPointId = doc->GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
    secondPointId = doc->GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTriangle::SetVisualization()
{
    if (vis != nullptr)
    {
        VisToolTriangle * visual = qobject_cast<VisToolTriangle *>(vis);
        SCASSERT(visual != nullptr);

        visual->setObject1Id(axisP1Id);
        visual->setObject2Id(axisP2Id);
        visual->setHypotenuseP1Id(firstPointId);
        visual->setHypotenuseP2Id(secondPointId);
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolTriangle::GetSecondPointId() const
{
    return secondPointId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTriangle::SetSecondPointId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        secondPointId = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTriangle::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolTriangle>(show);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolTriangle::GetFirstPointId() const
{
    return firstPointId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTriangle::SetFirstPointId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        firstPointId = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolTriangle::GetAxisP2Id() const
{
    return axisP2Id;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTriangle::SetAxisP2Id(const quint32 &value)
{
    if (value != NULL_ID)
    {
        axisP2Id = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolTriangle::GetAxisP1Id() const
{
    return axisP1Id;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTriangle::SetAxisP1Id(const quint32 &value)
{
    if (value != NULL_ID)
    {
        axisP1Id = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}
