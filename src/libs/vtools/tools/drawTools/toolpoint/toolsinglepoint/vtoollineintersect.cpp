/************************************************************************
 **
 **  @file   vtoollineintersect.cpp
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

#include "vtoollineintersect.h"
#include "../../../../dialogs/tools/dialoglineintersect.h"
#include "../vgeometry/vpointf.h"
#include "../../../../visualization/vistoollineintersect.h"

const QString VToolLineIntersect::ToolType = QStringLiteral("lineIntersect");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolLineIntersect constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param p1Line1 id first point first line.
 * @param p2Line1 id second point first line.
 * @param p1Line2 id first point second line.
 * @param p2Line2 id second point second line.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolLineIntersect::VToolLineIntersect(VAbstractPattern *doc, VContainer *data, const quint32 &id,
                                       const quint32 &p1Line1, const quint32 &p2Line1, const quint32 &p1Line2,
                                       const quint32 &p2Line2, const Source &typeCreation,
                                       QGraphicsItem *parent)
    :VToolSinglePoint(doc, data, id, parent), p1Line1(p1Line1), p2Line1(p2Line1), p1Line2(p1Line2),
    p2Line2(p2Line2)
{
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolLineIntersect::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogLineIntersect *dialogTool = qobject_cast<DialogLineIntersect*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->SetP1Line1(p1Line1);
    dialogTool->SetP2Line1(p2Line1);
    dialogTool->SetP1Line2(p1Line2);
    dialogTool->SetP2Line2(p2Line2);
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
VToolLineIntersect* VToolLineIntersect::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                               VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogLineIntersect *dialogTool = qobject_cast<DialogLineIntersect*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const quint32 p1Line1Id = dialogTool->GetP1Line1();
    const quint32 p2Line1Id = dialogTool->GetP2Line1();
    const quint32 p1Line2Id = dialogTool->GetP1Line2();
    const quint32 p2Line2Id = dialogTool->GetP2Line2();
    const QString pointName = dialogTool->getPointName();
    VToolLineIntersect* point = nullptr;
    point = Create(0, p1Line1Id, p2Line1Id, p1Line2Id, p2Line2Id, pointName, 5, 10, scene, doc, data,
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
 * @param p1Line1Id id first point first line.
 * @param p2Line1Id id second point first line.
 * @param p1Line2Id id first point second line.
 * @param p2Line2Id id second point second line.
 * @param pointName point name.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 * @return the created tool
 */
