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

#include "vmodelingnormal.h"
#include "../drawTools/vtoolnormal.h"
#include <container/calculator.h>

const QString VModelingNormal::ToolType = QStringLiteral("normal");

VModelingNormal::VModelingNormal(VDomDocument *doc, VContainer *data, const qint64 &id,
                         const QString &typeLine,
                         const QString &formula, const qreal &angle, const qint64 &firstPointId,
                         const qint64 &secondPointId, Tool::Sources typeCreation, QGraphicsItem *parent):
    VModelingLinePoint(doc, data, id, typeLine, formula, firstPointId, angle, parent),
    secondPointId(secondPointId), dialogNormal(QSharedPointer<DialogNormal>()){

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VModelingNormal::setDialog(){
    Q_ASSERT(!dialogNormal.isNull());
    if(!dialogNormal.isNull()){
        VPointF p = VAbstractTool::data.GetModelingPoint(id);
        dialogNormal->setTypeLine(typeLine);
        dialogNormal->setFormula(formula);
        dialogNormal->setAngle(angle);
        dialogNormal->setFirstPointId(basePointId, id);
        dialogNormal->setSecondPointId(secondPointId, id);
        dialogNormal->setPointName(p.name());
    }
}

VModelingNormal* VModelingNormal::Create(QSharedPointer<DialogNormal> &dialog, VDomDocument *doc,
                                         VContainer *data){
    QString formula = dialog->getFormula();
    qint64 firstPointId = dialog->getFirstPointId();
    qint64 secondPointId = dialog->getSecondPointId();
    QString typeLine = dialog->getTypeLine();
    QString pointName = dialog->getPointName();
    qreal angle = dialog->getAngle();
    return Create(0, formula, firstPointId, secondPointId, typeLine, pointName, angle, 5, 10, doc, data,
                  Document::FullParse, Tool::FromGui);
}

VModelingNormal *VModelingNormal::Create(const qint64 _id, const QString &formula,
                                         const qint64 &firstPointId, const qint64 &secondPointId,
                                         const QString typeLine, const QString pointName,
                                         const qreal angle, const qreal &mx, const qreal &my,
                                         VDomDocument *doc, VContainer *data, const Document::Documents &parse,
                                         Tool::Sources typeCreation){
    VModelingNormal *point = 0;
    VPointF firstPoint = data->GetModelingPoint(firstPointId);
    VPointF secondPoint = data->GetModelingPoint(secondPointId);
    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if(errorMsg.isEmpty()){
        QPointF fPoint = VToolNormal::FindPoint(firstPoint.toQPointF(), secondPoint.toQPointF(),
                                                toPixel(result), angle);
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
            point = new VModelingNormal(doc, data, id, typeLine, formula, angle, firstPointId, secondPointId,
                                        typeCreation);
            doc->AddTool(id, point);
            doc->IncrementReferens(firstPointId);
            doc->IncrementReferens(secondPointId);
        }
    }
    return point;
}

void VModelingNormal::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        typeLine = domElement.attribute(AttrTypeLine, "");
        formula = domElement.attribute(AttrLength, "");
        basePointId = domElement.attribute(AttrFirstPoint, "").toLongLong();
        secondPointId = domElement.attribute(AttrSecondPoint, "").toLongLong();
        angle = domElement.attribute(AttrAngle, "").toInt();
    }
    RefreshGeometry();
}

void VModelingNormal::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute(AttrName, dialogNormal->getPointName());
            domElement.setAttribute(AttrTypeLine, dialogNormal->getTypeLine());
            domElement.setAttribute(AttrLength, dialogNormal->getFormula());
            domElement.setAttribute(AttrAngle, QString().setNum(dialogNormal->getAngle()));
            domElement.setAttribute(AttrFirstPoint, QString().setNum(dialogNormal->getFirstPointId()));
            domElement.setAttribute(AttrSecondPoint, QString().setNum(dialogNormal->getSecondPointId()));
            emit FullUpdateTree();
        }
    }
    dialogNormal.clear();
}

void VModelingNormal::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogNormal, this, event);
}

void VModelingNormal::AddToFile(){
    VPointF point = VAbstractTool::data.GetModelingPoint(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrName, point.name());
    AddAttribute(domElement, AttrMx, toMM(point.mx()));
    AddAttribute(domElement, AttrMy, toMM(point.my()));

    AddAttribute(domElement, AttrTypeLine, typeLine);
    AddAttribute(domElement, AttrLength, formula);
    AddAttribute(domElement, AttrAngle, angle);
    AddAttribute(domElement, AttrFirstPoint, basePointId);
    AddAttribute(domElement, AttrSecondPoint, secondPointId);

    AddToModeling(domElement);
}

void VModelingNormal::RemoveReferens(){
    doc->DecrementReferens(secondPointId);
    VModelingLinePoint::RemoveReferens();
}
