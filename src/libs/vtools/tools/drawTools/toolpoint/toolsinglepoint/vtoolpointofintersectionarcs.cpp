/************************************************************************
 **
 **  @file   vtoolpointofintersectionarcs.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 5, 2015
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

#include "vtoolpointofintersectionarcs.h"

#include <QLineF>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <new>

#include "../../../../dialogs/tools/dialogpointofintersectionarcs.h"
#include "../../../../visualization/line/vistoolpointofintersectionarcs.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/exception/vexceptionobjecterror.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../../../dialogs/tools/dialogtool.h"
#include "../../../../visualization/visualization.h"
#include "../../../vabstracttool.h"
#include "../../vdrawtool.h"
#include "vtoolsinglepoint.h"

template <class T> class QSharedPointer;

const QString VToolPointOfIntersectionArcs::ToolType = QStringLiteral("pointOfIntersectionArcs");

//---------------------------------------------------------------------------------------------------------------------
VToolPointOfIntersectionArcs::VToolPointOfIntersectionArcs(const VToolPointOfIntersectionArcsInitData &initData,
                                                           QGraphicsItem *parent)
    :VToolSinglePoint(initData.doc, initData.data, initData.id, parent),
      firstArcId(initData.firstArcId),
      secondArcId(initData.secondArcId),
      crossPoint(initData.pType)
{
    ToolCreation(initData.typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionArcs::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    const QPointer<DialogPointOfIntersectionArcs> dialogTool = qobject_cast<DialogPointOfIntersectionArcs *>(m_dialog);
    SCASSERT(not dialogTool.isNull())
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(m_id);
    dialogTool->SetFirstArcId(firstArcId);
    dialogTool->SetSecondArcId(secondArcId);
    dialogTool->SetCrossArcPoint(crossPoint);
    dialogTool->SetPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
VToolPointOfIntersectionArcs *VToolPointOfIntersectionArcs::Create(const QPointer<DialogTool> &dialog,
                                                                   VMainGraphicsScene *scene, VAbstractPattern *doc,
                                                                   VContainer *data)
{
    SCASSERT(not dialog.isNull())
    const QPointer<DialogPointOfIntersectionArcs> dialogTool = qobject_cast<DialogPointOfIntersectionArcs *>(dialog);
    SCASSERT(not dialogTool.isNull())

    VToolPointOfIntersectionArcsInitData initData;
    initData.firstArcId = dialogTool->GetFirstArcId();
    initData.secondArcId = dialogTool->GetSecondArcId();
    initData.pType = dialogTool->GetCrossArcPoint();
    initData.name = dialogTool->GetPointName();
    initData.scene = scene;
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    VToolPointOfIntersectionArcs *point = Create(initData);
    if (point != nullptr)
    {
        point->m_dialog = dialog;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
VToolPointOfIntersectionArcs *VToolPointOfIntersectionArcs::Create(VToolPointOfIntersectionArcsInitData initData)
{
    const QSharedPointer<VArc> firstArc = initData.data->GeometricObject<VArc>(initData.firstArcId);
    const QSharedPointer<VArc> secondArc = initData.data->GeometricObject<VArc>(initData.secondArcId);

    QPointF point;
    const bool success = FindPoint(firstArc.data(), secondArc.data(), initData.pType, &point);

    if (not success)
    {
        const QString errorMsg = tr("Error calculating point '%1'. Arcs '%2' and '%3' have no point of intersection")
                      .arg(initData.name, firstArc->name(), secondArc->name());
        qApp->IsPedantic() ? throw VExceptionObjectError(errorMsg) : qWarning() << errorMsg;
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
        VAbstractTool::AddRecord(initData.id, Tool::PointOfIntersectionArcs, initData.doc);
        VToolPointOfIntersectionArcs *point = new VToolPointOfIntersectionArcs(initData);
        initData.scene->addItem(point);
        InitToolConnections(initData.scene, point);
        VAbstractPattern::AddTool(initData.id, point);
        initData.doc->IncrementReferens(firstArc->getIdTool());
        initData.doc->IncrementReferens(secondArc->getIdTool());
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
bool VToolPointOfIntersectionArcs::FindPoint(const VArc *arc1, const VArc *arc2, const CrossCirclesPoint pType,
                                             QPointF *intersectionPoint)
{
    SCASSERT(intersectionPoint != nullptr)

    QPointF p1, p2;
    const QPointF centerArc1 = static_cast<QPointF>(arc1->GetCenter());
    const QPointF centerArc2 = static_cast<QPointF>(arc2->GetCenter());
    const int res = VGObject::IntersectionCircles(centerArc1, arc1->GetRadius(), centerArc2, arc2->GetRadius(), p1, p2);

    QLineF r1Arc1(centerArc1, p1);
    r1Arc1.setLength(r1Arc1.length()+10);

    QLineF r1Arc2(centerArc2, p1);
    r1Arc2.setLength(r1Arc2.length()+10);

    QLineF r2Arc1(centerArc1, p2);
    r2Arc1.setLength(r2Arc1.length()+10);

    QLineF r2Arc2(centerArc2, p2);
    r2Arc2.setLength(r2Arc2.length()+10);

    switch(res)
    {
        case 2:
        {
            int localRes = 0;
            bool flagP1 = false;

            if (arc1->IsIntersectLine(r1Arc1)  && arc2->IsIntersectLine(r1Arc2))
            {
                ++localRes;
                flagP1 = true;
            }

            if (arc1->IsIntersectLine(r2Arc1)  && arc2->IsIntersectLine(r2Arc2))
            {
                ++localRes;
            }

            switch(localRes)
            {
                case 2:
                    if (pType == CrossCirclesPoint::FirstPoint)
                    {
                        *intersectionPoint = p1;
                        return true;
                    }
                    else
                    {
                        *intersectionPoint = p2;
                        return true;
                    }
                case 1:
                    if (flagP1)
                    {
                        *intersectionPoint = p1;
                        return true;
                    }
                    else
                    {
                        *intersectionPoint = p2;
                        return true;
                    }
                case 0:
                default:
                    return false;
            }

            break;
        }
        case 1:
            if (arc1->IsIntersectLine(r1Arc1) && arc2->IsIntersectLine(r1Arc2))
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
QString VToolPointOfIntersectionArcs::FirstArcName() const
{
    return VAbstractTool::data.GetGObject(firstArcId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolPointOfIntersectionArcs::SecondArcName() const
{
    return VAbstractTool::data.GetGObject(secondArcId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
CrossCirclesPoint VToolPointOfIntersectionArcs::GetCrossCirclesPoint() const
{
    return crossPoint;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionArcs::SetCrossCirclesPoint(const CrossCirclesPoint &value)
{
    crossPoint = value;

    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(m_id);
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionArcs::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolPointOfIntersectionArcs>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionArcs::ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id)
{
    try
    {
        ContextMenu<DialogPointOfIntersectionArcs>(event, id);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionArcs::RemoveReferens()
{
    const auto firstArc = VAbstractTool::data.GetGObject(firstArcId);
    const auto secondArc = VAbstractTool::data.GetGObject(secondArcId);

    doc->DecrementReferens(firstArc->getIdTool());
    doc->DecrementReferens(secondArc->getIdTool());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionArcs::SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                                              QList<quint32> &newDependencies)
{
    SCASSERT(not m_dialog.isNull())
    const QPointer<DialogPointOfIntersectionArcs> dialogTool = qobject_cast<DialogPointOfIntersectionArcs *>(m_dialog);
    SCASSERT(not dialogTool.isNull())

    AddDependence(oldDependencies, firstArcId);
    AddDependence(oldDependencies, secondArcId);
    AddDependence(newDependencies, dialogTool->GetFirstArcId());
    AddDependence(newDependencies, dialogTool->GetSecondArcId());

    doc->SetAttribute(domElement, AttrName, dialogTool->GetPointName());
    doc->SetAttribute(domElement, AttrFirstArc, QString().setNum(dialogTool->GetFirstArcId()));
    doc->SetAttribute(domElement, AttrSecondArc, QString().setNum(dialogTool->GetSecondArcId()));
    doc->SetAttribute(domElement, AttrCrossPoint, QString().setNum(static_cast<int>(dialogTool->GetCrossArcPoint())));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionArcs::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolSinglePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrFirstArc, firstArcId);
    doc->SetAttribute(tag, AttrSecondArc, secondArcId);
    doc->SetAttribute(tag, AttrCrossPoint, static_cast<int>(crossPoint));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionArcs::ReadToolAttributes(const QDomElement &domElement)
{
    firstArcId = doc->GetParametrUInt(domElement, AttrFirstArc, NULL_ID_STR);
    secondArcId = doc->GetParametrUInt(domElement, AttrSecondArc, NULL_ID_STR);
    crossPoint = static_cast<CrossCirclesPoint>(doc->GetParametrUInt(domElement, AttrCrossPoint, QChar('1')));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersectionArcs::SetVisualization()
{
    if (not vis.isNull())
    {
        VisToolPointOfIntersectionArcs *visual = qobject_cast<VisToolPointOfIntersectionArcs *>(vis);
        SCASSERT(visual != nullptr)

        visual->setArc1Id(firstArcId);
        visual->setArc2Id(secondArcId);
        visual->setCrossPoint(crossPoint);
        visual->RefreshGeometry();
    }
}
