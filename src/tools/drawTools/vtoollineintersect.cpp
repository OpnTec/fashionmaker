/************************************************************************
 **
 **  @file   vtoollineintersect.cpp
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

#include "vtoollineintersect.h"

const QString VToolLineIntersect::ToolType = QStringLiteral("lineIntersect");

VToolLineIntersect::VToolLineIntersect(VDomDocument *doc, VContainer *data, const qint64 &id,
                                       const qint64 &p1Line1, const qint64 &p2Line1, const qint64 &p1Line2,
                                       const qint64 &p2Line2, const Tool::Sources &typeCreation,
                                       QGraphicsItem *parent)
    :VToolPoint(doc, data, id, parent), p1Line1(p1Line1), p2Line1(p2Line1), p1Line2(p1Line2),
    p2Line2(p2Line2), dialogLineIntersect(QSharedPointer<DialogLineIntersect>())
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

void VToolLineIntersect::setDialog()
{
    Q_ASSERT(dialogLineIntersect.isNull() == false);
    const VPointF *p = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    dialogLineIntersect->setP1Line1(p1Line1);
    dialogLineIntersect->setP2Line1(p2Line1);
    dialogLineIntersect->setP1Line2(p1Line2);
    dialogLineIntersect->setP2Line2(p2Line2);
    dialogLineIntersect->setPointName(p->name());
}

void VToolLineIntersect::Create(QSharedPointer<DialogLineIntersect> &dialog, VMainGraphicsScene *scene,
                                VDomDocument *doc, VContainer *data)
{
    qint64 p1Line1Id = dialog->getP1Line1();
    qint64 p2Line1Id = dialog->getP2Line1();
    qint64 p1Line2Id = dialog->getP1Line2();
    qint64 p2Line2Id = dialog->getP2Line2();
    QString pointName = dialog->getPointName();
    Create(0, p1Line1Id, p2Line1Id, p1Line2Id, p2Line2Id, pointName, 5, 10, scene, doc, data,
           Document::FullParse, Tool::FromGui);
}

void VToolLineIntersect::Create(const qint64 _id, const qint64 &p1Line1Id, const qint64 &p2Line1Id,
                                const qint64 &p1Line2Id, const qint64 &p2Line2Id, const QString &pointName,
                                const qreal &mx, const qreal &my, VMainGraphicsScene *scene,
                                VDomDocument *doc, VContainer *data, const Document::Documents &parse,
                                const Tool::Sources &typeCreation)
{
    const VPointF *p1Line1 = data->GeometricObject<const VPointF *>(p1Line1Id);
    const VPointF *p2Line1 = data->GeometricObject<const VPointF *>(p2Line1Id);
    const VPointF *p1Line2 = data->GeometricObject<const VPointF *>(p1Line2Id);
    const VPointF *p2Line2 = data->GeometricObject<const VPointF *>(p2Line2Id);

    QLineF line1(p1Line1->toQPointF(), p2Line1->toQPointF());
    QLineF line2(p1Line2->toQPointF(), p2Line2->toQPointF());
    QPointF fPoint;
    QLineF::IntersectType intersect = line1.intersect(line2, &fPoint);
    if (intersect == QLineF::UnboundedIntersection || intersect == QLineF::BoundedIntersection)
    {
        qint64 id = _id;
        if (typeCreation == Tool::FromGui)
        {
            id = data->AddGObject(new VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            data->AddLine(p1Line1Id, id);
            data->AddLine(id, p2Line1Id);
            data->AddLine(p1Line2Id, id);
            data->AddLine(id, p2Line2Id);
        }
        else
        {
            data->UpdateGObject(id, new VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            data->AddLine(p1Line1Id, id);
            data->AddLine(id, p2Line1Id);
            data->AddLine(p1Line2Id, id);
            data->AddLine(id, p2Line2Id);
            if (parse != Document::FullParse)
            {
                doc->UpdateToolData(id, data);
            }
        }
        VDrawTool::AddRecord(id, Tool::LineIntersectTool, doc);
        if (parse == Document::FullParse)
        {
            VToolLineIntersect *point = new VToolLineIntersect(doc, data, id, p1Line1Id, p2Line1Id, p1Line2Id,
                                                               p2Line2Id, typeCreation);
            scene->addItem(point);
            connect(point, &VToolLineIntersect::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolLineIntersect::SetFactor);
            doc->AddTool(id, point);
            doc->IncrementReferens(p1Line1Id);
            doc->IncrementReferens(p2Line1Id);
            doc->IncrementReferens(p1Line2Id);
            doc->IncrementReferens(p2Line2Id);
        }
    }
}

void VToolLineIntersect::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        p1Line1 = domElement.attribute(AttrP1Line1, "").toLongLong();
        p2Line1 = domElement.attribute(AttrP2Line1, "").toLongLong();
        p1Line2 = domElement.attribute(AttrP1Line2, "").toLongLong();
        p2Line2 = domElement.attribute(AttrP2Line2, "").toLongLong();
    }
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<const VPointF *>(id));
}

void VToolLineIntersect::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            SetAttribute(domElement, AttrName, dialogLineIntersect->getPointName());
            SetAttribute(domElement, AttrP1Line1, QString().setNum(dialogLineIntersect->getP1Line1()));
            SetAttribute(domElement, AttrP2Line1, QString().setNum(dialogLineIntersect->getP2Line1()));
            SetAttribute(domElement, AttrP1Line2, QString().setNum(dialogLineIntersect->getP1Line2()));
            SetAttribute(domElement, AttrP2Line2, QString().setNum(dialogLineIntersect->getP2Line2()));
            emit FullUpdateTree();
        }
    }
    dialogLineIntersect.clear();
}

void VToolLineIntersect::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<const VPointF *>(id));
}

void VToolLineIntersect::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogLineIntersect, this, event);
}

void VToolLineIntersect::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogLineIntersect, this, event);
}

void VToolLineIntersect::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    SetAttribute(domElement, AttrId, id);
    SetAttribute(domElement, AttrType, ToolType);
    SetAttribute(domElement, AttrName, point->name());
    SetAttribute(domElement, AttrMx, toMM(point->mx()));
    SetAttribute(domElement, AttrMy, toMM(point->my()));

    SetAttribute(domElement, AttrP1Line1, p1Line1);
    SetAttribute(domElement, AttrP2Line1, p2Line1);
    SetAttribute(domElement, AttrP1Line2, p1Line2);
    SetAttribute(domElement, AttrP2Line2, p2Line2);

    AddToCalculation(domElement);
}

void VToolLineIntersect::RefreshDataInFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        SetAttribute(domElement, AttrName, point->name());
        SetAttribute(domElement, AttrMx, toMM(point->mx()));
        SetAttribute(domElement, AttrMy, toMM(point->my()));
        SetAttribute(domElement, AttrP1Line1, p1Line1);
        SetAttribute(domElement, AttrP2Line1, p2Line1);
        SetAttribute(domElement, AttrP1Line2, p1Line2);
        SetAttribute(domElement, AttrP2Line2, p2Line2);
    }
}

void VToolLineIntersect::RemoveReferens()
{
    doc->DecrementReferens(p1Line1);
    doc->DecrementReferens(p2Line1);
    doc->DecrementReferens(p1Line2);
    doc->DecrementReferens(p2Line2);
}
