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

#include "vtoolalongline.h"
#include "../../container/calculator.h"
#include "../../dialogs/tools/dialogalongline.h"
#include "../../geometry/vpointf.h"
#include "exception/vexceptionobjecterror.h"

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
VToolAlongLine::VToolAlongLine(VPattern *doc, VContainer *data, quint32 id, const QString &formula,
                               const quint32 &firstPointId, const quint32 &secondPointId,
                               const QString &typeLine, const Source &typeCreation,
                               QGraphicsItem *parent)
    :VToolLinePoint(doc, data, id, typeLine, formula, firstPointId, 0, parent), secondPointId(secondPointId)
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
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolAlongLine::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        typeLine = domElement.attribute(AttrTypeLine, "");
        formula = domElement.attribute(AttrLength, "");
        basePointId = domElement.attribute(AttrFirstPoint, "").toUInt();
        secondPointId = domElement.attribute(AttrSecondPoint, "").toUInt();
    }
    RefreshGeometry();
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
    ContextMenu<DialogAlongLine>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowContextMenu handle context menu event.
 * @param event context menu event.
 */
void VToolAlongLine::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogAlongLine>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VToolAlongLine::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrName, point->name());
    doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));

    doc->SetAttribute(domElement, AttrTypeLine, typeLine);
    doc->SetAttribute(domElement, AttrLength, formula);
    doc->SetAttribute(domElement, AttrFirstPoint, basePointId);
    doc->SetAttribute(domElement, AttrSecondPoint, secondPointId);

    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
 */
void VToolAlongLine::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
        doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));
        doc->SetAttribute(domElement, AttrName, point->name());
        doc->SetAttribute(domElement, AttrTypeLine, typeLine);
        doc->SetAttribute(domElement, AttrLength, formula);
        doc->SetAttribute(domElement, AttrFirstPoint, basePointId);
        doc->SetAttribute(domElement, AttrSecondPoint, secondPointId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolAlongLine::RemoveReferens()
{
    doc->DecrementReferens(secondPointId);
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
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->getTypeLine());
    doc->SetAttribute(domElement, AttrLength, dialogTool->getFormula());
    doc->SetAttribute(domElement, AttrFirstPoint, dialogTool->getFirstPointId());
    doc->SetAttribute(domElement, AttrSecondPoint, dialogTool->getSecondPointId());
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
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogTool->setTypeLine(typeLine);
    dialogTool->setFormula(formula);
    dialogTool->setFirstPointId(basePointId, id);
    dialogTool->setSecondPointId(secondPointId, id);
    dialogTool->setPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool form GUI.
 * @param dialog dialog options.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 */
VToolAlongLine* VToolAlongLine::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogAlongLine *dialogTool = qobject_cast<DialogAlongLine*>(dialog);
    SCASSERT(dialogTool != nullptr);
    QString formula = dialogTool->getFormula();
    const quint32 firstPointId = dialogTool->getFirstPointId();
    const quint32 secondPointId = dialogTool->getSecondPointId();
    const QString typeLine = dialogTool->getTypeLine();
    const QString pointName = dialogTool->getPointName();
    VToolAlongLine *point=nullptr;
    point = Create(0, pointName, typeLine, formula, firstPointId, secondPointId, 5, 10, scene, doc, data,
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
                                       QString &formula, const quint32 &firstPointId, const quint32 &secondPointId,
                                       const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VPattern *doc,
                                       VContainer *data, const Document &parse, const Source &typeCreation)
{
    const VPointF *firstPoint = data->GeometricObject<const VPointF *>(firstPointId);
    const VPointF *secondPoint = data->GeometricObject<const VPointF *>(secondPointId);
    QLineF line = QLineF(firstPoint->toQPointF(), secondPoint->toQPointF());

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
    VDrawTool::AddRecord(id, Tool::AlongLineTool, doc);
    if (parse == Document::FullParse)
    {
        VToolAlongLine *point = new VToolAlongLine(doc, data, id, formula, firstPointId,
                                                   secondPointId, typeLine, typeCreation);
        scene->addItem(point);
        connect(point, &VToolAlongLine::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolAlongLine::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, point, &VToolAlongLine::Disable);
        doc->AddTool(id, point);
        doc->IncrementReferens(firstPointId);
        doc->IncrementReferens(secondPointId);
        return point;
    }
    return nullptr;
}
