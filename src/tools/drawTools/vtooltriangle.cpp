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
#include "../../dialogs/tools/dialogtriangle.h"

const QString VToolTriangle::ToolType = QStringLiteral("triangle");

VToolTriangle::VToolTriangle(VPattern *doc, VContainer *data, const quint32 &id,
                             const quint32 &axisP1Id, const quint32 &axisP2Id, const quint32 &firstPointId,
                             const quint32 &secondPointId, const Valentina::Sources &typeCreation, QGraphicsItem *parent)
    :VToolPoint(doc, data, id, parent), axisP1Id(axisP1Id), axisP2Id(axisP2Id), firstPointId(firstPointId),
      secondPointId(secondPointId)
{
    ignoreFullUpdate = true;
    if (typeCreation == Valentina::FromGui)
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
    Q_CHECK_PTR(dialog);
    DialogTriangle *dialogTool = qobject_cast<DialogTriangle*>(dialog);
    Q_CHECK_PTR(dialogTool);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogTool->setAxisP1Id(axisP1Id, id);
    dialogTool->setAxisP2Id(axisP2Id, id);
    dialogTool->setFirstPointId(firstPointId, id);
    dialogTool->setSecondPointId(secondPointId, id);
    dialogTool->setPointName(p->name());
}

void VToolTriangle::Create(DialogTool *dialog, VMainGraphicsScene *scene,
                           VPattern *doc, VContainer *data)
{
    Q_CHECK_PTR(dialog);
    DialogTriangle *dialogTool = qobject_cast<DialogTriangle*>(dialog);
    Q_CHECK_PTR(dialogTool);
    quint32 axisP1Id = dialogTool->getAxisP1Id();
    quint32 axisP2Id = dialogTool->getAxisP2Id();
    quint32 firstPointId = dialogTool->getFirstPointId();
    quint32 secondPointId = dialogTool->getSecondPointId();
    QString pointName = dialogTool->getPointName();
    Create(0, pointName, axisP1Id, axisP2Id, firstPointId, secondPointId, 5, 10, scene, doc, data,
           Document::FullParse, Valentina::FromGui);
}

void VToolTriangle::Create(const quint32 _id, const QString &pointName, const quint32 &axisP1Id,
                           const quint32 &axisP2Id, const quint32 &firstPointId, const quint32 &secondPointId,
                           const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VPattern *doc,
                           VContainer *data, const Document::Documents &parse, const Valentina::Sources &typeCreation)
{
    const VPointF *axisP1 = data->GeometricObject<const VPointF *>(axisP1Id);
    const VPointF *axisP2 = data->GeometricObject<const VPointF *>(axisP2Id);
    const VPointF *firstPoint = data->GeometricObject<const VPointF *>(firstPointId);
    const VPointF *secondPoint = data->GeometricObject<const VPointF *>(secondPointId);

    QPointF point = FindPoint(axisP1->toQPointF(), axisP2->toQPointF(), firstPoint->toQPointF(),
                              secondPoint->toQPointF());
    quint32 id = _id;
    if (typeCreation == Valentina::FromGui)
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
    VDrawTool::AddRecord(id, Valentina::Triangle, doc);
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
        axisP1Id = domElement.attribute(AttrAxisP1, "").toUInt();
        axisP2Id = domElement.attribute(AttrAxisP2, "").toUInt();
        firstPointId = domElement.attribute(AttrFirstPoint, "").toUInt();
        secondPointId = domElement.attribute(AttrSecondPoint, "").toUInt();
    }
    VToolPoint::RefreshPointGeometry(*VDrawTool::data.GeometricObject<const VPointF *>(id));
}

void VToolTriangle::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogTriangle>(this, event);
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
    ContextMenu<DialogTriangle>(this, event);
}

void VToolTriangle::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrName, point->name());
    doc->SetAttribute(domElement, AttrMx, toMM(point->mx()));
    doc->SetAttribute(domElement, AttrMy, toMM(point->my()));

    doc->SetAttribute(domElement, AttrAxisP1, axisP1Id);
    doc->SetAttribute(domElement, AttrAxisP2, axisP2Id);
    doc->SetAttribute(domElement, AttrFirstPoint, firstPointId);
    doc->SetAttribute(domElement, AttrSecondPoint, secondPointId);

    AddToCalculation(domElement);
}

void VToolTriangle::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrName, point->name());
        doc->SetAttribute(domElement, AttrMx, toMM(point->mx()));
        doc->SetAttribute(domElement, AttrMy, toMM(point->my()));
        doc->SetAttribute(domElement, AttrAxisP1, axisP1Id);
        doc->SetAttribute(domElement, AttrAxisP2, axisP2Id);
        doc->SetAttribute(domElement, AttrFirstPoint, firstPointId);
        doc->SetAttribute(domElement, AttrSecondPoint, secondPointId);
    }
}

void VToolTriangle::SaveDialog(QDomElement &domElement)
{
    Q_CHECK_PTR(dialog);
    DialogTriangle *dialogTool = qobject_cast<DialogTriangle*>(dialog);
    Q_CHECK_PTR(dialogTool);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrAxisP1, QString().setNum(dialogTool->getAxisP1Id()));
    doc->SetAttribute(domElement, AttrAxisP2, QString().setNum(dialogTool->getAxisP2Id()));
    doc->SetAttribute(domElement, AttrFirstPoint, QString().setNum(dialogTool->getFirstPointId()));
    doc->SetAttribute(domElement, AttrSecondPoint, QString().setNum(dialogTool->getSecondPointId()));
}
