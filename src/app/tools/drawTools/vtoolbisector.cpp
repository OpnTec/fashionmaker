/************************************************************************
 **
 **  @file   vtoolbisector.cpp
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

#include "vtoolbisector.h"
#include "../../container/calculator.h"
#include "../../dialogs/tools/dialogbisector.h"
#include "../../geometry/vpointf.h"

const QString VToolBisector::ToolType = QStringLiteral("bisector");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolBisector constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param typeLine line type.
 * @param formula string with formula length of bisector.
 * @param firstPointId id first point of angle.
 * @param secondPointId id second point of angle.
 * @param thirdPointId id third point of angle.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolBisector::VToolBisector(VPattern *doc, VContainer *data, const quint32 &id, const QString &typeLine,
                             const QString &formula, const quint32 &firstPointId, const quint32 &secondPointId,
                             const quint32 &thirdPointId, const Source &typeCreation, QGraphicsItem *parent)
    :VToolLinePoint(doc, data, id, typeLine, formula, secondPointId, 0, parent), firstPointId(NULL_ID),
      thirdPointId(NULL_ID)
{
    this->firstPointId = firstPointId;
    this->thirdPointId = thirdPointId;

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
    SCASSERT(dialog != nullptr);
    DialogBisector *dialogTool = qobject_cast<DialogBisector*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VPointF> p = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->setTypeLine(typeLine);
    dialogTool->setFormula(formulaLength);
    dialogTool->setFirstPointId(firstPointId);
    dialogTool->setSecondPointId(basePointId);
    dialogTool->setThirdPointId(thirdPointId);
    dialogTool->setPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool form GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 */
VToolBisector* VToolBisector::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc,
                           VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogBisector *dialogTool = qobject_cast<DialogBisector*>(dialog);
    SCASSERT(dialogTool != nullptr);
    QString formula = dialogTool->getFormula();
    const quint32 firstPointId = dialogTool->getFirstPointId();
    const quint32 secondPointId = dialogTool->getSecondPointId();
    const quint32 thirdPointId = dialogTool->getThirdPointId();
    const QString typeLine = dialogTool->getTypeLine();
    const QString pointName = dialogTool->getPointName();
    VToolBisector *point = nullptr;
    point=Create(0, formula, firstPointId, secondPointId, thirdPointId, typeLine, pointName, 5, 10, scene, doc, data,
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
 * @param formula string with formula.
 * @param firstPointId id first point of angle.
 * @param secondPointId id second point of angle.
 * @param thirdPointId id third point of angle.
 * @param typeLine line type.
 * @param pointName point name.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 */
VToolBisector* VToolBisector::Create(const quint32 _id, QString &formula, const quint32 &firstPointId,
                           const quint32 &secondPointId, const quint32 &thirdPointId, const QString &typeLine,
                           const QString &pointName, const qreal &mx, const qreal &my,
                           VMainGraphicsScene *scene, VPattern *doc, VContainer *data,
                           const Document &parse, const Source &typeCreation)
{
    const QSharedPointer<VPointF> firstPoint = data->GeometricObject<VPointF>(firstPointId);
    const QSharedPointer<VPointF> secondPoint = data->GeometricObject<VPointF>(secondPointId);
    const QSharedPointer<VPointF> thirdPoint = data->GeometricObject<VPointF>(thirdPointId);

    const qreal result = CheckFormula(_id, formula, data);

    QPointF fPoint = VToolBisector::FindPoint(firstPoint->toQPointF(), secondPoint->toQPointF(),
                                              thirdPoint->toQPointF(), qApp->toPixel(result));
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
    VDrawTool::AddRecord(id, Tool::Bisector, doc);
    if (parse == Document::FullParse)
    {
        VToolBisector *point = new VToolBisector(doc, data, id, typeLine, formula, firstPointId, secondPointId,
                                                 thirdPointId, typeCreation);
        scene->addItem(point);
        connect(point, &VToolPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolBisector::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, point, &VToolPoint::Disable);
        doc->AddTool(id, point);
        doc->IncrementReferens(firstPointId);
        doc->IncrementReferens(secondPointId);
        doc->IncrementReferens(thirdPointId);
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolBisector::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        typeLine = domElement.attribute(AttrTypeLine, "");
        formulaLength = domElement.attribute(AttrLength, "");
        firstPointId = domElement.attribute(AttrFirstPoint, "").toUInt();
        basePointId = domElement.attribute(AttrSecondPoint, "").toUInt();
        thirdPointId = domElement.attribute(AttrThirdPoint, "").toUInt();
    }
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFactor set current scale factor of scene.
 * @param factor scene scale factor.
 */
void VToolBisector::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowContextMenu show context menu.
 * @param event context menu event.
 */
void VToolBisector::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogBisector>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolBisector::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogBisector>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VToolBisector::AddToFile()
{
    const QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrName, point->name());
    doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));

    doc->SetAttribute(domElement, AttrTypeLine, typeLine);
    doc->SetAttribute(domElement, AttrLength, formulaLength);
    doc->SetAttribute(domElement, AttrFirstPoint, firstPointId);
    doc->SetAttribute(domElement, AttrSecondPoint, basePointId);
    doc->SetAttribute(domElement, AttrThirdPoint, thirdPointId);

    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
 */
void VToolBisector::RefreshDataInFile()
{
    const QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
        doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));
        doc->SetAttribute(domElement, AttrName, point->name());
        doc->SetAttribute(domElement, AttrTypeLine, typeLine);
        doc->SetAttribute(domElement, AttrLength, formulaLength);
        doc->SetAttribute(domElement, AttrFirstPoint, firstPointId);
        doc->SetAttribute(domElement, AttrSecondPoint, basePointId);
        doc->SetAttribute(domElement, AttrThirdPoint, thirdPointId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveReferens decrement value of reference.
 */
void VToolBisector::RemoveReferens()
{
    doc->DecrementReferens(firstPointId);
    doc->DecrementReferens(thirdPointId);
    VToolLinePoint::RemoveReferens();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolBisector::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogBisector *dialogTool = qobject_cast<DialogBisector*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->getTypeLine());
    doc->SetAttribute(domElement, AttrLength, dialogTool->getFormula());
    doc->SetAttribute(domElement, AttrFirstPoint, QString().setNum(dialogTool->getFirstPointId()));
    doc->SetAttribute(domElement, AttrSecondPoint, QString().setNum(dialogTool->getSecondPointId()));
    doc->SetAttribute(domElement, AttrThirdPoint, QString().setNum(dialogTool->getThirdPointId()));
}
