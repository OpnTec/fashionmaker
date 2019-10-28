/************************************************************************
 **
 **  @file   vtoolpointfromarcandtangent.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   6 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include <QLineF>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <new>

#include "../../../../dialogs/tools/dialogpointfromarcandtangent.h"
#include "../../../../dialogs/tools/dialogtool.h"
#include "../../../../visualization/visualization.h"
#include "../../../../visualization/line/vistoolpointfromarcandtangent.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/exception/vexceptionobjecterror.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../../vabstracttool.h"
#include "../../vdrawtool.h"
#include "vtoolsinglepoint.h"

template <class T> class QSharedPointer;

const QString VToolPointFromArcAndTangent::ToolType = QStringLiteral("pointFromArcAndTangent");

//---------------------------------------------------------------------------------------------------------------------
VToolPointFromArcAndTangent::VToolPointFromArcAndTangent(const VToolPointFromArcAndTangentInitData &initData,
                                                         QGraphicsItem *parent)
    :VToolSinglePoint(initData.doc, initData.data, initData.id, parent),
      arcId(initData.arcId),
      tangentPointId(initData.tangentPointId),
      crossPoint(initData.crossPoint)
{
    ToolCreation(initData.typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromArcAndTangent::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    const QPointer<DialogPointFromArcAndTangent> dialogTool = qobject_cast<DialogPointFromArcAndTangent *>(m_dialog);
    SCASSERT(not dialogTool.isNull())
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(m_id);
    dialogTool->SetArcId(arcId);
    dialogTool->SetCrossCirclesPoint(crossPoint);
    dialogTool->SetTangentPointId(tangentPointId);
    dialogTool->SetPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
VToolPointFromArcAndTangent *VToolPointFromArcAndTangent::Create(const QPointer<DialogTool> &dialog,
                                                                 VMainGraphicsScene *scene,
                                                                 VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(not dialog.isNull())
    const QPointer<DialogPointFromArcAndTangent> dialogTool = qobject_cast<DialogPointFromArcAndTangent *>(dialog);
    SCASSERT(not dialogTool.isNull())

    VToolPointFromArcAndTangentInitData initData;
    initData.arcId = dialogTool->GetArcId();
    initData.tangentPointId = dialogTool->GetTangentPointId();
    initData.crossPoint = dialogTool->GetCrossCirclesPoint();
    initData.name = dialogTool->GetPointName();
    initData.scene = scene;
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    VToolPointFromArcAndTangent *point = Create(initData);
    if (point != nullptr)
    {
        point->m_dialog = dialog;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
VToolPointFromArcAndTangent *VToolPointFromArcAndTangent::Create(VToolPointFromArcAndTangentInitData initData)
{
    const VArc arc = *initData.data->GeometricObject<VArc>(initData.arcId);
    const VPointF tPoint = *initData.data->GeometricObject<VPointF>(initData.tangentPointId);

    QPointF point;
    const bool success = VToolPointFromArcAndTangent::FindPoint(static_cast<QPointF>(tPoint), &arc,
                                                                initData.crossPoint, &point);

    if (not success)
    {
        const QString errorMsg = tr("Error calculating point '%1'. Tangent to arc '%2' from point '%3' cannot be found")
                .arg(initData.name, arc.name(), tPoint.name());
        qApp->IsPedantic() ? throw VExceptionObjectError(errorMsg) :
                             qWarning() << VAbstractApplication::patternMessageSignature + errorMsg;
    }

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
        VAbstractTool::AddRecord(initData.id, Tool::PointFromArcAndTangent, initData.doc);
        VToolPointFromArcAndTangent *point = new VToolPointFromArcAndTangent(initData);
        initData.scene->addItem(point);
        InitToolConnections(initData.scene, point);
        VAbstractPattern::AddTool(initData.id, point);
        initData.doc->IncrementReferens(arc.getIdTool());
        initData.doc->IncrementReferens(tPoint.getIdTool());
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
bool VToolPointFromArcAndTangent::FindPoint(const QPointF &p, const VArc *arc, const CrossCirclesPoint pType,
                                            QPointF *intersectionPoint)
{
    SCASSERT(intersectionPoint != nullptr)

    QPointF p1, p2;
    const QPointF center = static_cast<QPointF>(arc->GetCenter());
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
                    *intersectionPoint = (pType == CrossCirclesPoint::FirstPoint ? p1 : p2);
                    return true;
                case 1:
                    *intersectionPoint = (flagP1 ? p1 : p2);
                    return true;
                case 0:
                default:
                    return false;
            }

            break;
        }
        case 1:
            if (arc->IsIntersectLine(r1Arc))
            {
                *intersectionPoint = p1;
                return true;
            }
            else
            {
                return false;
            }
        case 3:
        case 0:
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolPointFromArcAndTangent::TangentPointName() const
{
    return VAbstractTool::data.GetGObject(tangentPointId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolPointFromArcAndTangent::ArcName() const
{
    return VAbstractTool::data.GetGObject(arcId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
CrossCirclesPoint VToolPointFromArcAndTangent::GetCrossCirclesPoint() const
{
    return crossPoint;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromArcAndTangent::SetCrossCirclesPoint(CrossCirclesPoint value)
{
    crossPoint = value;

    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(m_id);
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromArcAndTangent::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolPointFromArcAndTangent>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromArcAndTangent::ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id)
{
    try
    {
        ContextMenu<DialogPointFromArcAndTangent>(event, id);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromArcAndTangent::RemoveReferens()
{
    const auto arc = VAbstractTool::data.GetGObject(arcId);
    const auto tP = VAbstractTool::data.GetGObject(tangentPointId);

    doc->DecrementReferens(arc->getIdTool());
    doc->DecrementReferens(tP->getIdTool());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromArcAndTangent::SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                                             QList<quint32> &newDependencies)
{
    SCASSERT(not m_dialog.isNull())
    const QPointer<DialogPointFromArcAndTangent> dialogTool = qobject_cast<DialogPointFromArcAndTangent *>(m_dialog);
    SCASSERT(not dialogTool.isNull())

    AddDependence(oldDependencies, arcId);
    AddDependence(oldDependencies, tangentPointId);
    AddDependence(newDependencies, dialogTool->GetArcId());
    AddDependence(newDependencies, dialogTool->GetTangentPointId());

    doc->SetAttribute(domElement, AttrName, dialogTool->GetPointName());
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
    crossPoint = static_cast<CrossCirclesPoint>(doc->GetParametrUInt(domElement, AttrCrossPoint, QChar('1')));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromArcAndTangent::SetVisualization()
{
    if (not vis.isNull())
    {
        VisToolPointFromArcAndTangent *visual = qobject_cast<VisToolPointFromArcAndTangent *>(vis);
        SCASSERT(visual != nullptr)

        visual->setObject1Id(tangentPointId);
        visual->setArcId(arcId);
        visual->setCrossPoint(crossPoint);
        visual->RefreshGeometry();
    }
}
