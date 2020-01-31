/************************************************************************
 **
 **  @file   vtooltruedarts.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   23 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#include "vtooltruedarts.h"

#include <QLine>
#include <QLineF>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <new>

#include "../../../../dialogs/tools/dialogtruedarts.h"
#include "../../../../dialogs/tools/dialogtool.h"
#include "../../../../visualization/visualization.h"
#include "../../../../visualization/line/vistooltruedarts.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../../vabstracttool.h"
#include "../../vdrawtool.h"
#include "vtooldoublepoint.h"
#include "../vmisc/compatibility.h"

template <class T> class QSharedPointer;

const QString VToolTrueDarts::ToolType = QStringLiteral("trueDarts");

//---------------------------------------------------------------------------------------------------------------------
VToolTrueDarts::VToolTrueDarts(const VToolTrueDartsInitData &initData, QGraphicsItem *parent)
    :VToolDoublePoint(initData.doc, initData.data, initData.id, initData.p1id, initData.p2id, parent),
      baseLineP1Id (initData.baseLineP1Id),
      baseLineP2Id(initData.baseLineP2Id),
      dartP1Id(initData.dartP1Id),
      dartP2Id(initData.dartP2Id),
      dartP3Id(initData.dartP3Id)
{
    ToolCreation(initData.typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTrueDarts::FindPoint(const QPointF &baseLineP1, const QPointF &baseLineP2, const QPointF &dartP1,
                               const QPointF &dartP2, const QPointF &dartP3, QPointF &p1, QPointF &p2)
{
    const QLineF d2d1(dartP2, dartP1);
    const QLineF d2d3(dartP2, dartP3);

    const qreal degrees = d2d3.angleTo(d2d1);

    QLineF d2blP2(dartP2, baseLineP2);
    d2blP2.setAngle(d2blP2.angle()+degrees);

    if (Intersects(QLineF(baseLineP1, d2blP2.p2()), d2d1, &p1) == QLineF::NoIntersection)
    {
        p1 = QPointF(0, 0);
        p2 = QPointF(0, 0);
        return;
    }

    QLineF d2p1(dartP2, p1);
    d2p1.setAngle(d2p1.angle()-degrees);
    p2 = d2p1.p2();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTrueDarts::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    const QPointer<DialogTrueDarts> dialogTool = qobject_cast<DialogTrueDarts *>(m_dialog);
    SCASSERT(not dialogTool.isNull())

    const QSharedPointer<VPointF> p1 = VAbstractTool::data.GeometricObject<VPointF>(p1id);
    const QSharedPointer<VPointF> p2 = VAbstractTool::data.GeometricObject<VPointF>(p2id);

    dialogTool->SetChildrenId(p1id, p2id);
    dialogTool->SetNewDartPointNames(p1->name(), p2->name());
    dialogTool->SetFirstBasePointId(baseLineP1Id);
    dialogTool->SetSecondBasePointId(baseLineP2Id);
    dialogTool->SetFirstDartPointId(dartP1Id);
    dialogTool->SetSecondDartPointId(dartP2Id);
    dialogTool->SetThirdDartPointId(dartP3Id);
}

//---------------------------------------------------------------------------------------------------------------------
VToolTrueDarts *VToolTrueDarts::Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene *scene,
                                       VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(not dialog.isNull())
    const QPointer<DialogTrueDarts> dialogTool = qobject_cast<DialogTrueDarts *>(dialog);
    SCASSERT(not dialogTool.isNull())

    VToolTrueDartsInitData initData;
    initData.name1 = dialogTool->GetFirstNewDartPointName();
    initData.name2 = dialogTool->GetSecondNewDartPointName();
    initData.baseLineP1Id = dialogTool->GetFirstBasePointId();
    initData.baseLineP2Id = dialogTool->GetSecondBasePointId();
    initData.dartP1Id = dialogTool->GetFirstDartPointId();
    initData.dartP2Id = dialogTool->GetSecondDartPointId();
    initData.dartP3Id = dialogTool->GetThirdDartPointId();
    initData.scene = scene;
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    VToolTrueDarts *point = Create(initData);
    if (point != nullptr)
    {
        point->m_dialog = dialog;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
VToolTrueDarts *VToolTrueDarts::Create(VToolTrueDartsInitData initData)
{
    const QSharedPointer<VPointF> baseLineP1 = initData.data->GeometricObject<VPointF>(initData.baseLineP1Id);
    const QSharedPointer<VPointF> baseLineP2 = initData.data->GeometricObject<VPointF>(initData.baseLineP2Id);
    const QSharedPointer<VPointF> dartP1 = initData.data->GeometricObject<VPointF>(initData.dartP1Id);
    const QSharedPointer<VPointF> dartP2 = initData.data->GeometricObject<VPointF>(initData.dartP2Id);
    const QSharedPointer<VPointF> dartP3 = initData.data->GeometricObject<VPointF>(initData.dartP3Id);

    QPointF fPoint1;
    QPointF fPoint2;
    VToolTrueDarts::FindPoint(static_cast<QPointF>(*baseLineP1), static_cast<QPointF>(*baseLineP2),
                              static_cast<QPointF>(*dartP1), static_cast<QPointF>(*dartP2),
                              static_cast<QPointF>(*dartP3), fPoint1, fPoint2);

    VPointF *p1 = new VPointF(fPoint1, initData.name1, initData.mx1, initData.my1, initData.id);
    p1->SetShowLabel(initData.showLabel1);

    VPointF *p2 = new VPointF(fPoint2, initData.name2, initData.mx2, initData.my2, initData.id);
    p2->SetShowLabel(initData.showLabel2);

    if (initData.typeCreation == Source::FromGui)
    {
        initData.id = initData.data->getNextId();//Just reserve id for tool
        initData.p1id = initData.data->AddGObject(p1);
        initData.p2id = initData.data->AddGObject(p2);
    }
    else
    {
        initData.data->UpdateGObject(initData.p1id, p1);
        initData.data->UpdateGObject(initData.p2id, p2);
        if (initData.parse != Document::FullParse)
        {
            initData.doc->UpdateToolData(initData.id, initData.data);
        }
    }

    if (initData.parse == Document::FullParse)
    {
        VAbstractTool::AddRecord(initData.id, Tool::TrueDarts, initData.doc);
        VToolTrueDarts *points = new VToolTrueDarts(initData);
        initData.scene->addItem(points);
        InitToolConnections(initData.scene, points);
        VAbstractPattern::AddTool(initData.id, points);
        initData.doc->IncrementReferens(baseLineP1->getIdTool());
        initData.doc->IncrementReferens(baseLineP2->getIdTool());
        initData.doc->IncrementReferens(dartP1->getIdTool());
        initData.doc->IncrementReferens(dartP2->getIdTool());
        initData.doc->IncrementReferens(dartP3->getIdTool());
        return points;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTrueDarts::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolTrueDarts>(show);
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolTrueDarts::BaseLineP1Name() const
{
    return VAbstractTool::data.GetGObject(baseLineP1Id)->name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolTrueDarts::BaseLineP2Name() const
{
    return VAbstractTool::data.GetGObject(baseLineP2Id)->name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolTrueDarts::DartP1Name() const
{
    return VAbstractTool::data.GetGObject(dartP1Id)->name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolTrueDarts::DartP2Name() const
{
    return VAbstractTool::data.GetGObject(dartP2Id)->name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolTrueDarts::DartP3Name() const
{
    return VAbstractTool::data.GetGObject(dartP3Id)->name();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTrueDarts::ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id)
{
    try
    {
        ContextMenu<DialogTrueDarts>(event, id);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTrueDarts::RemoveReferens()
{
    const auto baseLineP1 = VAbstractTool::data.GetGObject(baseLineP1Id);
    const auto baseLineP2 = VAbstractTool::data.GetGObject(baseLineP2Id);
    const auto dartP1 = VAbstractTool::data.GetGObject(dartP1Id);
    const auto dartP2 = VAbstractTool::data.GetGObject(dartP2Id);
    const auto dartP3 = VAbstractTool::data.GetGObject(dartP3Id);

    doc->DecrementReferens(baseLineP1->getIdTool());
    doc->DecrementReferens(baseLineP2->getIdTool());
    doc->DecrementReferens(dartP1->getIdTool());
    doc->DecrementReferens(dartP2->getIdTool());
    doc->DecrementReferens(dartP3->getIdTool());
    VToolDoublePoint::RemoveReferens();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTrueDarts::SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                                QList<quint32> &newDependencies)
{
    SCASSERT(not m_dialog.isNull())
    const QPointer<DialogTrueDarts> dialogTool = qobject_cast<DialogTrueDarts *>(m_dialog);
    SCASSERT(not dialogTool.isNull())

    AddDependence(oldDependencies, baseLineP1Id);
    AddDependence(oldDependencies, baseLineP2Id);
    AddDependence(oldDependencies, dartP1Id);
    AddDependence(oldDependencies, dartP2Id);
    AddDependence(oldDependencies, dartP3Id);

    AddDependence(newDependencies, dialogTool->GetFirstBasePointId());
    AddDependence(newDependencies, dialogTool->GetSecondBasePointId());
    AddDependence(newDependencies, dialogTool->GetFirstDartPointId());
    AddDependence(newDependencies, dialogTool->GetSecondDartPointId());
    AddDependence(newDependencies, dialogTool->GetThirdDartPointId());

    doc->SetAttribute(domElement, AttrName1, dialogTool->GetFirstNewDartPointName());
    doc->SetAttribute(domElement, AttrName2, dialogTool->GetSecondNewDartPointName());
    doc->SetAttribute(domElement, AttrBaseLineP1, QString().setNum(dialogTool->GetFirstBasePointId()));
    doc->SetAttribute(domElement, AttrBaseLineP2, QString().setNum(dialogTool->GetSecondBasePointId()));
    doc->SetAttribute(domElement, AttrDartP1, QString().setNum(dialogTool->GetFirstDartPointId()));
    doc->SetAttribute(domElement, AttrDartP2, QString().setNum(dialogTool->GetSecondDartPointId()));
    doc->SetAttribute(domElement, AttrDartP3, QString().setNum(dialogTool->GetThirdDartPointId()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTrueDarts::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolDoublePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrBaseLineP1, baseLineP1Id);
    doc->SetAttribute(tag, AttrBaseLineP2, baseLineP2Id);
    doc->SetAttribute(tag, AttrDartP1, dartP1Id);
    doc->SetAttribute(tag, AttrDartP2, dartP2Id);
    doc->SetAttribute(tag, AttrDartP3, dartP3Id);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTrueDarts::ReadToolAttributes(const QDomElement &domElement)
{
    baseLineP1Id = doc->GetParametrUInt(domElement, AttrBaseLineP1, NULL_ID_STR);
    baseLineP2Id = doc->GetParametrUInt(domElement, AttrBaseLineP2, NULL_ID_STR);
    dartP1Id = doc->GetParametrUInt(domElement, AttrDartP1, NULL_ID_STR);
    dartP2Id = doc->GetParametrUInt(domElement, AttrDartP2, NULL_ID_STR);
    dartP3Id = doc->GetParametrUInt(domElement, AttrDartP3, NULL_ID_STR);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTrueDarts::SetVisualization()
{
    if (not vis.isNull())
    {
        VisToolTrueDarts *visual = qobject_cast<VisToolTrueDarts *>(vis);
        SCASSERT(visual != nullptr)

        visual->setObject1Id(baseLineP1Id);
        visual->setObject2Id(baseLineP2Id);
        visual->setD1PointId(dartP1Id);
        visual->setD2PointId(dartP2Id);
        visual->setD3PointId(dartP3Id);
        visual->RefreshGeometry();
    }
}
