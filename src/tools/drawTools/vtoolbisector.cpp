/************************************************************************
 **
 **  @file   vtoolbisector.cpp
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

#include "vtoolbisector.h"
#include "../../container/calculator.h"
#include "../../dialogs/tools/dialogbisector.h"

const QString VToolBisector::ToolType = QStringLiteral("bisector");

VToolBisector::VToolBisector(VDomDocument *doc, VContainer *data, const qint64 &id,
                             const QString &typeLine, const QString &formula, const qint64 &firstPointId,
                             const qint64 &secondPointId, const qint64 &thirdPointId, const Tool::Sources &typeCreation,
                             QGraphicsItem *parent)
    :VToolLinePoint(doc, data, id, typeLine, formula, secondPointId, 0, parent), firstPointId(0), thirdPointId(0)
{
    this->firstPointId = firstPointId;
    this->thirdPointId = thirdPointId;

    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

QPointF VToolBisector::FindPoint(const QPointF &firstPoint, const QPointF &secondPoint,
                                 const QPointF &thirdPoint, const qreal &length)
{
    QLineF line1(secondPoint, firstPoint);
    QLineF line2(secondPoint, thirdPoint);
    qreal angle = line1.angleTo(line2);
    if (angle>180)
    {
        angle = 360 - angle;
        line1.setAngle(line1.angle()-angle/2);
    }
    else
    {
        line1.setAngle(line1.angle()+angle/2);
    }
    line1.setLength(length);
    return line1.p2();
}

void VToolBisector::setDialog()
{
    Q_CHECK_PTR(dialog);
    DialogBisector *dialogTool = qobject_cast<DialogBisector*>(dialog);
    Q_CHECK_PTR(dialogTool);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogTool->setTypeLine(typeLine);
    dialogTool->setFormula(formula);
    dialogTool->setFirstPointId(firstPointId, id);
    dialogTool->setSecondPointId(basePointId, id);
    dialogTool->setThirdPointId(thirdPointId, id);
    dialogTool->setPointName(p->name());
}

void VToolBisector::Create(DialogTool *dialog, VMainGraphicsScene *scene, VDomDocument *doc,
                           VContainer *data)
{
    Q_CHECK_PTR(dialog);
    DialogBisector *dialogTool = qobject_cast<DialogBisector*>(dialog);
    Q_CHECK_PTR(dialogTool);
    QString formula = dialogTool->getFormula();
    qint64 firstPointId = dialogTool->getFirstPointId();
    qint64 secondPointId = dialogTool->getSecondPointId();
    qint64 thirdPointId = dialogTool->getThirdPointId();
    QString typeLine = dialogTool->getTypeLine();
    QString pointName = dialogTool->getPointName();
    Create(0, formula, firstPointId, secondPointId, thirdPointId, typeLine, pointName, 5, 10, scene, doc, data,
           Document::FullParse, Tool::FromGui);
}

void VToolBisector::Create(const qint64 _id, const QString &formula, const qint64 &firstPointId,
                           const qint64 &secondPointId, const qint64 &thirdPointId, const QString &typeLine,
                           const QString &pointName, const qreal &mx, const qreal &my,
                           VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data,
                           const Document::Documents &parse, const Tool::Sources &typeCreation)
{
    const VPointF *firstPoint = data->GeometricObject<const VPointF *>(firstPointId);
    const VPointF *secondPoint = data->GeometricObject<const VPointF *>(secondPointId);
    const VPointF *thirdPoint = data->GeometricObject<const VPointF *>(thirdPointId);

    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if (errorMsg.isEmpty())
    {
        QPointF fPoint = VToolBisector::FindPoint(firstPoint->toQPointF(), secondPoint->toQPointF(),
                                                  thirdPoint->toQPointF(), toPixel(result));
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
        VDrawTool::AddRecord(id, Tool::BisectorTool, doc);
        if (parse == Document::FullParse)
        {
            VToolBisector *point = new VToolBisector(doc, data, id, typeLine, formula, firstPointId, secondPointId,
                                                     thirdPointId, typeCreation);
            scene->addItem(point);
            connect(point, &VToolBisector::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolBisector::SetFactor);
            doc->AddTool(id, point);
            doc->IncrementReferens(firstPointId);
            doc->IncrementReferens(secondPointId);
            doc->IncrementReferens(thirdPointId);
        }
    }
}

void VToolBisector::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        typeLine = domElement.attribute(AttrTypeLine, "");
        formula = domElement.attribute(AttrLength, "");
        firstPointId = domElement.attribute(AttrFirstPoint, "").toLongLong();
        basePointId = domElement.attribute(AttrSecondPoint, "").toLongLong();
        thirdPointId = domElement.attribute(AttrThirdPoint, "").toLongLong();
    }
    RefreshGeometry();
}

void VToolBisector::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

void VToolBisector::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogBisector>(this, event);
}

void VToolBisector::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogBisector>(this, event);
}

void VToolBisector::AddToFile()
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
    SetAttribute(domElement, AttrFirstPoint, firstPointId);
    SetAttribute(domElement, AttrSecondPoint, basePointId);
    SetAttribute(domElement, AttrThirdPoint, thirdPointId);

    AddToCalculation(domElement);
}

void VToolBisector::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        SetAttribute(domElement, AttrMx, toMM(point->mx()));
        SetAttribute(domElement, AttrMy, toMM(point->my()));
        SetAttribute(domElement, AttrName, point->name());
        SetAttribute(domElement, AttrTypeLine, typeLine);
        SetAttribute(domElement, AttrLength, formula);
        SetAttribute(domElement, AttrFirstPoint, firstPointId);
        SetAttribute(domElement, AttrSecondPoint, basePointId);
        SetAttribute(domElement, AttrThirdPoint, thirdPointId);
    }
}

void VToolBisector::RemoveReferens()
{
    doc->DecrementReferens(firstPointId);
    doc->DecrementReferens(thirdPointId);
    VToolLinePoint::RemoveReferens();
}

void VToolBisector::SaveDialog(QDomElement &domElement)
{
    Q_CHECK_PTR(dialog);
    DialogBisector *dialogTool = qobject_cast<DialogBisector*>(dialog);
    Q_CHECK_PTR(dialogTool);
    SetAttribute(domElement, AttrName, dialogTool->getPointName());
    SetAttribute(domElement, AttrTypeLine, dialogTool->getTypeLine());
    SetAttribute(domElement, AttrLength, dialogTool->getFormula());
    SetAttribute(domElement, AttrFirstPoint, QString().setNum(dialogTool->getFirstPointId()));
    SetAttribute(domElement, AttrSecondPoint, QString().setNum(dialogTool->getSecondPointId()));
    SetAttribute(domElement, AttrThirdPoint, QString().setNum(dialogTool->getThirdPointId()));
}
