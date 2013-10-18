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

#include "vmodelingendline.h"
#include <QDebug>
#include <QMenu>
#include "widgets/vmaingraphicsscene.h"

VModelingEndLine::VModelingEndLine(VDomDocument *doc, VContainer *data, const qint64 &id,
                                   const QString &typeLine, const QString &formula, const qreal &angle,
                                   const qint64 &basePointId, Tool::Sources typeCreation,
                                   QGraphicsItem *parent):
    VModelingLinePoint(doc, data, id, typeLine, formula, basePointId, angle, parent),
    dialogEndLine(QSharedPointer<DialogEndLine>()){

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VModelingEndLine::setDialog(){
    Q_ASSERT(!dialogEndLine.isNull());
    VPointF p = VAbstractTool::data.GetModelingPoint(id);
    dialogEndLine->setTypeLine(typeLine);
    dialogEndLine->setFormula(formula);
    dialogEndLine->setAngle(angle);
    dialogEndLine->setBasePointId(basePointId, id);
    dialogEndLine->setPointName(p.name());
}

VModelingEndLine *VModelingEndLine::Create(QSharedPointer<DialogEndLine> &dialog, VDomDocument *doc,
                                           VContainer *data){
    QString pointName = dialog->getPointName();
    QString typeLine = dialog->getTypeLine();
    QString formula = dialog->getFormula();
    qreal angle = dialog->getAngle();
    qint64 basePointId = dialog->getBasePointId();
    return Create(0, pointName, typeLine, formula, angle, basePointId, 5, 10, doc, data, Document::FullParse,
                  Tool::FromGui);
}

VModelingEndLine *VModelingEndLine::Create(const qint64 _id, const QString &pointName,
                                           const QString &typeLine, const QString &formula,
                                           const qreal &angle, const qint64 &basePointId, const qreal &mx,
                                           const qreal &my, VDomDocument *doc, VContainer *data,
                                           const Document::Documents &parse, Tool::Sources typeCreation){
    VModelingEndLine *point = 0;
    VPointF basePoint = data->GetModelingPoint(basePointId);
    QLineF line = QLineF(basePoint.toQPointF(), QPointF(basePoint.x()+100, basePoint.y()));
    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if(errorMsg.isEmpty()){
        line.setLength(toPixel(result));
        line.setAngle(angle);
        qint64 id = _id;
        if(typeCreation == Tool::FromGui){
            id = data->AddModelingPoint(VPointF(line.p2().x(), line.p2().y(), pointName, mx, my));
        } else {
            data->UpdateModelingPoint(id, VPointF(line.p2().x(), line.p2().y(), pointName, mx, my));
            if(parse != Document::FullParse){
                doc->UpdateToolData(id, data);
            }
        }
        data->AddLine(basePointId, id, Draw::Modeling);
        if(parse == Document::FullParse){
            point = new VModelingEndLine(doc, data, id, typeLine, formula, angle, basePointId, typeCreation);
            doc->AddTool(id, point);
            doc->IncrementReferens(basePointId);
        }
    }
    return point;
}

void VModelingEndLine::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        typeLine = domElement.attribute("typeLine", "");
        formula = domElement.attribute("length", "");
        basePointId = domElement.attribute("basePoint", "").toLongLong();
        angle = domElement.attribute("angle", "").toDouble();
    }
    RefreshGeometry();
}

void VModelingEndLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogEndLine, this, event);
}

void VModelingEndLine::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", dialogEndLine->getPointName());
            domElement.setAttribute("typeLine", dialogEndLine->getTypeLine());
            domElement.setAttribute("length", dialogEndLine->getFormula());
            domElement.setAttribute("angle", QString().setNum(dialogEndLine->getAngle()));
            domElement.setAttribute("basePoint", QString().setNum(dialogEndLine->getBasePointId()));
            emit FullUpdateTree();
        }
    }
    dialogEndLine.clear();
}

void VModelingEndLine::AddToFile(){
    VPointF point = VAbstractTool::data.GetModelingPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "endLine");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "mx", toMM(point.mx()));
    AddAttribute(domElement, "my", toMM(point.my()));

    AddAttribute(domElement, "typeLine", typeLine);
    AddAttribute(domElement, "length", formula);
    AddAttribute(domElement, "angle", angle);
    AddAttribute(domElement, "basePoint", basePointId);

    AddToModeling(domElement);
}
