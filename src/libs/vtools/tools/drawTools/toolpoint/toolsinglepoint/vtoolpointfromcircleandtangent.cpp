/************************************************************************
 **
 **  @file   vtoolpointfromcircleandtangent.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   3 6, 2015
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

#include "vtoolpointfromcircleandtangent.h"

#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <new>

#include "../../../../dialogs/tools/dialogpointfromcircleandtangent.h"
#include "../../../../dialogs/tools/dialogtool.h"
#include "../../../../visualization/visualization.h"
#include "../../../../visualization/line/vistoolpointfromcircleandtangent.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/xml/vdomdocument.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vformula.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../../vabstracttool.h"
#include "../../vdrawtool.h"
#include "vtoolsinglepoint.h"

template <class T> class QSharedPointer;

const QString VToolPointFromCircleAndTangent::ToolType = QStringLiteral("pointFromCircleAndTangent");

//---------------------------------------------------------------------------------------------------------------------
VToolPointFromCircleAndTangent::VToolPointFromCircleAndTangent(const VToolPointFromCircleAndTangentInitData &initData,
                                                               QGraphicsItem *parent)
    :VToolSinglePoint(initData.doc, initData.data, initData.id, parent),
      circleCenterId(initData.circleCenterId),
      tangentPointId(initData.tangentPointId),
      circleRadius(initData.circleRadius),
      crossPoint(initData.crossPoint)
{
    ToolCreation(initData.typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromCircleAndTangent::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    QSharedPointer<DialogPointFromCircleAndTangent> dialogTool = m_dialog.objectCast<DialogPointFromCircleAndTangent>();
    SCASSERT(not dialogTool.isNull())
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(m_id);
    dialogTool->SetCircleCenterId(circleCenterId);
    dialogTool->SetCircleRadius(circleRadius);
    dialogTool->SetCrossCirclesPoint(crossPoint);
    dialogTool->SetTangentPointId(tangentPointId);
    dialogTool->SetPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
VToolPointFromCircleAndTangent *VToolPointFromCircleAndTangent::Create(QSharedPointer<DialogTool> dialog,
                                                                       VMainGraphicsScene *scene,
                                                                       VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(not dialog.isNull())
    QSharedPointer<DialogPointFromCircleAndTangent> dialogTool = dialog.objectCast<DialogPointFromCircleAndTangent>();
    SCASSERT(not dialogTool.isNull())

    VToolPointFromCircleAndTangentInitData initData;
    initData.circleCenterId = dialogTool->GetCircleCenterId();
    initData.circleRadius = dialogTool->GetCircleRadius();
    initData.tangentPointId = dialogTool->GetTangentPointId();
    initData.crossPoint = dialogTool->GetCrossCirclesPoint();
    initData.name = dialogTool->getPointName();
    initData.scene = scene;
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    VToolPointFromCircleAndTangent *point = Create(initData);
    if (point != nullptr)
    {
        point->m_dialog = dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
VToolPointFromCircleAndTangent *VToolPointFromCircleAndTangent::Create(VToolPointFromCircleAndTangentInitData &initData)
{
    const qreal radius = qApp->toPixel(CheckFormula(initData.id, initData.circleRadius, initData.data));
    const VPointF cPoint = *initData.data->GeometricObject<VPointF>(initData.circleCenterId);
    const VPointF tPoint = *initData.data->GeometricObject<VPointF>(initData.tangentPointId);

    const QPointF point = VToolPointFromCircleAndTangent::FindPoint(static_cast<QPointF>(tPoint),
                                                                    static_cast<QPointF>(cPoint), radius,
                                                                    initData.crossPoint);

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
        VAbstractTool::AddRecord(initData.id, Tool::PointFromCircleAndTangent, initData.doc);
        VToolPointFromCircleAndTangent *point = new VToolPointFromCircleAndTangent(initData);
        initData.scene->addItem(point);
        InitToolConnections(initData.scene, point);
        VAbstractPattern::AddTool(initData.id, point);
        initData.doc->IncrementReferens(cPoint.getIdTool());
        initData.doc->IncrementReferens(tPoint.getIdTool());
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VToolPointFromCircleAndTangent::FindPoint(const QPointF &p, const QPointF &center, qreal radius,
                                                  const CrossCirclesPoint crossPoint)
{
    QPointF p1, p2;
    const int res = VGObject::ContactPoints (p, center, radius, p1, p2);

    switch(res)
    {
        case 2:
            if (crossPoint == CrossCirclesPoint::FirstPoint)
            {
                return p1;
            }
            else
            {
                return p2;
            }
        case 1:
            return p1;
        case 3:
        case 0:
        default:
            return QPointF();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolPointFromCircleAndTangent::TangentPointName() const
{
    return VAbstractTool::data.GetGObject(tangentPointId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolPointFromCircleAndTangent::CircleCenterPointName() const
{
    return VAbstractTool::data.GetGObject(circleCenterId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolPointFromCircleAndTangent::GetTangentPointId() const
{
    return tangentPointId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromCircleAndTangent::SetTangentPointId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        tangentPointId = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(m_id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolPointFromCircleAndTangent::GetCircleCenterId() const
{
    return circleCenterId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromCircleAndTangent::SetCircleCenterId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        circleCenterId = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(m_id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolPointFromCircleAndTangent::GetCircleRadius() const
{
    VFormula radius(circleRadius, getData());
    radius.setCheckZero(true);
    radius.setToolId(m_id);
    radius.setPostfix(UnitsToStr(qApp->patternUnit()));
    return radius;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromCircleAndTangent::SetCircleRadius(const VFormula &value)
{
    if (value.error() == false)
    {
        if (value.getDoubleValue() > 0)// Formula don't check this, but radius can't be 0 or negative
        {
            circleRadius = value.GetFormula(FormulaType::FromUser);
            QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(m_id);
            SaveOption(obj);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
CrossCirclesPoint VToolPointFromCircleAndTangent::GetCrossCirclesPoint() const
{
    return crossPoint;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromCircleAndTangent::SetCrossCirclesPoint(const CrossCirclesPoint &value)
{
    crossPoint = value;

    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(m_id);
    SaveOption(obj);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromCircleAndTangent::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolPointFromCircleAndTangent>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromCircleAndTangent::ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id)
{
    try
    {
        ContextMenu<DialogPointFromCircleAndTangent>(event, id);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromCircleAndTangent::RemoveReferens()
{
    const auto circleCenter = VAbstractTool::data.GetGObject(circleCenterId);
    const auto tangentPoint = VAbstractTool::data.GetGObject(tangentPointId);

    doc->DecrementReferens(circleCenter->getIdTool());
    doc->DecrementReferens(tangentPoint->getIdTool());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromCircleAndTangent::SaveDialog(QDomElement &domElement)
{
    SCASSERT(not m_dialog.isNull())
    QSharedPointer<DialogPointFromCircleAndTangent> dialogTool = m_dialog.objectCast<DialogPointFromCircleAndTangent>();
    SCASSERT(not dialogTool.isNull())
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrCCenter, QString().setNum(dialogTool->GetCircleCenterId()));
    doc->SetAttribute(domElement, AttrTangent, QString().setNum(dialogTool->GetTangentPointId()));
    doc->SetAttribute(domElement, AttrCRadius, dialogTool->GetCircleRadius());
    doc->SetAttribute(domElement, AttrCrossPoint,
                      QString().setNum(static_cast<int>(dialogTool->GetCrossCirclesPoint())));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromCircleAndTangent::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolSinglePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrCCenter, circleCenterId);
    doc->SetAttribute(tag, AttrTangent, tangentPointId);
    doc->SetAttribute(tag, AttrCRadius, circleRadius);
    doc->SetAttribute(tag, AttrCrossPoint, static_cast<int>(crossPoint));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromCircleAndTangent::ReadToolAttributes(const QDomElement &domElement)
{
    circleCenterId = doc->GetParametrUInt(domElement, AttrCCenter, NULL_ID_STR);
    tangentPointId = doc->GetParametrUInt(domElement, AttrTangent, NULL_ID_STR);
    circleRadius = doc->GetParametrString(domElement, AttrCRadius);
    crossPoint = static_cast<CrossCirclesPoint>(doc->GetParametrUInt(domElement, AttrCrossPoint, "1"));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointFromCircleAndTangent::SetVisualization()
{
    if (not vis.isNull())
    {
        VisToolPointFromCircleAndTangent *visual = qobject_cast<VisToolPointFromCircleAndTangent *>(vis);
        SCASSERT(visual != nullptr)

        visual->setObject1Id(tangentPointId);
        visual->setObject2Id(circleCenterId);
        visual->setCRadius(circleRadius);
        visual->setCrossPoint(crossPoint);
        visual->RefreshGeometry();
    }
}
