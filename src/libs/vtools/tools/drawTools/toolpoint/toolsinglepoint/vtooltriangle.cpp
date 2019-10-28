/************************************************************************
 **
 **  @file   vtooltriangle.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include <QLine>
#include <QLineF>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <new>

#include "../../../../dialogs/tools/dialogtriangle.h"
#include "../../../../dialogs/tools/dialogtool.h"
#include "../../../../visualization/visualization.h"
#include "../../../../visualization/line/vistooltriangle.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/exception/vexceptionobjecterror.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../../vabstracttool.h"
#include "../../vdrawtool.h"
#include "vtoolsinglepoint.h"
#include "../vmisc/vmath.h"

template <class T> class QSharedPointer;

const QString VToolTriangle::ToolType = QStringLiteral("triangle");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolTriangle constructor.
 * @param initData init data.
 * @param parent parent object.
 */
VToolTriangle::VToolTriangle(const VToolTriangleInitData &initData, QGraphicsItem *parent)
    :VToolSinglePoint(initData.doc, initData.data, initData.id, parent),
      axisP1Id(initData.axisP1Id),
      axisP2Id(initData.axisP2Id),
      firstPointId(initData.firstPointId),
      secondPointId(initData.secondPointId)
{
    ToolCreation(initData.typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolTriangle::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    const QPointer<DialogTriangle> dialogTool = qobject_cast<DialogTriangle *>(m_dialog);
    SCASSERT(not dialogTool.isNull())
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(m_id);
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
VToolTriangle* VToolTriangle::Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene *scene,
                                     VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(not dialog.isNull())
    const QPointer<DialogTriangle> dialogTool = qobject_cast<DialogTriangle *>(dialog);
    SCASSERT(not dialogTool.isNull())

    VToolTriangleInitData initData;
    initData.axisP1Id = dialogTool->GetAxisP1Id();
    initData.axisP2Id = dialogTool->GetAxisP2Id();
    initData.firstPointId = dialogTool->GetFirstPointId();
    initData.secondPointId = dialogTool->GetSecondPointId();
    initData.name = dialogTool->GetPointName();
    initData.scene = scene;
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    VToolTriangle* point = Create(initData);
    if (point != nullptr)
    {
        point->m_dialog = dialog;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create helps to create the tool.
 * @param initData init data.
 * @return the created tool
 */
VToolTriangle* VToolTriangle::Create(VToolTriangleInitData initData)
{
    const QSharedPointer<VPointF> axisP1 = initData.data->GeometricObject<VPointF>(initData.axisP1Id);
    const QSharedPointer<VPointF> axisP2 = initData.data->GeometricObject<VPointF>(initData.axisP2Id);
    const QSharedPointer<VPointF> firstPoint = initData.data->GeometricObject<VPointF>(initData.firstPointId);
    const QSharedPointer<VPointF> secondPoint = initData.data->GeometricObject<VPointF>(initData.secondPointId);

    QPointF point;
    const bool success = FindPoint(static_cast<QPointF>(*axisP1), static_cast<QPointF>(*axisP2),
                                   static_cast<QPointF>(*firstPoint), static_cast<QPointF>(*secondPoint), &point);

    if (not success)
    {
        const QString errorMsg = tr("Error calculating point '%1'. Point of intersection cannot be found")
                      .arg(initData.name);
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
        VAbstractTool::AddRecord(initData.id, Tool::Triangle, initData.doc);
        VToolTriangle *point = new VToolTriangle(initData);
        initData.scene->addItem(point);
        InitToolConnections(initData.scene, point);
        VAbstractPattern::AddTool(initData.id, point);
        initData.doc->IncrementReferens(axisP1->getIdTool());
        initData.doc->IncrementReferens(axisP2->getIdTool());
        initData.doc->IncrementReferens(firstPoint->getIdTool());
        initData.doc->IncrementReferens(secondPoint->getIdTool());
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
 * @param intersectionPoint point intersection two foots right triangle
 * @return true if the intersection exists.
 */
bool VToolTriangle::FindPoint(const QPointF &axisP1, const QPointF &axisP2, const QPointF &firstPoint,
                              const QPointF &secondPoint, QPointF *intersectionPoint)
{
    SCASSERT(intersectionPoint != nullptr)

    QLineF axis(axisP1, axisP2);
    QLineF hypotenuse(firstPoint, secondPoint);

    QPointF startPoint;
    QLineF::IntersectType intersect = axis.intersect(hypotenuse, &startPoint);
    if (intersect != QLineF::UnboundedIntersection && intersect != QLineF::BoundedIntersection)
    {
        return false;
    }
    if (VFuzzyComparePossibleNulls(axis.angle(), hypotenuse.angle())
        || VFuzzyComparePossibleNulls(qAbs(axis.angle() - hypotenuse.angle()), 180))
    {
        return false;
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
            *intersectionPoint = line.p2();
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolTriangle::AxisP1Name() const
{
    return VAbstractTool::data.GetGObject(axisP1Id)->name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolTriangle::AxisP2Name() const
{
    return VAbstractTool::data.GetGObject(axisP2Id)->name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolTriangle::FirstPointName() const
{
    return VAbstractTool::data.GetGObject(firstPointId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolTriangle::SecondPointName() const
{
    return VAbstractTool::data.GetGObject(secondPointId)->name();
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
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolTriangle::SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                               QList<quint32> &newDependencies)
{
    SCASSERT(not m_dialog.isNull())
    const QPointer<DialogTriangle> dialogTool = qobject_cast<DialogTriangle *>(m_dialog);
    SCASSERT(not dialogTool.isNull())

    AddDependence(oldDependencies, axisP1Id);
    AddDependence(oldDependencies, axisP2Id);
    AddDependence(oldDependencies, firstPointId);
    AddDependence(oldDependencies, secondPointId);
    AddDependence(newDependencies, dialogTool->GetAxisP1Id());
    AddDependence(newDependencies, dialogTool->GetAxisP2Id());
    AddDependence(newDependencies, dialogTool->GetFirstPointId());
    AddDependence(newDependencies, dialogTool->GetSecondPointId());

    doc->SetAttribute(domElement, AttrName, dialogTool->GetPointName());
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
    if (not vis.isNull())
    {
        VisToolTriangle * visual = qobject_cast<VisToolTriangle *>(vis);
        SCASSERT(visual != nullptr)

        visual->setObject1Id(axisP1Id);
        visual->setObject2Id(axisP2Id);
        visual->setHypotenuseP1Id(firstPointId);
        visual->setHypotenuseP2Id(secondPointId);
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTriangle::ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id)
{
    try
    {
        ContextMenu<DialogTriangle>(event, id);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolTriangle::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolTriangle>(show);
}
