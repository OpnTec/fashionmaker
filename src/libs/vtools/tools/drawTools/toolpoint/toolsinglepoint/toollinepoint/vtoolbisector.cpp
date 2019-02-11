/************************************************************************
 **
 **  @file   vtoolbisector.cpp
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

#include "vtoolbisector.h"

#include <QLineF>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <new>

#include "../../../../../dialogs/tools/dialogbisector.h"
#include "../../../../../dialogs/tools/dialogtool.h"
#include "../../../../../visualization/visualization.h"
#include "../../../../../visualization/line/vistoolbisector.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../../../vabstracttool.h"
#include "../../../vdrawtool.h"
#include "vtoollinepoint.h"

template <class T> class QSharedPointer;

const QString VToolBisector::ToolType = QStringLiteral("bisector");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolBisector constructor.
 * @param initData init data.
 */
VToolBisector::VToolBisector(const VToolBisectorInitData &initData, QGraphicsItem *parent)
    :VToolLinePoint(initData.doc, initData.data, initData.id, initData.typeLine, initData.lineColor, initData.formula,
                    initData.secondPointId, 0, parent),
      firstPointId(initData.firstPointId),
      thirdPointId(initData.thirdPointId)
{
    ToolCreation(initData.typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VToolBisector::BisectorAngle(const QPointF &firstPoint, const QPointF &secondPoint, const QPointF &thirdPoint)
{
    QLineF line1(secondPoint, firstPoint);
    QLineF line2(secondPoint, thirdPoint);
    qreal angle = line1.angleTo(line2);
    if (angle>180)
    {
        angle = 360 - angle;
        return line1.angle()-angle/2;
    }
    else
    {
        return line1.angle()+angle/2;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FindPoint find bisector point.
 * @param firstPoint first point of angle.
 * @param secondPoint second point of angle.
 * @param thirdPoint third point of angle.
 * @param length bisector length.
 * @return bisector point.
 */
QPointF VToolBisector::FindPoint(const QPointF &firstPoint, const QPointF &secondPoint,
                                 const QPointF &thirdPoint, const qreal &length)
{
    QLineF line1(secondPoint, firstPoint);
    line1.setAngle(BisectorAngle(firstPoint, secondPoint, thirdPoint));
    line1.setLength(length);
    return line1.p2();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolBisector::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    const QPointer<DialogBisector> dialogTool = qobject_cast<DialogBisector *>(m_dialog);
    SCASSERT(not dialogTool.isNull())
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(m_id);
    dialogTool->SetTypeLine(m_lineType);
    dialogTool->SetLineColor(lineColor);
    dialogTool->SetFormula(formulaLength);
    dialogTool->SetFirstPointId(firstPointId);
    dialogTool->SetSecondPointId(basePointId);
    dialogTool->SetThirdPointId(thirdPointId);
    dialogTool->SetPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool form GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 */
VToolBisector* VToolBisector::Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene *scene,
                                     VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(not dialog.isNull())
    const QPointer<DialogBisector> dialogTool = qobject_cast<DialogBisector *>(dialog);
    SCASSERT(not dialogTool.isNull())

    VToolBisectorInitData initData;
    initData.formula = dialogTool->GetFormula();
    initData.firstPointId = dialogTool->GetFirstPointId();
    initData.secondPointId = dialogTool->GetSecondPointId();
    initData.thirdPointId = dialogTool->GetThirdPointId();
    initData.typeLine = dialogTool->GetTypeLine();
    initData.lineColor = dialogTool->GetLineColor();
    initData.name = dialogTool->GetPointName();
    initData.scene = scene;
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    VToolBisector *point = Create(initData);
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
 */
VToolBisector* VToolBisector::Create(VToolBisectorInitData &initData)
{
    const QSharedPointer<VPointF> firstPoint = initData.data->GeometricObject<VPointF>(initData.firstPointId);
    const QSharedPointer<VPointF> secondPoint = initData.data->GeometricObject<VPointF>(initData.secondPointId);
    const QSharedPointer<VPointF> thirdPoint = initData.data->GeometricObject<VPointF>(initData.thirdPointId);

    const qreal result = CheckFormula(initData.id, initData.formula, initData.data);

    QPointF fPoint = VToolBisector::FindPoint(static_cast<QPointF>(*firstPoint), static_cast<QPointF>(*secondPoint),
                                              static_cast<QPointF>(*thirdPoint), qApp->toPixel(result));

    VPointF *p = new VPointF(fPoint, initData.name, initData.mx, initData.my);
    p->SetShowLabel(initData.showLabel);

    if (initData.typeCreation == Source::FromGui)
    {
        initData.id = initData.data->AddGObject(p);
        initData.data->AddLine(initData.secondPointId, initData.id);
    }
    else
    {
        initData.data->UpdateGObject(initData.id, p);
        initData.data->AddLine(initData.secondPointId, initData.id);
        if (initData.parse != Document::FullParse)
        {
            initData.doc->UpdateToolData(initData.id, initData.data);
        }
    }

    if (initData.parse == Document::FullParse)
    {
        VAbstractTool::AddRecord(initData.id, Tool::Bisector, initData.doc);
        VToolBisector *point = new VToolBisector(initData);
        initData.scene->addItem(point);
        InitToolConnections(initData.scene, point);
        VAbstractPattern::AddTool(initData.id, point);
        initData.doc->IncrementReferens(firstPoint->getIdTool());
        initData.doc->IncrementReferens(secondPoint->getIdTool());
        initData.doc->IncrementReferens(thirdPoint->getIdTool());
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolBisector::FirstPointName() const
{
    return VAbstractTool::data.GetGObject(firstPointId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolBisector::ThirdPointName() const
{
    return VAbstractTool::data.GetGObject(thirdPointId)->name();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolBisector::RemoveReferens()
{
    const auto firstPoint = VAbstractTool::data.GetGObject(firstPointId);
    const auto thirdPoint = VAbstractTool::data.GetGObject(thirdPointId);

    doc->DecrementReferens(firstPoint->getIdTool());
    doc->DecrementReferens(thirdPoint->getIdTool());
    VToolLinePoint::RemoveReferens();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolBisector::SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                               QList<quint32> &newDependencies)
{
    SCASSERT(not m_dialog.isNull())
    QPointer<DialogBisector> dialogTool = qobject_cast<DialogBisector *>(m_dialog);
    SCASSERT(not dialogTool.isNull())

    AddDependence(oldDependencies, firstPointId);
    AddDependence(oldDependencies, basePointId);
    AddDependence(oldDependencies, thirdPointId);
    AddDependence(newDependencies, dialogTool->GetFirstPointId());
    AddDependence(newDependencies, dialogTool->GetSecondPointId());
    AddDependence(newDependencies, dialogTool->GetThirdPointId());

    doc->SetAttribute(domElement, AttrName, dialogTool->GetPointName());
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->GetTypeLine());
    doc->SetAttribute(domElement, AttrLineColor, dialogTool->GetLineColor());
    doc->SetAttribute(domElement, AttrLength, dialogTool->GetFormula());
    doc->SetAttribute(domElement, AttrFirstPoint, QString().setNum(dialogTool->GetFirstPointId()));
    doc->SetAttribute(domElement, AttrSecondPoint, QString().setNum(dialogTool->GetSecondPointId()));
    doc->SetAttribute(domElement, AttrThirdPoint, QString().setNum(dialogTool->GetThirdPointId()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBisector::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolLinePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrLength, formulaLength);
    doc->SetAttribute(tag, AttrFirstPoint, firstPointId);
    doc->SetAttribute(tag, AttrSecondPoint, basePointId);
    doc->SetAttribute(tag, AttrThirdPoint, thirdPointId);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBisector::ReadToolAttributes(const QDomElement &domElement)
{
    m_lineType = doc->GetParametrString(domElement, AttrTypeLine, TypeLineLine);
    lineColor = doc->GetParametrString(domElement, AttrLineColor, ColorBlack);
    formulaLength = doc->GetParametrString(domElement, AttrLength, QString());
    firstPointId = doc->GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
    basePointId = doc->GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);
    thirdPointId = doc->GetParametrUInt(domElement, AttrThirdPoint, NULL_ID_STR);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBisector::SetVisualization()
{
    if (not vis.isNull())
    {
        VisToolBisector *visual = qobject_cast<VisToolBisector *>(vis);
        SCASSERT(visual != nullptr)

        visual->setObject1Id(firstPointId);
        visual->setObject2Id(basePointId);
        visual->setObject3Id(thirdPointId);
        visual->setLength(qApp->TrVars()->FormulaToUser(formulaLength, qApp->Settings()->GetOsSeparator()));
        visual->setLineStyle(LineStyleToPenStyle(m_lineType));
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBisector::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolBisector>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolBisector::ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id)
{
    try
    {
        ContextMenu<DialogBisector>(event, id);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}
