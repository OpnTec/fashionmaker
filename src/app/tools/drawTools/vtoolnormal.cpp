/************************************************************************
 **
 **  @file   vtoolnormal.cpp
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

#include "vtoolnormal.h"
#include "../../container/calculator.h"
#include "../../dialogs/tools/dialognormal.h"
#include "../../libs/vgeometry/vpointf.h"
#include "../../visualization/vistoolnormal.h"

const QString VToolNormal::ToolType = QStringLiteral("normal");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolNormal constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param typeLine line type.
 * @param formula string with formula normal length.
 * @param angle additional angle.
 * @param firstPointId id first line point.
 * @param secondPointId id second line point.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolNormal::VToolNormal(VPattern *doc, VContainer *data, const quint32 &id, const QString &typeLine,
                         const QString &lineColor, const QString &formula, const qreal &angle,
                         const quint32 &firstPointId, const quint32 &secondPointId, const Source &typeCreation,
                         QGraphicsItem *parent)
    :VToolLinePoint(doc, data, id, typeLine, lineColor, formula, firstPointId, angle, parent),
    secondPointId(secondPointId)
{

    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolNormal::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogNormal *dialogTool = qobject_cast<DialogNormal*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->SetTypeLine(typeLine);
    dialogTool->SetFormula(formulaLength);
    dialogTool->SetAngle(angle);
    dialogTool->SetFirstPointId(basePointId);
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
 */
VToolNormal* VToolNormal::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogNormal *dialogTool = qobject_cast<DialogNormal*>(dialog);
    SCASSERT(dialogTool != nullptr);
    QString formula = dialogTool->GetFormula();
    const quint32 firstPointId = dialogTool->GetFirstPointId();
    const quint32 secondPointId = dialogTool->GetSecondPointId();
    const QString typeLine = dialogTool->GetTypeLine();
    const QString lineColor = dialogTool->GetLineColor();
    const QString pointName = dialogTool->getPointName();
    const qreal angle = dialogTool->GetAngle();
    VToolNormal *point = nullptr;
    point=Create(0, formula, firstPointId, secondPointId, typeLine, lineColor, pointName, angle, 5, 10, scene, doc,
                 data, Document::FullParse, Source::FromGui);
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
 * @param formula string with formula normal length.
 * @param firstPointId id first line point.
 * @param secondPointId id second line point.
 * @param typeLine line type.
 * @param pointName point name.
 * @param angle additional angle.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 */
VToolNormal* VToolNormal::Create(const quint32 _id, QString &formula, const quint32 &firstPointId,
                                 const quint32 &secondPointId, const QString &typeLine, const QString &lineColor,
                                 const QString &pointName, const qreal angle, const qreal &mx, const qreal &my,
                                 VMainGraphicsScene *scene, VPattern *doc, VContainer *data, const Document &parse,
                                 const Source &typeCreation)
{
    const QSharedPointer<VPointF> firstPoint = data->GeometricObject<VPointF>(firstPointId);
    const QSharedPointer<VPointF> secondPoint = data->GeometricObject<VPointF>(secondPointId);

    const qreal result = CheckFormula(_id, formula, data);

    QPointF fPoint = VToolNormal::FindPoint(firstPoint->toQPointF(), secondPoint->toQPointF(),
                                            qApp->toPixel(result), angle);
    quint32 id = _id;
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(new VPointF(fPoint, pointName, mx, my));
        data->AddLine(firstPointId, id);
    }
    else
    {
        data->UpdateGObject(id, new VPointF(fPoint, pointName, mx, my));
        data->AddLine(firstPointId, id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::Normal, doc);
    if (parse == Document::FullParse)
    {
        VToolNormal *point = new VToolNormal(doc, data, id, typeLine, lineColor, formula, angle, firstPointId,
                                             secondPointId, typeCreation);
        scene->addItem(point);
        connect(point, &VToolNormal::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolNormal::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, point, &VToolNormal::Disable);
        connect(scene, &VMainGraphicsScene::EnableToolMove, point, &VToolNormal::EnableToolMove);
        doc->AddTool(id, point);
        doc->IncrementReferens(firstPointId);
        doc->IncrementReferens(secondPointId);
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FindPoint return normal point.
 * @param firstPoint first line point.
 * @param secondPoint second line point.
 * @param length normal length.
 * @param angle additional angle.
 * @return normal point.
 */
QPointF VToolNormal::FindPoint(const QPointF &firstPoint, const QPointF &secondPoint, const qreal &length,
                               const qreal &angle)
{
    QLineF line(firstPoint, secondPoint);
    QLineF normal = line.normalVector();
    normal.setAngle(normal.angle()+angle);
    normal.setLength(length);
    return normal.p2();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolNormal::FullUpdateFromFile()
{
    ReadAttributes();
    RefreshGeometry();
    SetVisualization();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFactor set current scale factor of scene.
 * @param factor scene scale factor.
 */
void VToolNormal::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolNormal::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogNormal>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolNormal::RemoveReferens()
{
    doc->DecrementReferens(secondPointId);
    VToolLinePoint::RemoveReferens();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolNormal::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogNormal *dialogTool = qobject_cast<DialogNormal*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->GetTypeLine());
    doc->SetAttribute(domElement, AttrLineColor, dialogTool->GetLineColor());
    doc->SetAttribute(domElement, AttrLength, dialogTool->GetFormula());
    doc->SetAttribute(domElement, AttrAngle, QString().setNum(dialogTool->GetAngle()));
    doc->SetAttribute(domElement, AttrFirstPoint, QString().setNum(dialogTool->GetFirstPointId()));
    doc->SetAttribute(domElement, AttrSecondPoint, QString().setNum(dialogTool->GetSecondPointId()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolNormal::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VToolLinePoint::SaveOptions(tag, obj);

    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrLength, formulaLength);
    doc->SetAttribute(tag, AttrAngle, angle);
    doc->SetAttribute(tag, AttrFirstPoint, basePointId);
    doc->SetAttribute(tag, AttrSecondPoint, secondPointId);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolNormal::ReadToolAttributes(const QDomElement &domElement)
{
    typeLine = doc->GetParametrString(domElement, AttrTypeLine, TypeLineLine);
    lineColor = doc->GetParametrString(domElement, AttrLineColor, ColorBlack);
    formulaLength = doc->GetParametrString(domElement, AttrLength, "");
    basePointId = doc->GetParametrUInt(domElement, AttrFirstPoint, NULL_ID_STR);
    secondPointId = doc->GetParametrUInt(domElement, AttrSecondPoint, NULL_ID_STR);
    angle = doc->GetParametrDouble(domElement, AttrAngle, "0");
}

//---------------------------------------------------------------------------------------------------------------------
void VToolNormal::SetVisualization()
{
    if (vis != nullptr)
    {
        VisToolNormal *visual = qobject_cast<VisToolNormal *>(vis);
        SCASSERT(visual != nullptr);

        visual->setPoint1Id(basePointId);
        visual->setPoint2Id(secondPointId);
        visual->setLength(qApp->FormulaToUser(formulaLength));
        visual->SetAngle(angle);
        visual->setLineStyle(VAbstractTool::LineStyleToPenStyle(typeLine));
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VToolNormal::GetSecondPointId() const
{
    return secondPointId;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolNormal::SetSecondPointId(const quint32 &value)
{
    if (value != NULL_ID)
    {
        secondPointId = value;

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolNormal::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolNormal>(show);
}
