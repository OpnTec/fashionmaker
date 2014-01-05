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

const QString VToolBisector::ToolType = QStringLiteral("bisector");

VToolBisector::VToolBisector(VDomDocument *doc, VContainer *data, const qint64 &id,
                             const QString &typeLine, const QString &formula, const qint64 &firstPointId,
                             const qint64 &secondPointId, const qint64 &thirdPointId, const Tool::Sources &typeCreation,
                             QGraphicsItem *parent)
    :VToolLinePoint(doc, data, id, typeLine, formula, secondPointId, 0, parent), firstPointId(0),
    thirdPointId(0), dialogBisector(QSharedPointer<DialogBisector>())
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
    Q_ASSERT(dialogBisector.isNull() == false);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogBisector->setTypeLine(typeLine);
    dialogBisector->setFormula(formula);
    dialogBisector->setFirstPointId(firstPointId, id);
    dialogBisector->setSecondPointId(basePointId, id);
    dialogBisector->setThirdPointId(thirdPointId, id);
    dialogBisector->setPointName(p->name());
}

void VToolBisector::Create(QSharedPointer<DialogBisector> &dialog, VMainGraphicsScene *scene, VDomDocument *doc,
                           VContainer *data)
{
    QString formula = dialog->getFormula();
    qint64 firstPointId = dialog->getFirstPointId();
    qint64 secondPointId = dialog->getSecondPointId();
    qint64 thirdPointId = dialog->getThirdPointId();
    QString typeLine = dialog->getTypeLine();
    QString pointName = dialog->getPointName();
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

void VToolBisector::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            domElement.setAttribute(AttrName, dialogBisector->getPointName());
            domElement.setAttribute(AttrTypeLine, dialogBisector->getTypeLine());
            domElement.setAttribute(AttrLength, dialogBisector->getFormula());
            domElement.setAttribute(AttrFirstPoint, QString().setNum(dialogBisector->getFirstPointId()));
            domElement.setAttribute(AttrSecondPoint, QString().setNum(dialogBisector->getSecondPointId()));
            domElement.setAttribute(AttrThirdPoint, QString().setNum(dialogBisector->getThirdPointId()));
            emit FullUpdateTree();
        }
    }
    dialogBisector.clear();
}

void VToolBisector::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

void VToolBisector::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogBisector, this, event);
}

void VToolBisector::AddToFile()
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
    AddAttribute(domElement, AttrFirstPoint, firstPointId);
    AddAttribute(domElement, AttrSecondPoint, basePointId);
    AddAttribute(domElement, AttrThirdPoint, thirdPointId);

    AddToCalculation(domElement);
}

void VToolBisector::RefreshDataInFile()
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
        domElement.setAttribute(AttrFirstPoint, firstPointId);
        domElement.setAttribute(AttrSecondPoint, basePointId);
        domElement.setAttribute(AttrThirdPoint, thirdPointId);
    }
}

void VToolBisector::RemoveReferens()
{
    doc->DecrementReferens(firstPointId);
    doc->DecrementReferens(thirdPointId);
    VToolLinePoint::RemoveReferens();
}
