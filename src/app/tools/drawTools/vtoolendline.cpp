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

const QString VToolEndLine::ToolType = QStringLiteral("endLine");

//---------------------------------------------------------------------------------------------------------------------
VToolEndLine::VToolEndLine(VPattern *doc, VContainer *data, const quint32 &id,  const QString &typeLine,
                           const QString &formula, const qreal &angle, const quint32 &basePointId,
                           const Valentina::Sources &typeCreation, QGraphicsItem *parent)
    :VToolLinePoint(doc, data, id, typeLine, formula, basePointId, angle, parent)
{

    if (typeCreation == Valentina::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolEndLine::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogEndLine *dialogTool = qobject_cast<DialogEndLine*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogTool->setTypeLine(typeLine);
    dialogTool->setFormula(formula);
    dialogTool->setAngle(angle);
    dialogTool->setBasePointId(basePointId, id);
    dialogTool->setPointName(p->name());
}

//---------------------------------------------------------------------------------------------------------------------
VToolEndLine* VToolEndLine::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc,
                          VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogEndLine *dialogTool = qobject_cast<DialogEndLine*>(dialog);
    SCASSERT(dialogTool);
    const QString pointName = dialogTool->getPointName();
    const QString typeLine = dialogTool->getTypeLine();
    QString formula = dialogTool->getFormula();
    const qreal angle = dialogTool->getAngle();
    const quint32 basePointId = dialogTool->getBasePointId();

    VToolEndLine *point = nullptr;
    point=Create(0, pointName, typeLine, formula, angle, basePointId, 5, 10, scene, doc, data, Document::FullParse,
                 Valentina::FromGui);
    if (point != nullptr)
    {
        point->dialog=dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
VToolEndLine* VToolEndLine::Create(const quint32 _id, const QString &pointName, const QString &typeLine,
                                   QString &formula, const qreal &angle, const quint32 &basePointId,
                                   const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VPattern *doc,
                                   VContainer *data, const Document::Documents &parse, const Valentina::Sources &typeCreation)
{
    const VPointF *basePoint = data->GeometricObject<const VPointF *>(basePointId);
    QLineF line = QLineF(basePoint->toQPointF(), QPointF(basePoint->x()+100, basePoint->y()));

    line.setLength(qApp->toPixel(CheckFormula(formula, data)));
    line.setAngle(angle);
    quint32 id = _id;
    if (typeCreation == Valentina::FromGui)
    {
        id = data->AddGObject(new VPointF(line.p2().x(), line.p2().y(), pointName, mx, my));
        data->AddLine(basePointId, id);
    }
    else
    {
        data->UpdateGObject(id, new VPointF(line.p2().x(), line.p2().y(), pointName, mx, my));
        data->AddLine(basePointId, id);
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Valentina::EndLineTool, doc);
    if (parse == Document::FullParse)
    {
        VToolEndLine *point = new VToolEndLine(doc, data, id, typeLine, formula, angle,
                                               basePointId, typeCreation);
        scene->addItem(point);
        connect(point, &VToolPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolPoint::SetFactor);
        doc->AddTool(id, point);
        doc->IncrementReferens(basePointId);
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolEndLine::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        typeLine = domElement.attribute(AttrTypeLine, "");
        formula = domElement.attribute(AttrLength, "");
        basePointId = domElement.attribute(AttrBasePoint, "").toUInt();
        angle = domElement.attribute(AttrAngle, "").toInt();
    }
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolEndLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogEndLine>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolEndLine::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogEndLine>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolEndLine::AddToFile()
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
    doc->SetAttribute(domElement, AttrAngle, angle);
    doc->SetAttribute(domElement, AttrBasePoint, basePointId);

    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolEndLine::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrName, point->name());
        doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
        doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));
        doc->SetAttribute(domElement, AttrTypeLine, typeLine);
        doc->SetAttribute(domElement, AttrLength, formula);
        doc->SetAttribute(domElement, AttrAngle, angle);
        doc->SetAttribute(domElement, AttrBasePoint, basePointId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolEndLine::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogEndLine *dialogTool = qobject_cast<DialogEndLine*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->getTypeLine());
    doc->SetAttribute(domElement, AttrLength, dialogTool->getFormula());
    doc->SetAttribute(domElement, AttrAngle, QString().setNum(dialogTool->getAngle()));
    doc->SetAttribute(domElement, AttrBasePoint, QString().setNum(dialogTool->getBasePointId()));
}
