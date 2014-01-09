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

const QString VToolAlongLine::ToolType = QStringLiteral("alongLine");

VToolAlongLine::VToolAlongLine(VDomDocument *doc, VContainer *data, qint64 id, const QString &formula,
                               const qint64 &firstPointId, const qint64 &secondPointId,
                               const QString &typeLine, const Tool::Sources &typeCreation,
                               QGraphicsItem *parent)
    :VToolLinePoint(doc, data, id, typeLine, formula, firstPointId, 0, parent), secondPointId(secondPointId),
    dialogAlongLine(QSharedPointer<DialogAlongLine>())
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

void VToolAlongLine::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            domElement.setAttribute(AttrName, dialogAlongLine->getPointName());
            domElement.setAttribute(AttrTypeLine, dialogAlongLine->getTypeLine());
            domElement.setAttribute(AttrLength, dialogAlongLine->getFormula());
            domElement.setAttribute(AttrFirstPoint, QString().setNum(dialogAlongLine->getFirstPointId()));
            domElement.setAttribute(AttrSecondPoint, QString().setNum(dialogAlongLine->getSecondPointId()));
            emit FullUpdateTree();
        }
    }
    dialogAlongLine.clear();
}

void VToolAlongLine::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

void VToolAlongLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogAlongLine, this, event);
}

void VToolAlongLine::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogAlongLine, this, event);
}

void VToolAlongLine::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrName, point->name());
    AddAttribute(domElement, AttrMx, toMM(point->mx()));
    AddAttribute(domElement, AttrMy, toMM(point->my()));

    AddAttribute(domElement, AttrTypeLine, typeLine);
    AddAttribute(domElement, AttrLength, formula);
    AddAttribute(domElement, AttrFirstPoint, basePointId);
    AddAttribute(domElement, AttrSecondPoint, secondPointId);

    AddToCalculation(domElement);
}

void VToolAlongLine::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        domElement.setAttribute(AttrMx, toMM(point->mx()));
        domElement.setAttribute(AttrMy, toMM(point->my()));
        domElement.setAttribute(AttrName, point->name());
        domElement.setAttribute(AttrTypeLine, typeLine);
        domElement.setAttribute(AttrLength, formula);
        domElement.setAttribute(AttrFirstPoint, basePointId);
        domElement.setAttribute(AttrSecondPoint, secondPointId);
    }
}

void VToolAlongLine::RemoveReferens()
{
    doc->DecrementReferens(secondPointId);
    VToolLinePoint::RemoveReferens();
}

void VToolAlongLine::setDialog()
{
    Q_ASSERT(dialogAlongLine.isNull() == false);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogAlongLine->setTypeLine(typeLine);
    dialogAlongLine->setFormula(formula);
    dialogAlongLine->setFirstPointId(basePointId, id);
    dialogAlongLine->setSecondPointId(secondPointId, id);
    dialogAlongLine->setPointName(p->name());
}

void VToolAlongLine::Create(QSharedPointer<DialogAlongLine> &dialog, VMainGraphicsScene *scene,
                            VDomDocument *doc, VContainer *data)
{
    QString formula = dialog->getFormula();
    qint64 firstPointId = dialog->getFirstPointId();
    qint64 secondPointId = dialog->getSecondPointId();
    QString typeLine = dialog->getTypeLine();
    QString pointName = dialog->getPointName();
    Create(0, pointName, typeLine, formula, firstPointId, secondPointId, 5, 10, scene, doc, data,
           Document::FullParse, Tool::FromGui);
}

void VToolAlongLine::Create(const qint64 _id, const QString &pointName, const QString &typeLine,
                            const QString &formula, const qint64 &firstPointId, const qint64 &secondPointId,
                            const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VDomDocument *doc,
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
        qint64 id = _id;
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
