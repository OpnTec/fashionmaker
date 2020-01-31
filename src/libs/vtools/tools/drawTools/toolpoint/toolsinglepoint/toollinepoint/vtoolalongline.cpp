/************************************************************************
 **
 **  @file   vtoolalongline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#include "vtoolalongline.h"

#include <QLineF>
#include <QPointF>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <new>

#include "../../../../../dialogs/tools/dialogalongline.h"
#include "../../../../../dialogs/tools/dialogtool.h"
#include "../../../../../visualization/visualization.h"
#include "../../../../../visualization/line/vistoolalongline.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vpatterndb/variables/vlinelength.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../../../vabstracttool.h"
#include "../../../vdrawtool.h"
#include "vtoollinepoint.h"

template <class T> class QSharedPointer;

const QString VToolAlongLine::ToolType = QStringLiteral("alongLine");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolAlongLine constuctor.
 * @param initData init data.
 * @param parent parent object.
 */
VToolAlongLine::VToolAlongLine(const VToolAlongLineInitData &initData, QGraphicsItem *parent)
    :VToolLinePoint(initData.doc, initData.data, initData.id, initData.typeLine, initData.lineColor, initData.formula,
                    initData.firstPointId, 0, parent),
      secondPointId(initData.secondPointId)
{
    ToolCreation(initData.typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolAlongLine::RemoveReferens()
{
    const auto secondPoint = VAbstractTool::data.GetGObject(secondPointId);
    doc->DecrementReferens(secondPoint->getIdTool());
    VToolLinePoint::RemoveReferens();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolAlongLine::SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                                QList<quint32> &newDependencies)
{
    SCASSERT(not m_dialog.isNull())
    const QPointer<DialogAlongLine> dialogTool = qobject_cast<DialogAlongLine *>(m_dialog);
    SCASSERT(not dialogTool.isNull())

    AddDependence(oldDependencies, basePointId);
    AddDependence(oldDependencies, secondPointId);
    AddDependence(newDependencies, dialogTool->GetFirstPointId());
    AddDependence(newDependencies, dialogTool->GetSecondPointId());

    doc->SetAttribute(domElement, AttrName, dialogTool->GetPointName());
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->GetTypeLine());
    doc->SetAttribute(domElement, AttrLineColor, dialogTool->GetLineColor());
    doc->SetAttribute(domElement, AttrLength, dialogTool->GetFormula());
    doc->SetAttribute(domElement, AttrFirstPoint, dialogTool->GetFirstPointId());
    doc->SetAttribute(domElement, AttrSecondPoint, dialogTool->GetSecondPointId());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolAlongLine::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolLinePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrLength, formulaLength);
    doc->SetAttribute(tag, AttrFirstPoint, basePointId);
    doc->SetAttribute(tag, AttrSecondPoint, secondPointId);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolAlongLine::ReadToolAttributes(const QDomElement &domElement)
{
    m_lineType = doc->GetParametrString(domElement, AttrTypeLine, TypeLineLine);
    lineColor = doc->GetParametrString(domElement, AttrLineColor, ColorBlack);
    formulaLength = doc->GetParametrString(domElement, AttrLength, QString());
    basePointId = doc->GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
    secondPointId = doc->GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolAlongLine::SetVisualization()
{
    if (not vis.isNull())
    {
        VisToolAlongLine *visual = qobject_cast<VisToolAlongLine *>(vis);
        SCASSERT(visual != nullptr)
        visual->setObject1Id(basePointId);
        visual->setObject2Id(secondPointId);
        visual->setLength(qApp->TrVars()->FormulaToUser(formulaLength, qApp->Settings()->GetOsSeparator()));
        visual->setLineStyle(LineStyleToPenStyle(m_lineType));
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolAlongLine::MakeToolTip() const
{
    const QSharedPointer<VPointF> basePoint = VAbstractTool::data.GeometricObject<VPointF>(basePointId);
    const QSharedPointer<VPointF> secondPoint = VAbstractTool::data.GeometricObject<VPointF>(secondPointId);
    const QSharedPointer<VPointF> current = VAbstractTool::data.GeometricObject<VPointF>(m_id);

    const QLineF curLine(static_cast<QPointF>(*basePoint), static_cast<QPointF>(*current));
    const QLineF curToSecond(static_cast<QPointF>(*current), static_cast<QPointF>(*secondPoint));

    const QString toolTip = QString("<table>"
                                    "<tr> <td><b>%9:</b> %10</td> </tr>"
                                    "<tr> <td><b>%1:</b> %2 %3</td> </tr>"
                                    "<tr> <td><b>%4:</b> %5°</td> </tr>"
                                    "<tr> <td><b>%6:</b> %2 %3</td> </tr>"
                                    "<tr> <td><b>%7:</b> %8 %3</td> </tr>"
                                    "</table>")
            .arg(tr("Length"))
            .arg(qApp->fromPixel(curLine.length()))
            .arg(UnitsToStr(qApp->patternUnit(), true), tr("Angle"))
            .arg(curLine.angle())
            .arg(QStringLiteral("%1->%2").arg(basePoint->name(), current->name()),
                 QStringLiteral("%1->%2").arg(current->name(), secondPoint->name()))
            .arg(qApp->fromPixel(curToSecond.length()))
            .arg(tr("Label"), current->name());
    return toolTip;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolAlongLine::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolAlongLine>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolAlongLine::ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id)
{
    try
    {
        ContextMenu<DialogAlongLine>(event, id);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolAlongLine::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    const QPointer<DialogAlongLine> dialogTool = qobject_cast<DialogAlongLine *>(m_dialog);
    SCASSERT(not dialogTool.isNull())
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(m_id);
    dialogTool->SetTypeLine(m_lineType);
    dialogTool->SetLineColor(lineColor);
    dialogTool->SetFormula(formulaLength);
    dialogTool->SetFirstPointId(basePointId);
    dialogTool->SetSecondPointId(secondPointId);
    dialogTool->SetPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool form GUI.
 * @param dialog dialog options.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 */
VToolAlongLine* VToolAlongLine::Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene *scene,
                                       VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(not dialog.isNull())
    const QPointer<DialogAlongLine> dialogTool = qobject_cast<DialogAlongLine *>(dialog);
    SCASSERT(not dialogTool.isNull())

    VToolAlongLineInitData initData;
    initData.formula = dialogTool->GetFormula();
    initData.firstPointId = dialogTool->GetFirstPointId();
    initData.secondPointId = dialogTool->GetSecondPointId();
    initData.typeLine = dialogTool->GetTypeLine();
    initData.lineColor = dialogTool->GetLineColor();
    initData.name = dialogTool->GetPointName();
    initData.scene = scene;
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    VToolAlongLine *point = Create(initData);
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
VToolAlongLine* VToolAlongLine::Create(VToolAlongLineInitData &initData)
{
    const QSharedPointer<VPointF> firstPoint = initData.data->GeometricObject<VPointF>(initData.firstPointId);
    const QSharedPointer<VPointF> secondPoint = initData.data->GeometricObject<VPointF>(initData.secondPointId);
    QLineF line = QLineF(static_cast<QPointF>(*firstPoint), static_cast<QPointF>(*secondPoint));

    //Declare special variable "CurrentLength"
    VLengthLine *length = new VLengthLine(firstPoint.data(), initData.firstPointId, secondPoint.data(),
                                          initData.secondPointId, *initData.data->GetPatternUnit());
    length->SetName(currentLength);
    initData.data->AddVariable(length);

    line.setLength(qApp->toPixel(CheckFormula(initData.id, initData.formula, initData.data)));

    VPointF *p = new VPointF(line.p2(), initData.name, initData.mx, initData.my);
    p->SetShowLabel(initData.showLabel);

    if (initData.typeCreation == Source::FromGui)
    {
        initData.id = initData.data->AddGObject(p);
        initData.data->AddLine(initData.firstPointId, initData.id);
        initData.data->AddLine(initData.id, initData.secondPointId);
    }
    else
    {
        initData.data->UpdateGObject(initData.id, p);
        initData.data->AddLine(initData.firstPointId, initData.id);
        initData.data->AddLine(initData.id, initData.secondPointId);
        if (initData.parse != Document::FullParse)
        {
            initData.doc->UpdateToolData(initData.id, initData.data);
        }
    }

    VToolAlongLine *point = nullptr;
    if (initData.parse == Document::FullParse)
    {
        VAbstractTool::AddRecord(initData.id, Tool::AlongLine, initData.doc);
        point = new VToolAlongLine(initData);
        initData.scene->addItem(point);
        InitToolConnections(initData.scene, point);
        VAbstractPattern::AddTool(initData.id, point);
        initData.doc->IncrementReferens(firstPoint->getIdTool());
        initData.doc->IncrementReferens(secondPoint->getIdTool());
    }
    //Very important to delete it. Only this tool need this special variable.
    initData.data->RemoveVariable(currentLength);
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolAlongLine::SecondPointName() const
{
    return VAbstractTool::data.GetGObject(secondPointId)->name();
}
