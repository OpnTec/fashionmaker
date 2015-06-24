/************************************************************************
 **
 **  @file   vtoolpointfromarcandtangent.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   6 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "vtoolpointfromarcandtangent.h"
#include "../../../../dialogs/tools/dialogpointfromarcandtangent.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/varc.h"
#include "../../../../visualization/vistoolpointfromarcandtangent.h"

const QString VToolPointFromArcAndTangent::ToolType = QStringLiteral("pointFromArcAndTangent");

//---------------------------------------------------------------------------------------------------------------------
VToolPointFromArcAndTangent::VToolPointFromArcAndTangent(VAbstractPattern *doc, VContainer *data, const quint32 &id,
                                                         quint32 arcId, quint32 tangentPointId,
                                                         CrossCirclesPoint crossPoint, const Source &typeCreation,
                                                         QGraphicsItem *parent)
    :VToolSinglePoint(doc, data, id, parent), arcId(arcId), tangentPointId(tangentPointId), crossPoint(crossPoint)
{
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromArcAndTangent::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogPointFromArcAndTangent *dialogTool = qobject_cast<DialogPointFromArcAndTangent*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->SetArcId(arcId);
    dialogTool->SetCrossCirclesPoint(crossPoint);
    dialogTool->SetTangentPointId(tangentPointId);
    dialogTool->SetPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
VToolPointFromArcAndTangent *VToolPointFromArcAndTangent::Create(DialogTool *dialog, VMainGraphicsScene *scene,
                                                                 VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogPointFromArcAndTangent *dialogTool = qobject_cast<DialogPointFromArcAndTangent*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const quint32 arcId = dialogTool->GetArcId();
    const quint32 tangentPointId = dialogTool->GetTangentPointId();
    const CrossCirclesPoint pType = dialogTool->GetCrossCirclesPoint();
    const QString pointName = dialogTool->getPointName();
    VToolPointFromArcAndTangent *point = nullptr;
    point = Create(0, pointName, arcId, tangentPointId, pType, 5, 10, scene, doc, data, Document::FullParse,
                   Source::FromGui);
    if (point != nullptr)
    {
        point->dialog=dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
VToolPointFromArcAndTangent *VToolPointFromArcAndTangent::Create(const quint32 _id, const QString &pointName,
                                                                 quint32 arcId, quint32 tangentPointId,
                                                                 CrossCirclesPoint crossPoint, const qreal &mx,
                                                                 const qreal &my, VMainGraphicsScene *scene,
                                                                 VAbstractPattern *doc, VContainer *data,
                                                                 const Document &parse, const Source &typeCreation)
{
    const VArc arc = *data->GeometricObject<VArc>(arcId);
    const VPointF tPoint = *data->GeometricObject<VPointF>(tangentPointId);

    const QPointF point = VToolPointFromArcAndTangent::FindPoint(tPoint.toQPointF(), &arc, crossPoint);
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
    VDrawTool::AddRecord(id, Tool::PointFromArcAndTangent, doc);
    if (parse == Document::FullParse)
    {
        VToolPointFromArcAndTangent *point = new VToolPointFromArcAndTangent(doc, data, id, arcId, tangentPointId,
                                                                             crossPoint, typeCreation);
        scene->addItem(point);
        connect(point, &VToolPointFromArcAndTangent::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolPointFromArcAndTangent::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, point, &VToolPointFromArcAndTangent::Disable);
        connect(scene, &VMainGraphicsScene::EnableToolMove, point, &VToolPointFromArcAndTangent::EnableToolMove);
        doc->AddTool(id, point);
        doc->IncrementReferens(arcId);
        doc->IncrementReferens(tangentPointId);
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VToolPointFromArcAndTangent::FindPoint(const QPointF &p, const VArc *arc, const CrossCirclesPoint pType)
{
    QPointF p1, p2;
    const QPointF center = arc->GetCenter().toQPointF();
    const qreal radius = arc->GetRadius();
    const int res = VGObject::ContactPoints (p, center, radius, p1, p2);

    QLineF r1Arc(center, p1);
    r1Arc.setLength(radius+10);

    QLineF r2Arc(center, p2);
    r2Arc.setLength(radius+10);

    switch(res)
    {
        case 2:
        {
            int localRes = 0;
            bool flagP1 = false;

            if (arc->IsIntersectLine(r1Arc))
            {
                ++localRes;
                flagP1 = true;
            }

            if (arc->IsIntersectLine(r2Arc))
            {
                ++localRes;
            }

            switch(localRes)
            {
                case 2:
                    if (pType == CrossCirclesPoint::FirstPoint)
                    {
                        return p1;
                    }
                    else
                    {
                        return p2;
                    }
                    break;
                case 1:
                    if (flagP1)
                    {
                        return p1;
                    }
                    else
                    {
                        return p2;
                    }
                    break;
                case 0:
                default:
                    return QPointF(0, 0);
                    break;
            }

            break;
        }
        case 1:
            if (arc->IsIntersectLine(r1Arc))
            {
                return p1;
            }
            else
            {
                return QPointF(0, 0);
            }
            break;
        case 3:
        case 0:
        default:
            return QPointF(0, 0);
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolPointFromArcAndTangent::GetTangentPointId() const
{
    return tangentPointId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromArcAndTangent::SetTangentPointId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        tangentPointId = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolPointFromArcAndTangent::GetArcId() const
{
    return arcId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromArcAndTangent::SetArcId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        arcId = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
CrossCirclesPoint VToolPointFromArcAndTangent::GetCrossCirclesPoint() const
{
    return crossPoint;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromArcAndTangent::SetCrossCirclesPoint(CrossCirclesPoint &value)
{
    crossPoint = value;

    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromArcAndTangent::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolPointFromArcAndTangent>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromArcAndTangent::RemoveReferens()
{
    doc->DecrementReferens(arcId);
    doc->DecrementReferens(tangentPointId);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromArcAndTangent::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogPointFromArcAndTangent>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromArcAndTangent::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogPointFromArcAndTangent *dialogTool = qobject_cast<DialogPointFromArcAndTangent*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrArc, QString().setNum(dialogTool->GetArcId()));
    doc->SetAttribute(domElement, AttrTangent, QString().setNum(dialogTool->GetTangentPointId()));
    doc->SetAttribute(domElement, AttrCrossPoint,
                      QString().setNum(static_cast<int>(dialogTool->GetCrossCirclesPoint())));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromArcAndTangent::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolSinglePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrArc, arcId);
    doc->SetAttribute(tag, AttrTangent, tangentPointId);
    doc->SetAttribute(tag, AttrCrossPoint, static_cast<int>(crossPoint));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromArcAndTangent::ReadToolAttributes(const QDomElement &domElement)
{
    arcId = doc->GetParametrUInt(domElement, AttrArc, NULL_ID_STR);
    tangentPointId = doc->GetParametrUInt(domElement, AttrTangent, NULL_ID_STR);
    crossPoint = static_cast<CrossCirclesPoint>(doc->GetParametrUInt(domElement, AttrCrossPoint, "1"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromArcAndTangent::SetVisualization()
{
    if (vis != nullptr)
    {
        VisToolPointFromArcAndTangent *visual = qobject_cast<VisToolPointFromArcAndTangent *>(vis);
        SCASSERT(visual != nullptr);

        visual->setPoint1Id(tangentPointId);
        visual->setArcId(arcId);
        visual->setCrossPoint(crossPoint);
        visual->RefreshGeometry();
    }
}
