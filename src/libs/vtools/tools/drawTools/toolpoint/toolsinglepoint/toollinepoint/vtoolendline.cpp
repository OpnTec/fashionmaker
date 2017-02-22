/************************************************************************
 **
 **  @file   vtoolendline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param typeLine line type.
 * @param formulaLength string with formula length of line.
 * @param formulaAngle formula angle of line.
 * @param basePointId id first point of line.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolEndLine::VToolEndLine(VAbstractPattern *doc, VContainer *data, const quint32 &id,  const QString &typeLine,
                           const QString &lineColor, const QString &formulaLength, const QString &formulaAngle,
                           const quint32 &basePointId, const Source &typeCreation, QGraphicsItem *parent)
    :VToolLinePoint(doc, data, id, typeLine, lineColor, formulaLength, basePointId, 0, parent),
      formulaAngle(formulaAngle)
{
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
VToolEndLine::~VToolEndLine()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolEndLine::setDialog()
{
    SCASSERT(dialog != nullptr)
    dialog->setModal(true);
    DialogEndLine *dialogTool = qobject_cast<DialogEndLine*>(dialog);
    SCASSERT(dialogTool != nullptr)
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->SetTypeLine(typeLine);
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
VToolEndLine* VToolEndLine::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                   VContainer *data)
{
    SCASSERT(dialog != nullptr)
    DialogEndLine *dialogTool = qobject_cast<DialogEndLine*>(dialog);
    SCASSERT(dialogTool)
    const QString pointName = dialogTool->getPointName();
    const QString typeLine = dialogTool->GetTypeLine();
    const QString lineColor = dialogTool->GetLineColor();
    QString formulaLength = dialogTool->GetFormula();
    QString formulaAngle = dialogTool->GetAngle();
    const quint32 basePointId = dialogTool->GetBasePointId();

    VToolEndLine *point = Create(0, pointName, typeLine, lineColor, formulaLength, formulaAngle,
                                 basePointId, 5, 10, scene, doc, data, Document::FullParse, Source::FromGui);
    if (point != nullptr)
    {
        point->dialog=dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param _id tool id, 0 if tool doesn't exist yet.
 * @param pointName point name.
 * @param typeLine line type.
 * @param lineColor line color.
 * @param formulaLength string with formula length of line.
 * @param formulaAngle formula angle of line.
 * @param basePointId id first point of line.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 * @return the created tool
 */
VToolEndLine* VToolEndLine::Create(const quint32 _id, const QString &pointName, const QString &typeLine,
                                   const QString &lineColor, QString &formulaLength, QString &formulaAngle,
                                   const quint32 &basePointId, const qreal &mx, const qreal &my,
                                   VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data,
                                   const Document &parse,
                                   const Source &typeCreation)
{
    const QSharedPointer<VPointF> basePoint = data->GeometricObject<VPointF>(basePointId);
    QLineF line = QLineF(*basePoint, QPointF(basePoint->x()+100, basePoint->y()));

    line.setAngle(CheckFormula(_id, formulaAngle, data)); //First set angle.
    line.setLength(qApp->toPixel(CheckFormula(_id, formulaLength, data)));
    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(new VPointF(line.p2(), pointName, mx, my));
        data->AddLine(basePointId, id);
    }
    else
    {
        data->UpdateGObject(id, new VPointF(line.p2(), pointName, mx, my));
        data->AddLine(basePointId, id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }

    if (parse == Document::FullParse)
    {
        VDrawTool::AddRecord(id, Tool::EndLine, doc);
        VToolEndLine *point = new VToolEndLine(doc, data, id, typeLine, lineColor, formulaLength, formulaAngle,
                                               basePointId, typeCreation);
        scene->addItem(point);
        InitToolConnections(scene, point);
        doc->AddTool(id, point);
        doc->IncrementReferens(basePoint->getIdTool());
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolEndLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    try
    {
        ContextMenu<DialogEndLine>(this, event);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e)
        return;//Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolEndLine::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr)
    DialogEndLine *dialogTool = qobject_cast<DialogEndLine*>(dialog);
    SCASSERT(dialogTool != nullptr)
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
    typeLine = doc->GetParametrString(domElement, AttrTypeLine, TypeLineLine);
    lineColor = doc->GetParametrString(domElement, AttrLineColor, ColorBlack);
    formulaLength = doc->GetParametrString(domElement, AttrLength, "");
    basePointId = doc->GetParametrUInt(domElement, AttrBasePoint, NULL_ID_STR);
    formulaAngle = doc->GetParametrString(domElement, AttrAngle, "");
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
        visual->setLineStyle(VAbstractTool::LineStyleToPenStyle(typeLine));
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolEndLine::GetFormulaAngle() const
{
    VFormula fAngle(formulaAngle, getData());
    fAngle.setCheckZero(false);
    fAngle.setToolId(id);
    fAngle.setPostfix(degreeSymbol);
    return fAngle;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolEndLine::SetFormulaAngle(const VFormula &value)
{
    if (value.error() == false)
    {
        formulaAngle = value.GetFormula(FormulaType::FromUser);

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolEndLine::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolEndLine>(show);
}
