/************************************************************************
 **
 **  @file   vtoolalongline.cpp
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

#include "vtoolalongline.h"
#include "../vpatterndb/calculator.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../../../../../dialogs/tools/dialogalongline.h"
#include "../vgeometry/vpointf.h"
#include "../ifc/exception/vexceptionobjecterror.h"
#include "../../../../../visualization/line/vistoolalongline.h"

const QString VToolAlongLine::ToolType = QStringLiteral("alongLine");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolAlongLine constuctor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param formula string with length formula.
 * @param firstPointId id first point of line.
 * @param secondPointId id second point of line.
 * @param typeLine line type. line type.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolAlongLine::VToolAlongLine(VAbstractPattern *doc, VContainer *data, quint32 id, const QString &formula,
                               const quint32 &firstPointId, const quint32 &secondPointId,
                               const QString &typeLine, const QString &lineColor, const Source &typeCreation,
                               QGraphicsItem *parent)
    :VToolLinePoint(doc, data, id, typeLine, lineColor, formula, firstPointId, 0, parent), secondPointId(secondPointId)
{
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFactor set current scale factor of scene.
 * @param factor scene scale factor.
 */
void VToolAlongLine::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events. handle context menu event.
 * @param event context menu event.
 */
//cppcheck-suppress unusedFunction
void VToolAlongLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    try
    {
        ContextMenu<DialogAlongLine>(this, event);
    }
    catch(const VExceptionToolWasDeleted &e)
    {
        Q_UNUSED(e);
        return;//Leave this method immediately!!!
    }
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
void VToolAlongLine::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogAlongLine *dialogTool = qobject_cast<DialogAlongLine*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
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
    typeLine = doc->GetParametrString(domElement, AttrTypeLine, TypeLineLine);
    lineColor = doc->GetParametrString(domElement, AttrLineColor, ColorBlack);
    formulaLength = doc->GetParametrString(domElement, AttrLength, "");
    basePointId = doc->GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
    secondPointId = doc->GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolAlongLine::SetVisualization()
{
    if (vis != nullptr)
    {
        VisToolAlongLine *visual = qobject_cast<VisToolAlongLine *>(vis);
        SCASSERT(visual != nullptr)
        visual->setObject1Id(basePointId);
        visual->setObject2Id(secondPointId);
        visual->setLength(qApp->TrVars()->FormulaToUser(formulaLength));
        visual->setLineStyle(VAbstractTool::LineStyleToPenStyle(typeLine));
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolAlongLine::GetSecondPointId() const
{
    return secondPointId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolAlongLine::SetSecondPointId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        secondPointId = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolAlongLine::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolAlongLine>(show);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolAlongLine::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogAlongLine *dialogTool = qobject_cast<DialogAlongLine*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->SetTypeLine(typeLine);
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
VToolAlongLine* VToolAlongLine::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                       VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogAlongLine *dialogTool = qobject_cast<DialogAlongLine*>(dialog);
    SCASSERT(dialogTool != nullptr);
    QString formula = dialogTool->GetFormula();
    const quint32 firstPointId = dialogTool->GetFirstPointId();
    const quint32 secondPointId = dialogTool->GetSecondPointId();
    const QString typeLine = dialogTool->GetTypeLine();
    const QString lineColor = dialogTool->GetLineColor();
    const QString pointName = dialogTool->getPointName();
    VToolAlongLine *point = Create(0, pointName, typeLine, lineColor, formula, firstPointId, secondPointId,
                                   5, 10, scene, doc, data, Document::FullParse, Source::FromGui);
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
 * @param pointName point name. point name.
 * @param typeLine line type.
 * @param formula string with length formula.
 * @param firstPointId id first point of line.
 * @param secondPointId id second point of line.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 */
VToolAlongLine* VToolAlongLine::Create(const quint32 _id, const QString &pointName, const QString &typeLine,
                                       const QString &lineColor, QString &formula, const quint32 &firstPointId,
                                       const quint32 &secondPointId, const qreal &mx, const qreal &my,
                                       VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data,
                                       const Document &parse, const Source &typeCreation)
{
    const QSharedPointer<VPointF> firstPoint = data->GeometricObject<VPointF>(firstPointId);
    const QSharedPointer<VPointF> secondPoint = data->GeometricObject<VPointF>(secondPointId);
    QLineF line = QLineF(*firstPoint, *secondPoint);

    line.setLength(qApp->toPixel(CheckFormula(_id, formula, data)));

    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject( new VPointF(line.p2(), pointName, mx, my));
        data->AddLine(firstPointId, id);
        data->AddLine(id, secondPointId);
    }
    else
    {
        data->UpdateGObject(id, new VPointF(line.p2(), pointName, mx, my));
        data->AddLine(firstPointId, id);
        data->AddLine(id, secondPointId);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::AlongLine, doc);
    if (parse == Document::FullParse)
    {
        VToolAlongLine *point = new VToolAlongLine(doc, data, id, formula, firstPointId, secondPointId, typeLine,
                                                   lineColor, typeCreation);
        scene->addItem(point);
        InitToolConnections(scene, point);
        doc->AddTool(id, point);
        doc->IncrementReferens(firstPoint->getIdTool());
        doc->IncrementReferens(secondPoint->getIdTool());
        return point;
    }
    return nullptr;
}
