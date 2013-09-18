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

#include "vtoolendline.h"
#include <QDebug>
#include <QMenu>
#include "widgets/vmaingraphicsscene.h"

VToolEndLine::VToolEndLine(VDomDocument *doc, VContainer *data, const qint64 &id,  const QString &typeLine,
                           const QString &formula, const qreal &angle, const qint64 &basePointId,
                           Tool::Enum typeCreation, QGraphicsItem *parent):
    VToolLinePoint(doc, data, id, typeLine, formula, basePointId, angle, parent),
    dialogEndLine(QSharedPointer<DialogEndLine>()){

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VToolEndLine::setDialog(){
    Q_ASSERT(!dialogEndLine.isNull());
    if(!dialogEndLine.isNull()){
        VPointF p = VAbstractTool::data.GetPoint(id);
        dialogEndLine->setTypeLine(typeLine);
        dialogEndLine->setFormula(formula);
        dialogEndLine->setAngle(angle);
        dialogEndLine->setBasePointId(basePointId, id);
        dialogEndLine->setPointName(p.name());
    }
}

void VToolEndLine::Create(QSharedPointer<DialogEndLine> &dialog, VMainGraphicsScene *scene,
                          VDomDocument *doc, VContainer *data){
    QString pointName = dialog->getPointName();
    QString typeLine = dialog->getTypeLine();
    QString formula = dialog->getFormula();
    qreal angle = dialog->getAngle();
    qint64 basePointId = dialog->getBasePointId();
    Create(0, pointName, typeLine, formula, angle, basePointId, 5, 10, scene, doc, data, Document::FullParse,
           Tool::FromGui);
}

void VToolEndLine::Create(const qint64 _id, const QString &pointName, const QString &typeLine,
                          const QString &formula, const qreal &angle, const qint64 &basePointId,
                          const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VDomDocument *doc,
                          VContainer *data, Document::Enum parse, Tool::Enum typeCreation){

    VPointF basePoint = data->GetPoint(basePointId);
    QLineF line = QLineF(basePoint.toQPointF(), QPointF(basePoint.x()+100, basePoint.y()));
    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if(errorMsg.isEmpty()){
        line.setLength(result*PrintDPI/25.4);
        line.setAngle(angle);
        qint64 id = _id;
        if(typeCreation == Tool::FromGui){
            id = data->AddPoint(VPointF(line.p2().x(), line.p2().y(), pointName, mx, my));
        } else {
            data->UpdatePoint(id, VPointF(line.p2().x(), line.p2().y(), pointName, mx, my));
            if(parse != Document::FullParse){
                QMap<qint64, VDataTool*>* tools = doc->getTools();
                VDataTool *tool = tools->value(id);
                if(tool != 0){
                    tool->VDataTool::setData(data);
                    data->IncrementReferens(id, Scene::Point);
                }
            }
        }
        data->AddLine(basePointId, id);
        VDrawTool::AddRecord(id, Tools::EndLineTool, doc);
        if(parse == Document::FullParse){
            VToolEndLine *point = new VToolEndLine(doc, data, id, typeLine, formula, angle,
                                                   basePointId, typeCreation);
            scene->addItem(point);
            connect(point, &VToolPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(point, &VToolPoint::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            tools->insert(id,point);  
        }
    }
}

void VToolEndLine::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        typeLine = domElement.attribute("typeLine", "");
        formula = domElement.attribute("length", "");
        basePointId = domElement.attribute("basePoint", "").toLongLong();
        angle = domElement.attribute("angle", "").toInt();
    }
    RefreshGeometry();
}

void VToolEndLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogEndLine, this, event);
}

void VToolEndLine::FullUpdateFromGui(int result){
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

void VToolEndLine::AddToFile(){
    VPointF point = VAbstractTool::data.GetPoint(id);
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

    AddToCalculation(domElement);
}

