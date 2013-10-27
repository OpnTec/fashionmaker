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
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#include "vmodelingbisector.h"
#include "../drawTools/vtoolbisector.h"
#include <container/calculator.h>

VModelingBisector::VModelingBisector(VDomDocument *doc, VContainer *data, const qint64 &id,
                             const QString &typeLine, const QString &formula, const qint64 &firstPointId,
                             const qint64 &secondPointId, const qint64 &thirdPointId, Tool::Sources typeCreation,
                             QGraphicsItem *parent):
    VModelingLinePoint(doc, data, id, typeLine, formula, secondPointId, 0, parent), firstPointId(0),
    thirdPointId(0), dialogBisector(QSharedPointer<DialogBisector>()){
    this->firstPointId = firstPointId;
    this->thirdPointId = thirdPointId;

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VModelingBisector::setDialog(){
    Q_ASSERT(!dialogBisector.isNull());
    if(!dialogBisector.isNull()){
        VPointF p = VAbstractTool::data.GetModelingPoint(id);
        dialogBisector->setTypeLine(typeLine);
        dialogBisector->setFormula(formula);
        dialogBisector->setFirstPointId(firstPointId, id);
        dialogBisector->setSecondPointId(basePointId, id);
        dialogBisector->setThirdPointId(thirdPointId, id);
        dialogBisector->setPointName(p.name());
    }
}

VModelingBisector *VModelingBisector::Create(QSharedPointer<DialogBisector> &dialog, VDomDocument *doc,
                                             VContainer *data){
    QString formula = dialog->getFormula();
    qint64 firstPointId = dialog->getFirstPointId();
    qint64 secondPointId = dialog->getSecondPointId();
    qint64 thirdPointId = dialog->getThirdPointId();
    QString typeLine = dialog->getTypeLine();
    QString pointName = dialog->getPointName();
    return Create(0, formula, firstPointId, secondPointId, thirdPointId, typeLine, pointName, 5, 10, doc,
                  data, Document::FullParse, Tool::FromGui);
}

VModelingBisector *VModelingBisector::Create(const qint64 _id, const QString &formula,
                                             const qint64 &firstPointId, const qint64 &secondPointId,
                                             const qint64 &thirdPointId, const QString &typeLine,
                                             const QString &pointName, const qreal &mx, const qreal &my,
                                             VDomDocument *doc, VContainer *data, const Document::Documents &parse,
                                             Tool::Sources typeCreation){
    VModelingBisector *point = 0;
    VPointF firstPoint = data->GetModelingPoint(firstPointId);
    VPointF secondPoint = data->GetModelingPoint(secondPointId);
    VPointF thirdPoint = data->GetModelingPoint(thirdPointId);

    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if(errorMsg.isEmpty()){
        QPointF fPoint = VToolBisector::FindPoint(firstPoint.toQPointF(), secondPoint.toQPointF(),
                                                  thirdPoint.toQPointF(), result*PrintDPI/25.4);
        qint64 id = _id;
        if(typeCreation == Tool::FromGui){
            id = data->AddModelingPoint(VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
        } else {
            data->UpdateModelingPoint(id, VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            if(parse != Document::FullParse){
                doc->UpdateToolData(id, data);
            }
        }
        data->AddLine(firstPointId, id, Draw::Modeling);
        if(parse == Document::FullParse){
            point = new VModelingBisector(doc, data, id, typeLine, formula, firstPointId, secondPointId,
                                          thirdPointId, typeCreation);
            doc->AddTool(id, point);
            doc->IncrementReferens(firstPointId);
            doc->IncrementReferens(secondPointId);
            doc->IncrementReferens(thirdPointId);
        }
    }
    return point;
}

void VModelingBisector::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        typeLine = domElement.attribute("typeLine", "");
        formula = domElement.attribute("length", "");
        firstPointId = domElement.attribute("firstPoint", "").toLongLong();
        basePointId = domElement.attribute("secondPoint", "").toLongLong();
        thirdPointId = domElement.attribute("thirdPoint", "").toLongLong();
    }
    RefreshGeometry();
}

void VModelingBisector::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", dialogBisector->getPointName());
            domElement.setAttribute("typeLine", dialogBisector->getTypeLine());
            domElement.setAttribute("length", dialogBisector->getFormula());
            domElement.setAttribute("firstPoint", QString().setNum(dialogBisector->getFirstPointId()));
            domElement.setAttribute("secondPoint", QString().setNum(dialogBisector->getSecondPointId()));
            domElement.setAttribute("thirdPoint", QString().setNum(dialogBisector->getThirdPointId()));
            emit FullUpdateTree();
        }
    }
    dialogBisector.clear();
}

void VModelingBisector::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogBisector, this, event);
}

void VModelingBisector::AddToFile(){
    VPointF point = VAbstractTool::data.GetModelingPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "bisector");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "mx", point.mx()/PrintDPI*25.4);
    AddAttribute(domElement, "my", point.my()/PrintDPI*25.4);

    AddAttribute(domElement, "typeLine", typeLine);
    AddAttribute(domElement, "length", formula);
    AddAttribute(domElement, "firstPoint", firstPointId);
    AddAttribute(domElement, "secondPoint", basePointId);
    AddAttribute(domElement, "thirdPoint", thirdPointId);

    AddToModeling(domElement);
}

void VModelingBisector::RemoveReferens(){
    doc->DecrementReferens(firstPointId);
    doc->DecrementReferens(thirdPointId);
    VModelingLinePoint::RemoveReferens();
}
