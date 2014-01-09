/************************************************************************
 **
 **  @file   vtooltriangle.cpp
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

#include "vtooltriangle.h"

const QString VToolTriangle::ToolType = QStringLiteral("triangle");

VToolTriangle::VToolTriangle(VDomDocument *doc, VContainer *data, const qint64 &id,
                             const qint64 &axisP1Id, const qint64 &axisP2Id, const qint64 &firstPointId,
                             const qint64 &secondPointId, const Tool::Sources &typeCreation, QGraphicsItem *parent)
    :VToolPoint(doc, data, id, parent), axisP1Id(axisP1Id), axisP2Id(axisP2Id), firstPointId(firstPointId),
      secondPointId(secondPointId), dialogTriangle(QSharedPointer<DialogTriangle>())
{
    ignoreFullUpdate = true;
    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

void VToolTriangle::setDialog()
{
    Q_ASSERT(dialogTriangle.isNull() == false);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogTriangle->setAxisP1Id(axisP1Id, id);
    dialogTriangle->setAxisP2Id(axisP2Id, id);
    dialogTriangle->setFirstPointId(firstPointId, id);
    dialogTriangle->setSecondPointId(secondPointId, id);
    dialogTriangle->setPointName(p->name());
}

void VToolTriangle::Create(QSharedPointer<DialogTriangle> &dialog, VMainGraphicsScene *scene,
                           VDomDocument *doc, VContainer *data)
{
    qint64 axisP1Id = dialog->getAxisP1Id();
    qint64 axisP2Id = dialog->getAxisP2Id();
    qint64 firstPointId = dialog->getFirstPointId();
    qint64 secondPointId = dialog->getSecondPointId();
    QString pointName = dialog->getPointName();
    Create(0, pointName, axisP1Id, axisP2Id, firstPointId, secondPointId, 5, 10, scene, doc, data,
           Document::FullParse, Tool::FromGui);
}

void VToolTriangle::Create(const qint64 _id, const QString &pointName, const qint64 &axisP1Id,
                           const qint64 &axisP2Id, const qint64 &firstPointId, const qint64 &secondPointId,
                           const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VDomDocument *doc,
                           VContainer *data, const Document::Documents &parse, const Tool::Sources &typeCreation)
{
    const VPointF *axisP1 = data->GeometricObject<const VPointF *>(axisP1Id);
    const VPointF *axisP2 = data->GeometricObject<const VPointF *>(axisP2Id);
    const VPointF *firstPoint = data->GeometricObject<const VPointF *>(firstPointId);
    const VPointF *secondPoint = data->GeometricObject<const VPointF *>(secondPointId);

    QPointF point = FindPoint(axisP1->toQPointF(), axisP2->toQPointF(), firstPoint->toQPointF(),
                              secondPoint->toQPointF());
    qint64 id = _id;
    if (typeCreation == Tool::FromGui)
    {
        id = data->AddGObject(new VPointF(point.x(), point.y(), pointName, mx, my));
    }
    else
    {
        data->UpdateGObject(id, new VPointF(point.x(), point.y(), pointName, mx, my));
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::Triangle, doc);
    if (parse == Document::FullParse)
    {
        VToolTriangle *point = new VToolTriangle(doc, data, id, axisP1Id, axisP2Id, firstPointId,
                                                 secondPointId, typeCreation);
        scene->addItem(point);
        connect(point, &VToolTriangle::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolTriangle::SetFactor);
        doc->AddTool(id, point);
        doc->IncrementReferens(axisP1Id);
        doc->IncrementReferens(axisP2Id);
        doc->IncrementReferens(firstPointId);
        doc->IncrementReferens(secondPointId);
    }
}

QPointF VToolTriangle::FindPoint(const QPointF &axisP1, const QPointF &axisP2, const QPointF &firstPoint,
                                 const QPointF &secondPoint)
{
    qreal c = QLineF(firstPoint, secondPoint).length();
    qreal a = QLineF(axisP2, firstPoint).length();
    qreal b = QLineF(axisP2, secondPoint).length();
    if (fabs(c*c - (a*a + b*b)) < 0.0001)
    {
        QLineF l1(axisP2, firstPoint);
        QLineF l2(axisP2, secondPoint);
        if (fabs(l1.angleTo(l2) - 90) < 0.1 || fabs(l2.angleTo(l1) - 90) < 0.1)
        {
            return axisP2;
        }
    }

    QLineF line = QLineF(axisP1, axisP2);
    qreal step = 0.01;
    while (1)
    {
        line.setLength(line.length()+step);
        a = QLineF(line.p2(), firstPoint).length();
        b = QLineF(line.p2(), secondPoint).length();
        if (qFuzzyCompare(c*c, a*a + b*b))
        {
            QLineF l1(axisP2, firstPoint);
            QLineF l2(axisP2, secondPoint);
            if (fabs(l1.angleTo(l2) - 90) < 0.1 || fabs(l2.angleTo(l1) - 90) < 0.1)
            {
                return line.p2();
            }
        }
        if (c*c < a*a + b*b)
        {
            //Still don't know why this code handled. Need to think about that.
            qWarning()<<tr("Can't find point.")<<Q_FUNC_INFO;
            return line.p2();
        }
    }
}

void VToolTriangle::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        axisP1Id = domElement.attribute(AttrAxisP1, "").toLongLong();
        axisP2Id = domElement.attribute(AttrAxisP2, "").toLongLong();
        firstPointId = domElement.attribute(AttrFirstPoint, "").toLongLong();
        secondPointId = domElement.attribute(AttrSecondPoint, "").toLongLong();
    }
    VToolPoint::RefreshPointGeometry(*VDrawTool::data.GeometricObject<const VPointF *>(id));
}

void VToolTriangle::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            domElement.setAttribute(AttrName, dialogTriangle->getPointName());
            domElement.setAttribute(AttrAxisP1, QString().setNum(dialogTriangle->getAxisP1Id()));
            domElement.setAttribute(AttrAxisP2, QString().setNum(dialogTriangle->getAxisP2Id()));
            domElement.setAttribute(AttrFirstPoint, QString().setNum(dialogTriangle->getFirstPointId()));
            domElement.setAttribute(AttrSecondPoint, QString().setNum(dialogTriangle->getSecondPointId()));
            emit FullUpdateTree();
        }

    }
    dialogTriangle.clear();
}

void VToolTriangle::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogTriangle, this, event);
}

void VToolTriangle::RemoveReferens()
{
    doc->DecrementReferens(axisP1Id);
    doc->DecrementReferens(axisP2Id);
    doc->DecrementReferens(firstPointId);
    doc->DecrementReferens(secondPointId);
}

void VToolTriangle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogTriangle, this, event);
}

void VToolTriangle::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrName, point->name());
    AddAttribute(domElement, AttrMx, toMM(point->mx()));
    AddAttribute(domElement, AttrMy, toMM(point->my()));

    AddAttribute(domElement, AttrAxisP1, axisP1Id);
    AddAttribute(domElement, AttrAxisP2, axisP2Id);
    AddAttribute(domElement, AttrFirstPoint, firstPointId);
    AddAttribute(domElement, AttrSecondPoint, secondPointId);

    AddToCalculation(domElement);
}

void VToolTriangle::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        domElement.setAttribute(AttrName, point->name());
        domElement.setAttribute(AttrMx, toMM(point->mx()));
        domElement.setAttribute(AttrMy, toMM(point->my()));
        domElement.setAttribute(AttrAxisP1, axisP1Id);
        domElement.setAttribute(AttrAxisP2, axisP2Id);
        domElement.setAttribute(AttrFirstPoint, firstPointId);
        domElement.setAttribute(AttrSecondPoint, secondPointId);
    }
}
