/************************************************************************
 **
 **  @file   vtoolnormal.cpp
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

#include "vtoolnormal.h"
#include "../../container/calculator.h"
#include "../../dialogs/dialognormal.h"

const QString VToolNormal::ToolType = QStringLiteral("normal");

VToolNormal::VToolNormal(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &typeLine,
                         const QString &formula, const qreal &angle, const qint64 &firstPointId,
                         const qint64 &secondPointId, const Tool::Sources &typeCreation, QGraphicsItem *parent)
    :VToolLinePoint(doc, data, id, typeLine, formula, firstPointId, angle, parent),
    secondPointId(secondPointId)
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

void VToolNormal::setDialog()
{
    Q_CHECK_PTR(dialog);
    DialogNormal *dialogTool = qobject_cast<DialogNormal*>(dialog);
    Q_CHECK_PTR(dialogTool);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogTool->setTypeLine(typeLine);
    dialogTool->setFormula(formula);
    dialogTool->setAngle(angle);
    dialogTool->setFirstPointId(basePointId, id);
    dialogTool->setSecondPointId(secondPointId, id);
    dialogTool->setPointName(p->name());
}

void VToolNormal::Create(DialogTool *dialog, VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data)
{
    Q_CHECK_PTR(dialog);
    DialogNormal *dialogTool = qobject_cast<DialogNormal*>(dialog);
    Q_CHECK_PTR(dialogTool);
    QString formula = dialogTool->getFormula();
    qint64 firstPointId = dialogTool->getFirstPointId();
    qint64 secondPointId = dialogTool->getSecondPointId();
    QString typeLine = dialogTool->getTypeLine();
    QString pointName = dialogTool->getPointName();
    qreal angle = dialogTool->getAngle();
    Create(0, formula, firstPointId, secondPointId, typeLine, pointName, angle, 5, 10, scene, doc, data,
           Document::FullParse, Tool::FromGui);
}

void VToolNormal::Create(const qint64 _id, const QString &formula, const qint64 &firstPointId,
                         const qint64 &secondPointId, const QString &typeLine, const QString &pointName,
                         const qreal angle, const qreal &mx, const qreal &my, VMainGraphicsScene *scene,
                         VDomDocument *doc, VContainer *data, const Document::Documents &parse,
                         const Tool::Sources &typeCreation)
{
    const VPointF *firstPoint = data->GeometricObject<const VPointF *>(firstPointId);
    const VPointF *secondPoint = data->GeometricObject<const VPointF *>(secondPointId);
    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if (errorMsg.isEmpty())
    {
        QPointF fPoint = VToolNormal::FindPoint(firstPoint->toQPointF(), secondPoint->toQPointF(),
                                                toPixel(result), angle);
        qint64 id = _id;
        if (typeCreation == Tool::FromGui)
        {
            id = data->AddGObject(new VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            data->AddLine(firstPointId, id);
        }
        else
        {
            data->UpdateGObject(id, new VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            data->AddLine(firstPointId, id);
            if (parse != Document::FullParse)
            {
                doc->UpdateToolData(id, data);
            }
        }
        VDrawTool::AddRecord(id, Tool::NormalTool, doc);
        if (parse == Document::FullParse)
        {
            VToolNormal *point = new VToolNormal(doc, data, id, typeLine, formula, angle,
                                                 firstPointId, secondPointId, typeCreation);
            scene->addItem(point);
            connect(point, &VToolNormal::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolNormal::SetFactor);
            doc->AddTool(id, point);
            doc->IncrementReferens(firstPointId);
            doc->IncrementReferens(secondPointId);
        }
    }
}

QPointF VToolNormal::FindPoint(const QPointF &firstPoint, const QPointF &secondPoint, const qreal &length,
                               const qreal &angle)
{
    QLineF line(firstPoint, secondPoint);
    QLineF normal = line.normalVector();
    normal.setAngle(normal.angle()+angle);
    normal.setLength(length);
    return normal.p2();
}

void VToolNormal::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        typeLine = domElement.attribute(AttrTypeLine, "");
        formula = domElement.attribute(AttrLength, "");
        basePointId = domElement.attribute(AttrFirstPoint, "").toLongLong();
        secondPointId = domElement.attribute(AttrSecondPoint, "").toLongLong();
        angle = domElement.attribute(AttrAngle, "").toDouble();
    }
    RefreshGeometry();
}

void VToolNormal::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

void VToolNormal::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogNormal>(this, event);
}

void VToolNormal::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogNormal>(this, event);
}

void VToolNormal::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    SetAttribute(domElement, AttrId, id);
    SetAttribute(domElement, AttrType, ToolType);
    SetAttribute(domElement, AttrName, point->name());
    SetAttribute(domElement, AttrMx, toMM(point->mx()));
    SetAttribute(domElement, AttrMy, toMM(point->my()));

    SetAttribute(domElement, AttrTypeLine, typeLine);
    SetAttribute(domElement, AttrLength, formula);
    SetAttribute(domElement, AttrAngle, angle);
    SetAttribute(domElement, AttrFirstPoint, basePointId);
    SetAttribute(domElement, AttrSecondPoint, secondPointId);

    AddToCalculation(domElement);
}

void VToolNormal::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        SetAttribute(domElement, AttrName, point->name());
        SetAttribute(domElement, AttrMx, toMM(point->mx()));
        SetAttribute(domElement, AttrMy, toMM(point->my()));
        SetAttribute(domElement, AttrTypeLine, typeLine);
        SetAttribute(domElement, AttrLength, formula);
        SetAttribute(domElement, AttrAngle, angle);
        SetAttribute(domElement, AttrFirstPoint, basePointId);
        SetAttribute(domElement, AttrSecondPoint, secondPointId);
    }
}

void VToolNormal::RemoveReferens()
{
    doc->DecrementReferens(secondPointId);
    VToolLinePoint::RemoveReferens();
}

void VToolNormal::SaveDialog(QDomElement &domElement)
{
    Q_CHECK_PTR(dialog);
    DialogNormal *dialogTool = qobject_cast<DialogNormal*>(dialog);
    Q_CHECK_PTR(dialogTool);
    SetAttribute(domElement, AttrName, dialogTool->getPointName());
    SetAttribute(domElement, AttrTypeLine, dialogTool->getTypeLine());
    SetAttribute(domElement, AttrLength, dialogTool->getFormula());
    SetAttribute(domElement, AttrAngle, QString().setNum(dialogTool->getAngle()));
    SetAttribute(domElement, AttrFirstPoint, QString().setNum(dialogTool->getFirstPointId()));
    SetAttribute(domElement, AttrSecondPoint, QString().setNum(dialogTool->getSecondPointId()));
}
