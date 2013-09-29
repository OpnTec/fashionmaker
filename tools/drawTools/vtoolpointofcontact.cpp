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

#include "vtoolpointofcontact.h"

VToolPointOfContact::VToolPointOfContact(VDomDocument *doc, VContainer *data, const qint64 &id,
                                         const QString &radius, const qint64 &center,
                                         const qint64 &firstPointId, const qint64 &secondPointId,
                                         Tool::Sources typeCreation, QGraphicsItem *parent)
    : VToolPoint(doc, data, id, parent), radius(radius), center(center), firstPointId(firstPointId),
      secondPointId(secondPointId), dialogPointOfContact(QSharedPointer<DialogPointOfContact>()){

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VToolPointOfContact::setDialog(){
    Q_ASSERT(!dialogPointOfContact.isNull());
    if(!dialogPointOfContact.isNull()){
        VPointF p = VAbstractTool::data.GetPoint(id);
        dialogPointOfContact->setRadius(radius);
        dialogPointOfContact->setCenter(center, id);
        dialogPointOfContact->setFirstPoint(firstPointId, id);
        dialogPointOfContact->setSecondPoint(secondPointId, id);
        dialogPointOfContact->setPointName(p.name());
    }
}

QPointF VToolPointOfContact::FindPoint(const qreal &radius, const QPointF &center, const QPointF &firstPoint,
                                       const QPointF &secondPoint){
    QPointF pArc;
    qreal s = 0.0, s_x, s_y, step = 0.01, distans;
    while( s < 1){
        s_x = secondPoint.x()-(qAbs(secondPoint.x()-firstPoint.x()))*s;
        s_y = secondPoint.y()-(qAbs(secondPoint.y()-firstPoint.y()))*s;
        distans = QLineF(center.x(), center.y(), s_x, s_y).length();
        if(ceil(distans*10) == ceil(radius*10)){
            pArc.rx() = s_x;
            pArc.ry() = s_y;
            break;
        }
        if(distans<radius){
            pArc.rx() = s_x;
            pArc.ry() = s_y;
        }
        s = s + step;
    }
    return pArc;
}

void VToolPointOfContact::Create(QSharedPointer<DialogPointOfContact> &dialog, VMainGraphicsScene *scene,
                                 VDomDocument *doc, VContainer *data){
    QString radius = dialog->getRadius();
    qint64 center = dialog->getCenter();
    qint64 firstPointId = dialog->getFirstPoint();
    qint64 secondPointId = dialog->getSecondPoint();
    QString pointName = dialog->getPointName();
    Create(0, radius, center, firstPointId, secondPointId, pointName, 5, 10, scene, doc, data,
           Document::FullParse, Tool::FromGui);
}

void VToolPointOfContact::Create(const qint64 _id, const QString &radius, const qint64 &center,
                                 const qint64 &firstPointId, const qint64 &secondPointId,
                                 const QString &pointName, const qreal &mx, const qreal &my,
                                 VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data,
                                 const Document::Documents &parse, Tool::Sources typeCreation){
    VPointF centerP = data->GetPoint(center);
    VPointF firstP = data->GetPoint(firstPointId);
    VPointF secondP = data->GetPoint(secondPointId);

    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(radius, &errorMsg);
    if(errorMsg.isEmpty()){
        QPointF fPoint = VToolPointOfContact::FindPoint(result*PrintDPI/25.4, centerP.toQPointF(),
                                                         firstP.toQPointF(), secondP.toQPointF());
        qint64 id =  _id;
        if(typeCreation == Tool::FromGui){
            id = data->AddPoint(VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
        } else {
            data->UpdatePoint(id,VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            if(parse != Document::FullParse){
                QMap<qint64, VDataTool*>* tools = doc->getTools();
                VDataTool *tool = tools->value(id);
                if(tool != 0){
                    tool->VDataTool::setData(data);
                    data->IncrementReferens(id, Scene::Point);
                }
            }
        }
        VDrawTool::AddRecord(id, Tool::PointOfContact, doc);
        if(parse == Document::FullParse){
            VToolPointOfContact *point = new VToolPointOfContact(doc, data, id, radius, center,
                                                                 firstPointId, secondPointId, typeCreation);
            scene->addItem(point);
            connect(point, &VToolPointOfContact::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(point, &VToolPointOfContact::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            tools->insert(id,point);
        }
    }
}

void VToolPointOfContact::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        radius = domElement.attribute("radius", "");
        center = domElement.attribute("center", "").toLongLong();
        firstPointId = domElement.attribute("firstPoint", "").toLongLong();
        secondPointId = domElement.attribute("secondPoint", "").toLongLong();
    }
    RefreshPointGeometry(VAbstractTool::data.GetPoint(id));
}

void VToolPointOfContact::FullUpdateFromGui(int result){
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

void VToolPointOfContact::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    VPointF point = VDrawTool::data.GetPoint(id);
    if(point.referens() > 1){
        ContextMenu(dialogPointOfContact, this, event, false);
    } else {
        ContextMenu(dialogPointOfContact, this, event);
    }
}

void VToolPointOfContact::AddToFile(){
    VPointF point = VAbstractTool::data.GetPoint(id);
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

    AddToCalculation(domElement);
}
