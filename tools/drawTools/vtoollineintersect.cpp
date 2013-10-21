/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#include "vtoollineintersect.h"
#include <QMenu>

VToolLineIntersect::VToolLineIntersect(VDomDocument *doc, VContainer *data, const qint64 &id,
                                       const qint64 &p1Line1, const qint64 &p2Line1, const qint64 &p1Line2,
                                       const qint64 &p2Line2, Tool::Sources typeCreation,
                                       QGraphicsItem *parent):
    VToolPoint(doc, data, id, parent), p1Line1(p1Line1), p2Line1(p2Line1), p1Line2(p1Line2),
    p2Line2(p2Line2), dialogLineIntersect(QSharedPointer<DialogLineIntersect>()){
    ignoreFullUpdate = true;
    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VToolLineIntersect::setDialog(){
    Q_ASSERT(!dialogLineIntersect.isNull());
    if(!dialogLineIntersect.isNull()){
        VPointF p = VAbstractTool::data.GetPoint(id);
        dialogLineIntersect->setP1Line1(p1Line1);
        dialogLineIntersect->setP2Line1(p2Line1);
        dialogLineIntersect->setP1Line2(p1Line2);
        dialogLineIntersect->setP2Line2(p2Line2);
        dialogLineIntersect->setPointName(p.name());
    }
}

void VToolLineIntersect::Create(QSharedPointer<DialogLineIntersect> &dialog, VMainGraphicsScene *scene,
                                VDomDocument *doc, VContainer *data){
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
                                Tool::Sources typeCreation){
    VPointF p1Line1 = data->GetPoint(p1Line1Id);
    VPointF p2Line1 = data->GetPoint(p2Line1Id);
    VPointF p1Line2 = data->GetPoint(p1Line2Id);
    VPointF p2Line2 = data->GetPoint(p2Line2Id);

    QLineF line1(p1Line1.toQPointF(), p2Line1.toQPointF());
    QLineF line2(p1Line2.toQPointF(), p2Line2.toQPointF());
    QPointF fPoint;
    QLineF::IntersectType intersect = line1.intersect(line2, &fPoint);
    if(intersect == QLineF::UnboundedIntersection || intersect == QLineF::BoundedIntersection){
        qint64 id = _id;
        if(typeCreation == Tool::FromGui){
            id = data->AddPoint(VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            data->AddLine(p1Line1Id, id);
            data->AddLine(id, p2Line1Id);
            data->AddLine(p1Line2Id, id);
            data->AddLine(id, p2Line2Id);
        } else {
            data->UpdatePoint(id, VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            data->AddLine(p1Line1Id, id);
            data->AddLine(id, p2Line1Id);
            data->AddLine(p1Line2Id, id);
            data->AddLine(id, p2Line2Id);
            if(parse != Document::FullParse){
                doc->UpdateToolData(id, data);
            }
        }
        VDrawTool::AddRecord(id, Tool::LineIntersectTool, doc);
        if(parse == Document::FullParse){
            VToolLineIntersect *point = new VToolLineIntersect(doc, data, id, p1Line1Id,
                                                               p2Line1Id, p1Line2Id,
                                                               p2Line2Id, typeCreation);
            scene->addItem(point);
            connect(point, &VToolLineIntersect::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(point, &VToolLineIntersect::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
            connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolLineIntersect::SetFactor);
            doc->AddTool(id, point);
            doc->IncrementReferens(p1Line1Id);
            doc->IncrementReferens(p2Line1Id);
            doc->IncrementReferens(p1Line2Id);
            doc->IncrementReferens(p2Line2Id);
        }
    }
}

void VToolLineIntersect::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        p1Line1 = domElement.attribute("p1Line1", "").toLongLong();
        p2Line1 = domElement.attribute("p2Line1", "").toLongLong();
        p1Line2 = domElement.attribute("p1Line2", "").toLongLong();
        p2Line2 = domElement.attribute("p2Line2", "").toLongLong();
    }
    RefreshPointGeometry(VAbstractTool::data.GetPoint(id));
}

void VToolLineIntersect::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", dialogLineIntersect->getPointName());
            domElement.setAttribute("p1Line1", QString().setNum(dialogLineIntersect->getP1Line1()));
            domElement.setAttribute("p2Line1", QString().setNum(dialogLineIntersect->getP2Line1()));
            domElement.setAttribute("p1Line2", QString().setNum(dialogLineIntersect->getP1Line2()));
            domElement.setAttribute("p2Line2", QString().setNum(dialogLineIntersect->getP2Line2()));
            emit FullUpdateTree();
        }
    }
    dialogLineIntersect.clear();
}

void VToolLineIntersect::SetFactor(qreal factor){
    VDrawTool::SetFactor(factor);
    RefreshPointGeometry(VAbstractTool::data.GetPoint(id));
}

void VToolLineIntersect::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogLineIntersect, this, event);
}

void VToolLineIntersect::AddToFile(){
    VPointF point = VAbstractTool::data.GetPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "lineIntersect");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "mx", toMM(point.mx()));
    AddAttribute(domElement, "my", toMM(point.my()));

    AddAttribute(domElement, "p1Line1", p1Line1);
    AddAttribute(domElement, "p2Line1", p2Line1);
    AddAttribute(domElement, "p1Line2", p1Line2);
    AddAttribute(domElement, "p2Line2", p2Line2);

    AddToCalculation(domElement);
}

void VToolLineIntersect::RemoveReferens(){
    doc->DecrementReferens(p1Line1);
    doc->DecrementReferens(p2Line1);
    doc->DecrementReferens(p1Line2);
    doc->DecrementReferens(p2Line2);
}
