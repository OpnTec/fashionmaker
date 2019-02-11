/************************************************************************
 **
 **  @file   vtoolpointofintersection.cpp
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

#include "vtoolpointofintersection.h"

#include <QPointF>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <new>

#include "../../../../dialogs/tools/dialogpointofintersection.h"
#include "../../../../visualization/line/vistoolpointofintersection.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/ifcdef.h"
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

const QString VToolPointOfIntersection::ToolType = QStringLiteral("pointOfIntersection");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolPointOfIntersection constructor.
 * @param initData init data.
 * @param parent parent object.
 */
VToolPointOfIntersection::VToolPointOfIntersection(const VToolPointOfIntersectionInitData &initData,
                                                   QGraphicsItem *parent)
    :VToolSinglePoint(initData.doc, initData.data, initData.id, parent),
      firstPointId(initData.firstPointId),
      secondPointId(initData.secondPointId)
{
    ToolCreation(initData.typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolPointOfIntersection::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    const QPointer<DialogPointOfIntersection> dialogTool = qobject_cast<DialogPointOfIntersection *>(m_dialog);
    SCASSERT(not dialogTool.isNull())
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(m_id);
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
VToolPointOfIntersection *VToolPointOfIntersection::Create(const QPointer<DialogTool> &dialog,
                                                           VMainGraphicsScene *scene, VAbstractPattern *doc,
                                                           VContainer *data)
{
    SCASSERT(not dialog.isNull())
    const QPointer<DialogPointOfIntersection> dialogTool = qobject_cast<DialogPointOfIntersection *>(dialog);
    SCASSERT(not dialogTool.isNull())

    VToolPointOfIntersectionInitData initData;
    initData.firstPointId = dialogTool->GetFirstPointId();
    initData.secondPointId = dialogTool->GetSecondPointId();
    initData.name = dialogTool->GetPointName();
    initData.scene = scene;
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    VToolPointOfIntersection *point = Create(initData);
    if (point != nullptr)
    {
        point->m_dialog = dialog;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param initData init data.
 * @return the created tool
 */
VToolPointOfIntersection *VToolPointOfIntersection::Create(VToolPointOfIntersectionInitData initData)
{
    const QSharedPointer<VPointF> firstPoint = initData.data->GeometricObject<VPointF>(initData.firstPointId);
    const QSharedPointer<VPointF> secondPoint = initData.data->GeometricObject<VPointF>(initData.secondPointId);

    QPointF point(firstPoint->x(), secondPoint->y());

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
        VAbstractTool::AddRecord(initData.id, Tool::PointOfIntersection, initData.doc);
        VToolPointOfIntersection *point = new VToolPointOfIntersection(initData);
        initData.scene->addItem(point);
        InitToolConnections(initData.scene, point);
        VAbstractPattern::AddTool(initData.id, point);
        initData.doc->IncrementReferens(firstPoint->getIdTool());
        initData.doc->IncrementReferens(secondPoint->getIdTool());
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolPointOfIntersection::FirstPointName() const
{
    return VAbstractTool::data.GetGObject(firstPointId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolPointOfIntersection::SecondPointName() const
{
    return VAbstractTool::data.GetGObject(secondPointId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolPointOfIntersection::RemoveReferens()
{
    const auto firstPoint = VAbstractTool::data.GetGObject(firstPointId);
    const auto secondPoint = VAbstractTool::data.GetGObject(secondPointId);

    doc->DecrementReferens(firstPoint->getIdTool());
    doc->DecrementReferens(secondPoint->getIdTool());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolPointOfIntersection::SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                                          QList<quint32> &newDependencies)
{
    SCASSERT(not m_dialog.isNull())
    const QPointer<DialogPointOfIntersection> dialogTool = qobject_cast<DialogPointOfIntersection *>(m_dialog);
    SCASSERT(not dialogTool.isNull())

    AddDependence(oldDependencies, firstPointId);
    AddDependence(oldDependencies, secondPointId);
    AddDependence(newDependencies, dialogTool->GetFirstPointId());
    AddDependence(newDependencies, dialogTool->GetSecondPointId());

    doc->SetAttribute(domElement, AttrName, dialogTool->GetPointName());
    doc->SetAttribute(domElement, AttrFirstPoint, QString().setNum(dialogTool->GetFirstPointId()));
    doc->SetAttribute(domElement, AttrSecondPoint, QString().setNum(dialogTool->GetSecondPointId()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersection::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolSinglePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrFirstPoint, firstPointId);
    doc->SetAttribute(tag, AttrSecondPoint, secondPointId);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersection::ReadToolAttributes(const QDomElement &domElement)
{
    firstPointId = doc->GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
    secondPointId = doc->GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersection::SetVisualization()
{
    if (not vis.isNull())
    {
        VisToolPointOfIntersection *visual = qobject_cast<VisToolPointOfIntersection *>(vis);
        SCASSERT(visual != nullptr)

        visual->setObject1Id(firstPointId);
        visual->setPoint2Id(secondPointId);
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersection::ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id)
{
    try
    {
        ContextMenu<DialogPointOfIntersection>(event, id);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolPointOfIntersection::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolPointOfIntersection>(show);
}
