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

#include "vtoolnormal.h"

VToolNormal::VToolNormal(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &typeLine,
                         const QString &formula, const qreal &angle, const qint64 &firstPointId,
                         const qint64 &secondPointId, Tool::Sources typeCreation, QGraphicsItem *parent):
    VToolLinePoint(doc, data, id, typeLine, formula, firstPointId, angle, parent),
    secondPointId(secondPointId), dialogNormal(QSharedPointer<DialogNormal>()){

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }

}

void VToolNormal::setDialog(){
    Q_ASSERT(!dialogNormal.isNull());
    if(!dialogNormal.isNull()){
        VPointF p = VAbstractTool::data.GetPoint(id);
        dialogNormal->setTypeLine(typeLine);
        dialogNormal->setFormula(formula);
        dialogNormal->setAngle(angle);
        dialogNormal->setFirstPointId(basePointId, id);
        dialogNormal->setSecondPointId(secondPointId, id);
        dialogNormal->setPointName(p.name());
    }
}

void VToolNormal::Create(QSharedPointer<DialogNormal> &dialog, VMainGraphicsScene *scene, VDomDocument *doc,
                         VContainer *data){
    QString formula = dialog->getFormula();
    qint64 firstPointId = dialog->getFirstPointId();
    qint64 secondPointId = dialog->getSecondPointId();
    QString typeLine = dialog->getTypeLine();
    QString pointName = dialog->getPointName();
    qreal angle = dialog->getAngle();
    Create(0, formula, firstPointId, secondPointId, typeLine, pointName, angle, 5, 10, scene, doc, data,
           Document::FullParse, Tool::FromGui);
}

void VToolNormal::Create(const qint64 _id, const QString &formula, const qint64 &firstPointId,
                         const qint64 &secondPointId, const QString typeLine, const QString pointName,
                         const qreal angle, const qreal &mx, const qreal &my, VMainGraphicsScene *scene,
                         VDomDocument *doc, VContainer *data, const Document::Documents &parse,
                         Tool::Sources typeCreation){
    VPointF firstPoint = data->GetPoint(firstPointId);
    VPointF secondPoint = data->GetPoint(secondPointId);
    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if(errorMsg.isEmpty()){
        QPointF fPoint = VToolNormal::FindPoint(firstPoint.toQPointF(), secondPoint.toQPointF(),
                                                toPixel(result), angle);
        qint64 id = _id;
        if(typeCreation == Tool::FromGui){
            id = data->AddPoint(VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            data->AddLine(firstPointId, id);
        } else {
            data->UpdatePoint(id, VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            data->AddLine(firstPointId, id);
            if(parse != Document::FullParse){
                doc->UpdateToolData(id, data);
            }
        }
        VDrawTool::AddRecord(id, Tool::NormalTool, doc);
        if(parse == Document::FullParse){
            VToolNormal *point = new VToolNormal(doc, data, id, typeLine, formula, angle,
                                                 firstPointId, secondPointId, typeCreation);
            scene->addItem(point);
            connect(point, &VToolNormal::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(point, &VToolNormal::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
            connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolNormal::SetFactor);
            doc->AddTool(id, point);
            doc->IncrementReferens(firstPointId);
            doc->IncrementReferens(secondPointId);
        }
    }
}

QPointF VToolNormal::FindPoint(const QPointF &firstPoint, const QPointF &secondPoint, const qreal &length,
                               const qreal &angle){
    QLineF line(firstPoint, secondPoint);
    QLineF normal = line.normalVector();
    normal.setAngle(normal.angle()+angle);
    normal.setLength(length);
    return normal.p2();
}

void VToolNormal::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        typeLine = domElement.attribute("typeLine", "");
        formula = domElement.attribute("length", "");
        basePointId = domElement.attribute("firstPoint", "").toLongLong();
        secondPointId = domElement.attribute("secondPoint", "").toLongLong();
        angle = domElement.attribute("angle", "").toDouble();
    }
    RefreshGeometry();
}

void VToolNormal::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", dialogNormal->getPointName());
            domElement.setAttribute("typeLine", dialogNormal->getTypeLine());
            domElement.setAttribute("length", dialogNormal->getFormula());
            domElement.setAttribute("angle", QString().setNum(dialogNormal->getAngle()));
            domElement.setAttribute("firstPoint", QString().setNum(dialogNormal->getFirstPointId()));
            domElement.setAttribute("secondPoint", QString().setNum(dialogNormal->getSecondPointId()));
            emit FullUpdateTree();
        }
    }
    dialogNormal.clear();
}

void VToolNormal::SetFactor(qreal factor){
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}

void VToolNormal::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogNormal, this, event);
}

void VToolNormal::AddToFile(){
    VPointF point = VAbstractTool::data.GetPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "normal");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "mx", toMM(point.mx()));
    AddAttribute(domElement, "my", toMM(point.my()));

    AddAttribute(domElement, "typeLine", typeLine);
    AddAttribute(domElement, "length", formula);
    AddAttribute(domElement, "angle", angle);
    AddAttribute(domElement, "firstPoint", basePointId);
    AddAttribute(domElement, "secondPoint", secondPointId);

    AddToCalculation(domElement);
}

void VToolNormal::RemoveReferens(){
    doc->DecrementReferens(secondPointId);
    VToolLinePoint::RemoveReferens();
}
