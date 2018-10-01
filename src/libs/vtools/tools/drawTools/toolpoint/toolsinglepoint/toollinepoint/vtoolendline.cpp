/************************************************************************
 **
 **  @file   vtoolendline.cpp
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

#include "vtoolendline.h"

#include <QLineF>
#include <QPointF>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <new>

#include "../../../../../dialogs/tools/dialogendline.h"
#include "../../../../../dialogs/tools/dialogtool.h"
#include "../../../../../visualization/visualization.h"
#include "../../../../../visualization/line/vistoolendline.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/ifcdef.h"
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

const QString VToolEndLine::ToolType = QStringLiteral("endLine");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolEndLine constructor.
 * @param initData init data.
 * @param parent parent object.
 */
VToolEndLine::VToolEndLine(const VToolEndLineInitData &initData, QGraphicsItem *parent)
    :VToolLinePoint(initData.doc, initData.data, initData.id, initData.typeLine, initData.lineColor,
                    initData.formulaLength, initData.basePointId, 0, parent),
      formulaAngle(initData.formulaAngle)
{
    ToolCreation(initData.typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolEndLine::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    m_dialog->setModal(true);
    QPointer<DialogEndLine> dialogTool = qobject_cast<DialogEndLine *>(m_dialog);
    SCASSERT(not dialogTool.isNull())
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(m_id);
    dialogTool->SetTypeLine(m_lineType);
    dialogTool->SetLineColor(lineColor);
    dialogTool->SetFormula(formulaLength);
    dialogTool->SetAngle(formulaAngle);
    dialogTool->SetBasePointId(basePointId);
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
VToolEndLine* VToolEndLine::Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                   VContainer *data)
{
    SCASSERT(not dialog.isNull())
    const QPointer<DialogEndLine> dialogTool = qobject_cast<DialogEndLine *>(dialog);
    SCASSERT(not dialogTool.isNull())

    VToolEndLineInitData initData;
    initData.formulaLength = dialogTool->GetFormula();
    initData.formulaAngle = dialogTool->GetAngle();
    initData.basePointId = dialogTool->GetBasePointId();
    initData.typeLine = dialogTool->GetTypeLine();
    initData.lineColor = dialogTool->GetLineColor();
    initData.name = dialogTool->getPointName();
    initData.scene = scene;
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;

    VToolEndLine *point = Create(initData);
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
VToolEndLine* VToolEndLine::Create(VToolEndLineInitData &initData)
{
    const QSharedPointer<VPointF> basePoint = initData.data->GeometricObject<VPointF>(initData.basePointId);
    QLineF line = QLineF(static_cast<QPointF>(*basePoint), QPointF(basePoint->x()+100, basePoint->y()));

    line.setAngle(CheckFormula(initData.id, initData.formulaAngle, initData.data)); //First set angle.
    line.setLength(qApp->toPixel(CheckFormula(initData.id, initData.formulaLength, initData.data)));

    VPointF *p = new VPointF(line.p2(), initData.name, initData.mx, initData.my);
    p->SetShowLabel(initData.showLabel);

    if (initData.typeCreation == Source::FromGui)
    {
        initData.id = initData.data->AddGObject(p);
        initData.data->AddLine(initData.basePointId, initData.id);
    }
    else
    {
        initData.data->UpdateGObject(initData.id, p);
        initData.data->AddLine(initData.basePointId, initData.id);
        if (initData.parse != Document::FullParse)
        {
            initData.doc->UpdateToolData(initData.id, initData.data);
        }
    }

    if (initData.parse == Document::FullParse)
    {
        VAbstractTool::AddRecord(initData.id, Tool::EndLine, initData.doc);
        VToolEndLine *point = new VToolEndLine(initData);
        initData.scene->addItem(point);
        InitToolConnections(initData.scene, point);
        VAbstractPattern::AddTool(initData.id, point);
        initData.doc->IncrementReferens(basePoint->getIdTool());
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolEndLine::SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                              QList<quint32> &newDependencies)
{
    SCASSERT(not m_dialog.isNull())
    const QPointer<DialogEndLine> dialogTool = qobject_cast<DialogEndLine *>(m_dialog);
    SCASSERT(not dialogTool.isNull())

    AddDependence(oldDependencies, basePointId);
    AddDependence(newDependencies, dialogTool->GetBasePointId());

    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->GetTypeLine());
    doc->SetAttribute(domElement, AttrLineColor, dialogTool->GetLineColor());
    doc->SetAttribute(domElement, AttrLength, dialogTool->GetFormula());
    doc->SetAttribute(domElement, AttrAngle, dialogTool->GetAngle());
    doc->SetAttribute(domElement, AttrBasePoint, QString().setNum(dialogTool->GetBasePointId()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolEndLine::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolLinePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrLength, formulaLength);
    doc->SetAttribute(tag, AttrAngle, formulaAngle);
    doc->SetAttribute(tag, AttrBasePoint, basePointId);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolEndLine::ReadToolAttributes(const QDomElement &domElement)
{
    m_lineType = doc->GetParametrString(domElement, AttrTypeLine, TypeLineLine);
    lineColor = doc->GetParametrString(domElement, AttrLineColor, ColorBlack);
    formulaLength = doc->GetParametrString(domElement, AttrLength, QString());
    basePointId = doc->GetParametrUInt(domElement, AttrBasePoint, NULL_ID_STR);
    formulaAngle = doc->GetParametrString(domElement, AttrAngle, QString());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolEndLine::SetVisualization()
{
    if (not vis.isNull())
    {
        VisToolEndLine *visual = qobject_cast<VisToolEndLine *>(vis);
        SCASSERT(visual != nullptr)

        visual->setObject1Id(basePointId);
        visual->setLength(qApp->TrVars()->FormulaToUser(formulaLength, qApp->Settings()->GetOsSeparator()));
        visual->SetAngle(qApp->TrVars()->FormulaToUser(formulaAngle, qApp->Settings()->GetOsSeparator()));
        visual->setLineStyle(LineStyleToPenStyle(m_lineType));
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolEndLine::GetFormulaAngle() const
{
    VFormula fAngle(formulaAngle, getData());
    fAngle.setCheckZero(false);
    fAngle.setToolId(m_id);
    fAngle.setPostfix(degreeSymbol);
    fAngle.Eval();
    return fAngle;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolEndLine::SetFormulaAngle(const VFormula &value)
{
    if (value.error() == false)
    {
        formulaAngle = value.GetFormula(FormulaType::FromUser);

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(m_id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolEndLine::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolEndLine>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolEndLine::ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id)
{
    try
    {
        ContextMenu<DialogEndLine>(event, id);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}
