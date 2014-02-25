/************************************************************************
 **
 **  @file   vtoolalongline.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
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

const QString VToolAlongLine::ToolType = QStringLiteral("alongLine");

VToolAlongLine::VToolAlongLine(VPattern *doc, VContainer *data, quint32 id, const QString &formula,
                               const quint32 &firstPointId, const quint32 &secondPointId,
                               const QString &typeLine, const Tool::Sources &typeCreation,
                               QGraphicsItem *parent)
    :VToolLinePoint(doc, data, id, typeLine, formula, firstPointId, 0, parent), secondPointId(secondPointId)
{

    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

void VToolAlongLine::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        typeLine = domElement.attribute(AttrTypeLine, "");
        formula = domElement.attribute(AttrLength, "");
        basePointId = domElement.attribute(AttrFirstPoint, "").toLongLong();
        secondPointId = domElement.attribute(AttrSecondPoint, "").toLongLong();
    }
    RefreshGeometry();
}

void VToolAlongLine::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

//cppcheck-suppress unusedFunction
void VToolAlongLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogAlongLine>(this, event);
}

void VToolAlongLine::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogAlongLine>(this, event);
}

void VToolAlongLine::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrName, point->name());
    doc->SetAttribute(domElement, AttrMx, toMM(point->mx()));
    doc->SetAttribute(domElement, AttrMy, toMM(point->my()));

    doc->SetAttribute(domElement, AttrTypeLine, typeLine);
    doc->SetAttribute(domElement, AttrLength, formula);
    doc->SetAttribute(domElement, AttrFirstPoint, basePointId);
    doc->SetAttribute(domElement, AttrSecondPoint, secondPointId);

    AddToCalculation(domElement);
}

void VToolAlongLine::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrMx, toMM(point->mx()));
        doc->SetAttribute(domElement, AttrMy, toMM(point->my()));
        doc->SetAttribute(domElement, AttrName, point->name());
        doc->SetAttribute(domElement, AttrTypeLine, typeLine);
        doc->SetAttribute(domElement, AttrLength, formula);
        doc->SetAttribute(domElement, AttrFirstPoint, basePointId);
        doc->SetAttribute(domElement, AttrSecondPoint, secondPointId);
    }
}

void VToolAlongLine::RemoveReferens()
{
    doc->DecrementReferens(secondPointId);
    VToolLinePoint::RemoveReferens();
}

void VToolAlongLine::SaveDialog(QDomElement &domElement)
{
    Q_CHECK_PTR(dialog);
    DialogAlongLine *dialogTool = qobject_cast<DialogAlongLine*>(dialog);
    Q_CHECK_PTR(dialogTool);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrTypeLine, dialogTool->getTypeLine());
    doc->SetAttribute(domElement, AttrLength, dialogTool->getFormula());
    doc->SetAttribute(domElement, AttrFirstPoint, dialogTool->getFirstPointId());
    doc->SetAttribute(domElement, AttrSecondPoint, dialogTool->getSecondPointId());
}

void VToolAlongLine::setDialog()
{
    Q_CHECK_PTR(dialog);
    DialogAlongLine *dialogTool = qobject_cast<DialogAlongLine*>(dialog);
    Q_CHECK_PTR(dialogTool);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogTool->setTypeLine(typeLine);
    dialogTool->setFormula(formula);
    dialogTool->setFirstPointId(basePointId, id);
    dialogTool->setSecondPointId(secondPointId, id);
    dialogTool->setPointName(p->name());
}

void VToolAlongLine::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data)
{
    Q_CHECK_PTR(dialog);
    DialogAlongLine *dialogTool = qobject_cast<DialogAlongLine*>(dialog);
    Q_CHECK_PTR(dialogTool);
    QString formula = dialogTool->getFormula();
    quint32 firstPointId = dialogTool->getFirstPointId();
    quint32 secondPointId = dialogTool->getSecondPointId();
    QString typeLine = dialogTool->getTypeLine();
    QString pointName = dialogTool->getPointName();
    Create(0, pointName, typeLine, formula, firstPointId, secondPointId, 5, 10, scene, doc, data,
           Document::FullParse, Tool::FromGui);
}

void VToolAlongLine::Create(const quint32 _id, const QString &pointName, const QString &typeLine,
                            const QString &formula, const quint32 &firstPointId, const quint32 &secondPointId,
                            const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VPattern *doc,
                            VContainer *data, const Document::Documents &parse, const Tool::Sources &typeCreation)
{
    const VPointF *firstPoint = data->GeometricObject<const VPointF *>(firstPointId);
    const VPointF *secondPoint = data->GeometricObject<const VPointF *>(secondPointId);
    QLineF line = QLineF(firstPoint->toQPointF(), secondPoint->toQPointF());
    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if (errorMsg.isEmpty())
    {
        line.setLength(toPixel(result));
        quint32 id = _id;
        if (typeCreation == Tool::FromGui)
        {
            id = data->AddGObject( new VPointF(line.p2().x(), line.p2().y(), pointName, mx, my));
            data->AddLine(firstPointId, id);
            data->AddLine(id, secondPointId);
        }
        else
        {
            data->UpdateGObject(id, new VPointF(line.p2().x(), line.p2().y(), pointName, mx, my));
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
            doc->AddTool(id, point);
            doc->IncrementReferens(firstPointId);
            doc->IncrementReferens(secondPointId);
        }
    }
}
