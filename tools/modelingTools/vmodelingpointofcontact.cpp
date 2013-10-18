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

#include "vmodelingpointofcontact.h"
#include "../drawTools/vtoolpointofcontact.h"

VModelingPointOfContact::VModelingPointOfContact(VDomDocument *doc, VContainer *data, const qint64 &id,
                                         const QString &radius, const qint64 &center,
                                         const qint64 &firstPointId, const qint64 &secondPointId,
                                         Tool::Sources typeCreation, QGraphicsItem *parent)
    : VModelingPoint(doc, data, id, parent), radius(radius), center(center), firstPointId(firstPointId),
      secondPointId(secondPointId), dialogPointOfContact(QSharedPointer<DialogPointOfContact>()){

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VModelingPointOfContact::setDialog(){
    Q_ASSERT(!dialogPointOfContact.isNull());
    if(!dialogPointOfContact.isNull()){
        VPointF p = VAbstractTool::data.GetModelingPoint(id);
        dialogPointOfContact->setRadius(radius);
        dialogPointOfContact->setCenter(center, id);
        dialogPointOfContact->setFirstPoint(firstPointId, id);
        dialogPointOfContact->setSecondPoint(secondPointId, id);
        dialogPointOfContact->setPointName(p.name());
    }
}

VModelingPointOfContact *VModelingPointOfContact::Create(QSharedPointer<DialogPointOfContact> &dialog,
                                                         VDomDocument *doc, VContainer *data){
    QString radius = dialog->getRadius();
    qint64 center = dialog->getCenter();
    qint64 firstPointId = dialog->getFirstPoint();
    qint64 secondPointId = dialog->getSecondPoint();
    QString pointName = dialog->getPointName();
    return Create(0, radius, center, firstPointId, secondPointId, pointName, 5, 10, doc, data,
                  Document::FullParse, Tool::FromGui);
}

VModelingPointOfContact *VModelingPointOfContact::Create(const qint64 _id, const QString &radius,
                                                         const qint64 &center, const qint64 &firstPointId,
                                                         const qint64 &secondPointId,
                                                         const QString &pointName, const qreal &mx,
                                                         const qreal &my, VDomDocument *doc,
                                                         VContainer *data, const Document::Documents &parse,
                                                         Tool::Sources typeCreation){
    VModelingPointOfContact *point = 0;
    VPointF centerP = data->GetModelingPoint(center);
    VPointF firstP = data->GetModelingPoint(firstPointId);
    VPointF secondP = data->GetModelingPoint(secondPointId);

    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(radius, &errorMsg);
    if(errorMsg.isEmpty()){
        QPointF fPoint = VToolPointOfContact::FindPoint(toPixel(result), centerP.toQPointF(),
                                                        firstP.toQPointF(), secondP.toQPointF());
        qint64 id =  _id;
        if(typeCreation == Tool::FromGui){
            id = data->AddModelingPoint(VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
        } else {
            data->UpdateModelingPoint(id, VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            if(parse != Document::FullParse){
                doc->UpdateToolData(id, data);
            }
        }
        if(parse == Document::FullParse){
            point = new VModelingPointOfContact(doc, data, id, radius, center, firstPointId, secondPointId,
                                                typeCreation);
            doc->AddTool(id, point);
            doc->IncrementReferens(center);
            doc->IncrementReferens(firstPointId);
            doc->IncrementReferens(secondPointId);
        }
    }
    return point;
}

void VModelingPointOfContact::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        radius = domElement.attribute("radius", "");
        center = domElement.attribute("center", "").toLongLong();
        firstPointId = domElement.attribute("firstPoint", "").toLongLong();
        secondPointId = domElement.attribute("secondPoint", "").toLongLong();
    }
    RefreshPointGeometry(VAbstractTool::data.GetModelingPoint(id));
}

void VModelingPointOfContact::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", dialogPointOfContact->getPointName());
            domElement.setAttribute("radius", dialogPointOfContact->getRadius());
            domElement.setAttribute("center", QString().setNum(dialogPointOfContact->getCenter()));
            domElement.setAttribute("firstPoint", QString().setNum(dialogPointOfContact->getFirstPoint()));
            domElement.setAttribute("secondPoint", QString().setNum(dialogPointOfContact->getSecondPoint()));
            emit FullUpdateTree();
        }
    }
    dialogPointOfContact.clear();
}

void VModelingPointOfContact::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogPointOfContact, this, event);
}

void VModelingPointOfContact::AddToFile(){
    VPointF point = VAbstractTool::data.GetModelingPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "pointOfContact");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "mx", point.mx()/PrintDPI*25.4);
    AddAttribute(domElement, "my", point.my()/PrintDPI*25.4);

    AddAttribute(domElement, "radius", radius);
    AddAttribute(domElement, "center", center);
    AddAttribute(domElement, "firstPoint", firstPointId);
    AddAttribute(domElement, "secondPoint", secondPointId);

    AddToModeling(domElement);
}

void VModelingPointOfContact::RemoveReferens(){
    doc->DecrementReferens(center);
    doc->DecrementReferens(firstPointId);
    doc->DecrementReferens(secondPointId);
}