VToolLineIntersect* VToolLineIntersect::Create(const quint32 _id, const quint32 &p1Line1Id, const quint32 &p2Line1Id,
                                               const quint32 &p1Line2Id, const quint32 &p2Line2Id,
                                               const QString &pointName, const qreal &mx, const qreal &my,
                                               VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data,
                                               const Document &parse, const Source &typeCreation)
{
    const QSharedPointer<VPointF> p1Line1 = data->GeometricObject<VPointF>(p1Line1Id);
    const QSharedPointer<VPointF> p2Line1 = data->GeometricObject<VPointF>(p2Line1Id);
    const QSharedPointer<VPointF> p1Line2 = data->GeometricObject<VPointF>(p1Line2Id);
    const QSharedPointer<VPointF> p2Line2 = data->GeometricObject<VPointF>(p2Line2Id);

    QLineF line1(p1Line1->toQPointF(), p2Line1->toQPointF());
    QLineF line2(p1Line2->toQPointF(), p2Line2->toQPointF());
    QPointF fPoint;
    QLineF::IntersectType intersect = line1.intersect(line2, &fPoint);
    if (intersect == QLineF::UnboundedIntersection || intersect == QLineF::BoundedIntersection)
    {
        quint32 id = _id;
        if (typeCreation == Source::FromGui)
        {
            id = data->AddGObject(new VPointF(fPoint, pointName, mx, my));
            data->AddLine(p1Line1Id, id);
            data->AddLine(id, p2Line1Id);
            data->AddLine(p1Line2Id, id);
            data->AddLine(id, p2Line2Id);
        }
        else
        {
            data->UpdateGObject(id, new VPointF(fPoint, pointName, mx, my));
            data->AddLine(p1Line1Id, id);
            data->AddLine(id, p2Line1Id);
            data->AddLine(p1Line2Id, id);
            data->AddLine(id, p2Line2Id);
            if (parse != Document::FullParse)
            {
                doc->UpdateToolData(id, data);
            }
        }
        VDrawTool::AddRecord(id, Tool::LineIntersect, doc);
        if (parse == Document::FullParse)
        {
            VToolLineIntersect *point = new VToolLineIntersect(doc, data, id, p1Line1Id, p2Line1Id, p1Line2Id,
                                                               p2Line2Id, typeCreation);
            scene->addItem(point);
            connect(point, &VToolLineIntersect::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolLineIntersect::SetFactor);
            connect(scene, &VMainGraphicsScene::DisableItem, point, &VToolLineIntersect::Disable);
            connect(scene, &VMainGraphicsScene::EnableToolMove, point, &VToolLineIntersect::EnableToolMove);
            doc->AddTool(id, point);
            doc->IncrementReferens(p1Line1Id);
            doc->IncrementReferens(p2Line1Id);
            doc->IncrementReferens(p1Line2Id);
            doc->IncrementReferens(p2Line2Id);
            return point;
        }
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFactor set current scale factor of scene.
 * @param factor scene scale factor.
 */
void VToolLineIntersect::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<VPointF>(id));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolLineIntersect::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogLineIntersect>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolLineIntersect::RemoveReferens()
{
    doc->DecrementReferens(p1Line1);
    doc->DecrementReferens(p2Line1);
    doc->DecrementReferens(p1Line2);
    doc->DecrementReferens(p2Line2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolLineIntersect::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogLineIntersect *dialogTool = qobject_cast<DialogLineIntersect*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrP1Line1, QString().setNum(dialogTool->GetP1Line1()));
    doc->SetAttribute(domElement, AttrP2Line1, QString().setNum(dialogTool->GetP2Line1()));
    doc->SetAttribute(domElement, AttrP1Line2, QString().setNum(dialogTool->GetP1Line2()));
    doc->SetAttribute(domElement, AttrP2Line2, QString().setNum(dialogTool->GetP2Line2()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersect::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolSinglePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrP1Line1, p1Line1);
    doc->SetAttribute(tag, AttrP2Line1, p2Line1);
    doc->SetAttribute(tag, AttrP1Line2, p1Line2);
    doc->SetAttribute(tag, AttrP2Line2, p2Line2);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersect::ReadToolAttributes(const QDomElement &domElement)
{
    p1Line1 = doc->GetParametrUInt(domElement, AttrP1Line1, NULL_ID_STR);
    p2Line1 = doc->GetParametrUInt(domElement, AttrP2Line1, NULL_ID_STR);
    p1Line2 = doc->GetParametrUInt(domElement, AttrP1Line2, NULL_ID_STR);
    p2Line2 = doc->GetParametrUInt(domElement, AttrP2Line2, NULL_ID_STR);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersect::SetVisualization()
{
    if (vis != nullptr)
    {
        VisToolLineIntersect *visual = qobject_cast<VisToolLineIntersect *>(vis);
        SCASSERT(visual != nullptr);

        visual->setPoint1Id(p1Line1);
        visual->setLine1P2Id(p2Line1);
        visual->setLine2P1Id(p1Line2);
        visual->setLine2P2Id(p2Line2);
        vis->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolLineIntersect::GetP2Line2() const
{
    return p2Line2;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersect::SetP2Line2(const quint32 &value)
{
    if (value != NULL_ID)
    {
        p2Line2 = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersect::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolLineIntersect>(show);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolLineIntersect::GetP1Line2() const
{
    return p1Line2;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersect::SetP1Line2(const quint32 &value)
{
    if (value != NULL_ID)
    {
        p1Line2 = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolLineIntersect::GetP2Line1() const
{
    return p2Line1;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersect::SetP2Line1(const quint32 &value)
{
    if (value != NULL_ID)
    {
        p2Line1 = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolLineIntersect::GetP1Line1() const
{
    return p1Line1;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLineIntersect::SetP1Line1(const quint32 &value)
{
    if (value != NULL_ID)
    {
        p1Line1 = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}
