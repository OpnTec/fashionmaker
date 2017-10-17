/************************************************************************
 **
 **  @file   vtoolpointofintersectioncurves.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 1, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include <QLineF>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <new>

#include "../../../../dialogs/tools/dialogpointofintersectioncurves.h"
#include "../../../../dialogs/tools/dialogtool.h"
#include "../../../../visualization/path/../visualization.h"
#include "../../../../visualization/path/vistoolpointofintersectioncurves.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../../vabstracttool.h"
#include "../../vdrawtool.h"
#include "vtoolsinglepoint.h"

template <class T> class QSharedPointer;

const QString VToolPointOfIntersectionCurves::ToolType = QStringLiteral("pointOfIntersectionCurves");

//---------------------------------------------------------------------------------------------------------------------
VToolPointOfIntersectionCurves::VToolPointOfIntersectionCurves(const VToolPointOfIntersectionCurvesInitData &initData,
                                                               QGraphicsItem *parent)
    :VToolSinglePoint(initData.doc, initData.data, initData.id, parent),
      firstCurveId(initData.firstCurveId),
      secondCurveId(initData.secondCurveId),
      vCrossPoint(initData.vCrossPoint),
      hCrossPoint(initData.hCrossPoint)
{
    ToolCreation(initData.typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCurves::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    auto dialogTool = qobject_cast<DialogPointOfIntersectionCurves*>(m_dialog);
    SCASSERT(dialogTool != nullptr)
    auto p = VAbstractTool::data.GeometricObject<VPointF>(m_id);
    dialogTool->SetFirstCurveId(firstCurveId);
    dialogTool->SetSecondCurveId(secondCurveId);
    dialogTool->SetVCrossPoint(vCrossPoint);
    dialogTool->SetHCrossPoint(hCrossPoint);
    dialogTool->SetPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
VToolPointOfIntersectionCurves *VToolPointOfIntersectionCurves::Create(QSharedPointer<DialogTool> dialog,
                                                                       VMainGraphicsScene *scene,
                                                                       VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(not dialog.isNull())
    QSharedPointer<DialogPointOfIntersectionCurves> dialogTool = dialog.objectCast<DialogPointOfIntersectionCurves>();
    SCASSERT(not dialogTool.isNull())

    VToolPointOfIntersectionCurvesInitData initData;
    initData.firstCurveId = dialogTool->GetFirstCurveId();
    initData.secondCurveId = dialogTool->GetSecondCurveId();
    initData.vCrossPoint = dialogTool->GetVCrossPoint();
    initData.hCrossPoint = dialogTool->GetHCrossPoint();
    initData.name = dialogTool->getPointName();
    initData.scene = scene;
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    VToolPointOfIntersectionCurves *point = Create(initData);
    if (point != nullptr)
    {
        point->m_dialog = dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
VToolPointOfIntersectionCurves *VToolPointOfIntersectionCurves::Create(VToolPointOfIntersectionCurvesInitData initData)
{
    auto curve1 = initData.data->GeometricObject<VAbstractCurve>(initData.firstCurveId);
    auto curve2 = initData.data->GeometricObject<VAbstractCurve>(initData.secondCurveId);

    const QPointF point = VToolPointOfIntersectionCurves::FindPoint(curve1->GetPoints(), curve2->GetPoints(),
                                                                    initData.vCrossPoint, initData.hCrossPoint);

    VPointF *p = new VPointF(point, initData.name, initData.mx, initData.my);
    p->SetShowLabel(initData.showLabel);

    if (initData.typeCreation == Source::FromGui)
    {
        initData.id = initData.data->AddGObject(p);
    }
    else
    {
        initData.data->UpdateGObject(initData.id, p);
        if (initData.parse != Document::FullParse)
        {
            initData.doc->UpdateToolData(initData.id, initData.data);
        }
    }

    if (initData.parse == Document::FullParse)
    {
        VAbstractTool::AddRecord(initData.id, Tool::PointOfIntersectionCurves, initData.doc);
        auto point = new VToolPointOfIntersectionCurves(initData);
        initData.scene->addItem(point);
        InitToolConnections(initData.scene, point);
        VAbstractPattern::AddTool(initData.id, point);
        initData.doc->IncrementReferens(curve1->getIdTool());
        initData.doc->IncrementReferens(curve2->getIdTool());
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VToolPointOfIntersectionCurves::FindPoint(const QVector<QPointF> &curve1Points,
                                                  const QVector<QPointF> &curve2Points,
                                                  VCrossCurvesPoint vCrossPoint, HCrossCurvesPoint hCrossPoint)
{
    if (curve1Points.isEmpty() || curve2Points.isEmpty())
    {
        return QPointF();
    }

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
            if (p.y() < minY)
            {
                minY = p.y();
                vIntersections.clear();
                vIntersections.append(p);
            }
            else if (VFuzzyComparePossibleNulls(p.y(), minY))
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
            else if (VFuzzyComparePossibleNulls(p.y(), maxY))
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

    if (hCrossPoint == HCrossCurvesPoint::RightmostPoint)
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
QString VToolPointOfIntersectionCurves::FirstCurveName() const
{
    return VAbstractTool::data.GetGObject(firstCurveId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolPointOfIntersectionCurves::SecondCurveName() const
{
    return VAbstractTool::data.GetGObject(secondCurveId)->name();
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

    auto obj = VAbstractTool::data.GetGObject(m_id);
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

    auto obj = VAbstractTool::data.GetGObject(m_id);
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCurves::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolPointOfIntersectionCurves>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCurves::ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id)
{
    try
    {
        ContextMenu<DialogPointOfIntersectionCurves>(event, id);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCurves::RemoveReferens()
{
    const auto curve1 = VAbstractTool::data.GetGObject(firstCurveId);
    const auto curve2 = VAbstractTool::data.GetGObject(secondCurveId);

    doc->DecrementReferens(curve1->getIdTool());
    doc->DecrementReferens(curve2->getIdTool());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionCurves::SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                                                QList<quint32> &newDependencies)
{
    SCASSERT(not m_dialog.isNull())
    auto dialogTool = qobject_cast<DialogPointOfIntersectionCurves*>(m_dialog);
    SCASSERT(dialogTool != nullptr)

    AddDependence(oldDependencies, firstCurveId);
    AddDependence(oldDependencies, secondCurveId);
    AddDependence(newDependencies, dialogTool->GetFirstCurveId());
    AddDependence(newDependencies, dialogTool->GetSecondCurveId());

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
    if (not vis.isNull())
    {
        auto visual = qobject_cast<VisToolPointOfIntersectionCurves *>(vis);
        SCASSERT(visual != nullptr)

        visual->setObject1Id(firstCurveId);
        visual->setObject2Id(secondCurveId);
        visual->setVCrossPoint(vCrossPoint);
        visual->setHCrossPoint(hCrossPoint);
        visual->RefreshGeometry();
    }
}
