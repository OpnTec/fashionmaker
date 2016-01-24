/************************************************************************
 **
 **  @file   vtoolpointofintersectioncurves.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 1, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "vtoolpointofintersectioncurves.h"
#include "../../../../dialogs/tools/dialogpointofintersectioncurves.h"
#include "../../../../visualization/vistoolpointofintersectioncurves.h"

const QString VToolPointOfIntersectionCurves::ToolType = QStringLiteral("pointOfIntersectionCurves");

//---------------------------------------------------------------------------------------------------------------------
VToolPointOfIntersectionCurves::VToolPointOfIntersectionCurves(VAbstractPattern *doc, VContainer *data,
                                                               const quint32 &id, const quint32 firstCurveId,
                                                               quint32 secondCurveId, VCrossCurvesPoint vCrossPoint,
                                                               HCrossCurvesPoint hCrossPoint, const Source &typeCreation,
                                                               QGraphicsItem *parent)
    :VToolSinglePoint(doc, data, id, parent),
      firstCurveId(firstCurveId),
      secondCurveId(secondCurveId),
      vCrossPoint(vCrossPoint),
      hCrossPoint(hCrossPoint)
{
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCurves::setDialog()
{
    SCASSERT(dialog != nullptr);
    auto dialogTool = qobject_cast<DialogPointOfIntersectionCurves*>(dialog);
    SCASSERT(dialogTool != nullptr);
    auto p = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->SetFirstCurveId(firstCurveId);
    dialogTool->SetSecondCurveId(secondCurveId);
    dialogTool->SetVCrossPoint(vCrossPoint);
    dialogTool->SetHCrossPoint(hCrossPoint);
    dialogTool->SetPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
VToolPointOfIntersectionCurves *VToolPointOfIntersectionCurves::Create(DialogTool *dialog, VMainGraphicsScene *scene,
                                                                       VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    auto dialogTool = qobject_cast<DialogPointOfIntersectionCurves*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const quint32 firstCurveId = dialogTool->GetFirstCurveId();
    const quint32 secondCurveId = dialogTool->GetSecondCurveId();
    const VCrossCurvesPoint vCrossPoint = dialogTool->GetVCrossPoint();
    const HCrossCurvesPoint hCrossPoint = dialogTool->GetHCrossPoint();
    const QString pointName = dialogTool->getPointName();
    VToolPointOfIntersectionCurves *point = Create(0, pointName, firstCurveId, secondCurveId, vCrossPoint, hCrossPoint,
                                                   5, 10, scene, doc, data, Document::FullParse, Source::FromGui);
    if (point != nullptr)
    {
        point->dialog=dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
VToolPointOfIntersectionCurves *VToolPointOfIntersectionCurves::Create(const quint32 _id, const QString &pointName,
                                                                       quint32 firstCurveId, quint32 secondCurveId,
                                                                       VCrossCurvesPoint vCrossPoint,
                                                                       HCrossCurvesPoint hCrossPoint, const qreal &mx,
                                                                       const qreal &my, VMainGraphicsScene *scene,
                                                                       VAbstractPattern *doc, VContainer *data,
                                                                       const Document &parse,
                                                                       const Source &typeCreation)
{
    auto curve1 = data->GeometricObject<VAbstractCurve>(firstCurveId);
    auto curve2 = data->GeometricObject<VAbstractCurve>(secondCurveId);

    const QPointF point = VToolPointOfIntersectionCurves::FindPoint(curve1->GetPoints(), curve2->GetPoints(),
                                                                    vCrossPoint, hCrossPoint);
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
    VDrawTool::AddRecord(id, Tool::PointOfIntersectionCurves, doc);
    if (parse == Document::FullParse)
    {
        auto point = new VToolPointOfIntersectionCurves(doc, data, id, firstCurveId, secondCurveId, vCrossPoint,
                                                        hCrossPoint, typeCreation);
        scene->addItem(point);
        connect(point, &VToolPointOfIntersectionCurves::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolPointOfIntersectionCurves::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, point, &VToolPointOfIntersectionCurves::Disable);
        connect(scene, &VMainGraphicsScene::EnableToolMove, point, &VToolPointOfIntersectionCurves::EnableToolMove);
        doc->AddTool(id, point);
        doc->IncrementReferens(firstCurveId);
        doc->IncrementReferens(secondCurveId);
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VToolPointOfIntersectionCurves::FindPoint(const QVector<QPointF> &curve1Points,
                                                  const QVector<QPointF> &curve2Points,
                                                  VCrossCurvesPoint vCrossPoint, HCrossCurvesPoint hCrossPoint)
{
    QVector<QPointF> intersections;
    for ( auto i = 0; i < curve1Points.count()-1; ++i )
    {
        intersections << VAbstractCurve::CurveIntersectLine(curve2Points,
                                                            QLineF(curve1Points.at(i), curve1Points.at(i+1)));
    }

    if (intersections.isEmpty())
    {
        return QPointF();
    }

    if (intersections.size() == 1)
    {
        return intersections.at(0);
    }

    QVector<QPointF> vIntersections;
    if (vCrossPoint == VCrossCurvesPoint::HighestPoint)
    {
        qreal minY = intersections.at(0).y();
        vIntersections.append(intersections.at(0));

        for ( auto i = 1; i < intersections.count(); ++i )
        {
            const QPointF p = intersections.at(i);
            if (p.y() > minY)
            {
                continue;
            }
            else if (p.y() < minY)
            {
                minY = p.y();
                vIntersections.clear();
                vIntersections.append(p);
            }
            else
            {
                vIntersections.append(p);
            }
        }
    }
    else
    {
        qreal maxY = intersections.at(0).y();
        vIntersections.append(intersections.at(0));

        for ( auto i = 1; i < intersections.count(); ++i )
        {
            const QPointF p = intersections.at(i);
            if (p.y() > maxY)
            {
                maxY = p.y();
                vIntersections.clear();
                vIntersections.append(p);
            }
            else if (p.y() < maxY)
            {
                continue;
            }
            else
            {
                vIntersections.append(p);
            }
        }
    }

    if (vIntersections.isEmpty())
    {
        return QPointF();
    }

    if (vIntersections.size() == 1)
    {
        return vIntersections.at(0);
    }

    QPointF crossPoint = vIntersections.at(0);

    if (hCrossPoint == HCrossCurvesPoint::RightPoint)
    {
        qreal maxX = vIntersections.at(0).x();

        for ( auto i = 1; i < vIntersections.count(); ++i )
        {
            const QPointF p = vIntersections.at(i);
            if (p.x() > maxX)
            {
                maxX = p.x();
                crossPoint = p;
            }
        }
    }
    else
    {
        qreal minX = vIntersections.at(0).x();

        for ( auto i = 1; i < vIntersections.count(); ++i )
        {
            const QPointF p = vIntersections.at(i);
            if (p.x() < minX)
            {
                minX = p.x();
                crossPoint = p;
            }
        }
    }

    return crossPoint;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolPointOfIntersectionCurves::GetFirstCurveId() const
{
    return firstCurveId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCurves::SetFirstCurveId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        firstCurveId = value;

        auto obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolPointOfIntersectionCurves::GetSecondCurveId() const
{
    return secondCurveId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCurves::SetSecondCurveId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        secondCurveId = value;

        auto obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VCrossCurvesPoint VToolPointOfIntersectionCurves::GetVCrossPoint() const
{
    return vCrossPoint;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCurves::SetVCrossPoint(const VCrossCurvesPoint &value)
{
    vCrossPoint = value;

    auto obj = VAbstractTool::data.GetGObject(id);
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
HCrossCurvesPoint VToolPointOfIntersectionCurves::GetHCrossPoint() const
{
    return hCrossPoint;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCurves::SetHCrossPoint(const HCrossCurvesPoint &value)
{
    hCrossPoint = value;

    auto obj = VAbstractTool::data.GetGObject(id);
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCurves::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolPointOfIntersectionCurves>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCurves::RemoveReferens()
{
    doc->DecrementReferens(firstCurveId);
    doc->DecrementReferens(secondCurveId);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCurves::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    try
    {
        ContextMenu<DialogPointOfIntersectionCurves>(this, event);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e);
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCurves::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    auto dialogTool = qobject_cast<DialogPointOfIntersectionCurves*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrCurve1, QString().setNum(dialogTool->GetFirstCurveId()));
    doc->SetAttribute(domElement, AttrCurve2, QString().setNum(dialogTool->GetSecondCurveId()));
    doc->SetAttribute(domElement, AttrVCrossPoint, QString().setNum(static_cast<int>(dialogTool->GetVCrossPoint())));
    doc->SetAttribute(domElement, AttrHCrossPoint, QString().setNum(static_cast<int>(dialogTool->GetHCrossPoint())));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCurves::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolSinglePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrCurve1, firstCurveId);
    doc->SetAttribute(tag, AttrCurve2, secondCurveId);
    doc->SetAttribute(tag, AttrVCrossPoint, static_cast<int>(vCrossPoint));
    doc->SetAttribute(tag, AttrHCrossPoint, static_cast<int>(hCrossPoint));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCurves::ReadToolAttributes(const QDomElement &domElement)
{
    firstCurveId = doc->GetParametrUInt(domElement, AttrCurve1, NULL_ID_STR);
    secondCurveId = doc->GetParametrUInt(domElement, AttrCurve2, NULL_ID_STR);
    vCrossPoint = static_cast<VCrossCurvesPoint>(doc->GetParametrUInt(domElement, AttrVCrossPoint, "1"));
    hCrossPoint = static_cast<HCrossCurvesPoint>(doc->GetParametrUInt(domElement, AttrHCrossPoint, "1"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCurves::SetVisualization()
{
    if (vis != nullptr)
    {
        auto visual = qobject_cast<VisToolPointOfIntersectionCurves *>(vis);
        SCASSERT(visual != nullptr);

        visual->setObject1Id(firstCurveId);
        visual->setObject2Id(secondCurveId);
        visual->setVCrossPoint(vCrossPoint);
        visual->setHCrossPoint(hCrossPoint);
        visual->RefreshGeometry();
    }
}
