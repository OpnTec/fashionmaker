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
 **  Copyright (C) 2013 Valentina project
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
#include "../../widgets/vmaingraphicsscene.h"
#include "../../container/calculator.h"
#include "../../dialogs/tools/dialogendline.h"
#include "../../dialogs/tools/dialogeditwrongformula.h"
#include "../../geometry/vpointf.h"

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
VToolEndLine::VToolEndLine(VPattern *doc, VContainer *data, const quint32 &id,  const QString &typeLine,
                           const QString &formulaLength, const QString &formulaAngle, const quint32 &basePointId,
                           const Source &typeCreation, QGraphicsItem *parent)
    :VToolLinePoint(doc, data, id, typeLine, formulaLength, basePointId, 0, parent), formulaAngle(formulaAngle)
{
    if (typeCreation == Source::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolEndLine::setDialog()
{
    SCASSERT(dialog != nullptr);
    dialog->setModal(true);
    DialogEndLine *dialogTool = qobject_cast<DialogEndLine*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->setTypeLine(typeLine);
    dialogTool->setFormula(formulaLength);
    dialogTool->setAngle(formulaAngle);
    dialogTool->setBasePointId(basePointId);
    dialogTool->setPointName(p->name());
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
VToolEndLine* VToolEndLine::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogEndLine *dialogTool = qobject_cast<DialogEndLine*>(dialog);
    SCASSERT(dialogTool);
    const QString pointName = dialogTool->getPointName();
    const QString typeLine = dialogTool->getTypeLine();
    QString formulaLength = dialogTool->getFormula();
    QString formulaAngle = dialogTool->getAngle();
    const quint32 basePointId = dialogTool->getBasePointId();

    VToolEndLine *point = nullptr;
    point=Create(0, pointName, typeLine, formulaLength, formulaAngle, basePointId, 5, 10, scene, doc, data,
                 Document::FullParse, Source::FromGui);
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
                                   QString &formulaLength, QString &formulaAngle, const quint32 &basePointId,
                                   const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VPattern *doc,
                                   VContainer *data, const Document &parse, const Source &typeCreation)
{
    const QSharedPointer<VPointF> basePoint = data->GeometricObject<VPointF>(basePointId);
    QLineF line = QLineF(basePoint->toQPointF(), QPointF(basePoint->x()+100, basePoint->y()));

    line.setLength(qApp->toPixel(CheckFormula(_id, formulaLength, data)));
    line.setAngle(CheckFormula(_id, formulaAngle, data));
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
    VDrawTool::AddRecord(id, Tool::EndLine, doc);
    if (parse == Document::FullParse)
    {
        VToolEndLine *point = new VToolEndLine(doc, data, id, typeLine, formulaLength, formulaAngle, basePointId,
                                               typeCreation);
        scene->addItem(point);
        connect(point, &VToolPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolPoint::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, point, &VToolPoint::Disable);
        doc->AddTool(id, point);
        doc->IncrementReferens(basePointId);
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolEndLine::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        typeLine = domElement.attribute(AttrTypeLine, "");
        formulaLength = domElement.attribute(AttrLength, "");
        basePointId = domElement.attribute(AttrBasePoint, "").toUInt();
        formulaAngle = domElement.attribute(AttrAngle, "");
    }
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolEndLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogEndLine>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowContextMenu show context menu.
 * @param event context menu event.
 */
void VToolEndLine::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogEndLine>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolEndLine::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogEndLine *dialogTool = qobject_cast<DialogEndLine*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->getTypeLine());
    doc->SetAttribute(domElement, AttrLength, dialogTool->getFormula());
    doc->SetAttribute(domElement, AttrAngle, dialogTool->getAngle());
    doc->SetAttribute(domElement, AttrBasePoint, QString().setNum(dialogTool->getBasePointId()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolEndLine::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    QSharedPointer<VPointF> point = qSharedPointerDynamicCast<VPointF>(obj);
    SCASSERT(point.isNull() == false);

    doc->SetAttribute(tag, VDomDocument::AttrId, id);
    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrName, point->name());
    doc->SetAttribute(tag, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(tag, AttrMy, qApp->fromPixel(point->my()));

    doc->SetAttribute(tag, AttrTypeLine, typeLine);
    doc->SetAttribute(tag, AttrLength, formulaLength);
    doc->SetAttribute(tag, AttrAngle, formulaAngle);
    doc->SetAttribute(tag, AttrBasePoint, basePointId);
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VToolEndLine::getFormulaAngle() const
{
    VFormula fAngle(formulaAngle, getData());
    fAngle.setCheckZero(false);
    fAngle.setToolId(id);
    fAngle.setPostfix(QStringLiteral("°"));
    return fAngle;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolEndLine::setFormulaAngle(const VFormula &value)
{
    if (value.error() == false)
    {
        formulaAngle = value.getFormula(FormulaType::FromUser);

        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOption(obj);
    }
}
