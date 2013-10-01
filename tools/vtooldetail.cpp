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

#include "vtooldetail.h"
#include "nodeDetails/nodedetails.h"
#include <QDebug>
#include "modelingTools/vmodelingtool.h"
#include "modelingTools/modelingtools.h"

VToolDetail::VToolDetail(VDomDocument *doc, VContainer *data, const qint64 &id, Tool::Sources typeCreation,
                         VMainGraphicsScene *scene, QGraphicsItem *parent)
    :VAbstractTool(doc, data, id), QGraphicsPathItem(parent), dialogDetail(QSharedPointer<DialogDetail>()),
     sceneDetails(scene){
    VDetail detail = data->GetDetail(id);
    QMap<qint64, VDataTool*>* tools = doc->getTools();
    Q_CHECK_PTR(tools);
    for(qint32 i = 0; i< detail.CountNode(); ++i){
        switch(detail[i].getTypeTool()){
        case(Tool::NodePoint):{
            VNodePoint *point = qobject_cast<VNodePoint*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(point);
            connect(point, &VNodePoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem); 
            point->setParentItem(this);
            break;
        }
        case(Tool::NodeArc):{
            VNodeArc *arc = qobject_cast<VNodeArc*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(arc);
            connect(arc, &VNodeArc::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            arc->setParentItem(this);
            break;
        }
        case(Tool::NodeSpline):{
            VNodeSpline *spl = qobject_cast<VNodeSpline*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(spl);
            connect(spl, &VNodeSpline::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            spl->setParentItem(this);
            break;
        }
        case(Tool::NodeSplinePath):{
            VNodeSplinePath *splPath = qobject_cast<VNodeSplinePath*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(splPath);
            connect(splPath, &VNodeSplinePath::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            splPath->setParentItem(this);
            break;
        }
        case(Tool::AlongLineTool):{
            VModelingAlongLine *tool = qobject_cast<VModelingAlongLine*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingAlongLine::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(tool, &VModelingAlongLine::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
            tool->setParentItem(this);
            break;
        }
        case(Tool::ArcTool):{
            VModelingArc *tool = qobject_cast<VModelingArc*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingArc::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(tool, &VModelingArc::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
            tool->setParentItem(this);
            break;
        }
        case(Tool::BisectorTool):{
            VModelingBisector *tool = qobject_cast<VModelingBisector*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingBisector::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(tool, &VModelingBisector::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
            tool->setParentItem(this);
            break;
        }
        case(Tool::EndLineTool):{
            VModelingEndLine *tool = qobject_cast<VModelingEndLine*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingEndLine::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(tool, &VModelingEndLine::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
            tool->setParentItem(this);
            break;
        }
        case(Tool::LineIntersectTool):{
            VModelingLineIntersect *tool = qobject_cast<VModelingLineIntersect*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingLineIntersect::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(tool, &VModelingLineIntersect::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
            tool->setParentItem(this);
            break;
        }
        case(Tool::LineTool):{
            VModelingLine *tool = qobject_cast<VModelingLine*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingLine::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(tool, &VModelingLine::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
            tool->setParentItem(this);
            break;
        }
        case(Tool::NormalTool):{
            VModelingNormal *tool = qobject_cast<VModelingNormal*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingNormal::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(tool, &VModelingNormal::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
            tool->setParentItem(this);
            break;
        }
        case(Tool::PointOfContact):{
            VModelingPointOfContact *tool = qobject_cast<VModelingPointOfContact*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingPointOfContact::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(tool, &VModelingPointOfContact::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
            tool->setParentItem(this);
            break;
        }
        case(Tool::ShoulderPointTool):{
            VModelingShoulderPoint *tool = qobject_cast<VModelingShoulderPoint*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingShoulderPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(tool, &VModelingShoulderPoint::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
            tool->setParentItem(this);
            break;
        }
        case(Tool::SplinePathTool):{
            VModelingSplinePath *tool = qobject_cast<VModelingSplinePath*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingSplinePath::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(tool, &VModelingSplinePath::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
            tool->setParentItem(this);
            break;
        }
        case(Tool::SplineTool):{
            VModelingSpline *tool = qobject_cast<VModelingSpline*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingSpline::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(tool, &VModelingSpline::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
            tool->setParentItem(this);
            break;
        }
        default:
            qWarning()<<"Get wrong tool type. Ignore.";
            break;
        }
        doc->IncrementReferens(detail[i].getId());
    }
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    RefreshGeometry();
    this->setPos(detail.getMx(), detail.getMy());
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    if(typeCreation == Tool::FromGui){
       AddToFile();
    }
}

void VToolDetail::setDialog(){
    Q_ASSERT(!dialogDetail.isNull());
    if(!dialogDetail.isNull()){
        VDetail detail = VAbstractTool::data.GetDetail(id);
        dialogDetail->setDetails(detail);
    }
}

void VToolDetail::Create(QSharedPointer<DialogDetail> &dialog, VMainGraphicsScene *scene,
                         VDomDocument *doc, VContainer *data){
    VDetail detail = dialog->getDetails();
    VDetail det;
    for(qint32 i = 0; i< detail.CountNode(); ++i){
        qint64 id = 0;
        switch(detail[i].getTypeTool()){
        case(Tool::NodePoint):{
            VPointF point;
            if(detail[i].getMode() == Draw::Calculation){
                point = data->GetPoint(detail[i].getId());
            } else {
                point = data->GetModelingPoint(detail[i].getId());
            }
            id = data->AddModelingPoint(point);
        }
            break;
        case(Tool::NodeArc):{
            VArc arc;
            if(detail[i].getMode() == Draw::Calculation){
                arc = data->GetArc(detail[i].getId());
            } else {
                arc = data->GetModelingArc(detail[i].getId());
            }
            id = data->AddModelingArc(arc);
        }
            break;
        case(Tool::NodeSpline):{
            VSpline spline;
            if(detail[i].getMode() == Draw::Calculation){
                spline = data->GetSpline(detail[i].getId());
            } else {
                spline = data->GetModelingSpline(detail[i].getId());
            }
            id = data->AddModelingSpline(spline);
        }
            break;
        case(Tool::NodeSplinePath):{
            VSplinePath splinePath;
            if(detail[i].getMode() == Draw::Calculation){
                splinePath = data->GetSplinePath(detail[i].getId());
            } else {
                splinePath = data->GetModelingSplinePath(detail[i].getId());
            }
            id = data->AddModelingSplinePath(splinePath);
        }
            break;
        default:
            qWarning()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
            break;
        }
        VNodeDetail node(id, detail[i].getTypeTool(), detail[i].getMode(), NodeDetail::Contour);
        det.append(node);
    }
    det.setName(detail.getName());
    Create(0, det, scene, doc, data, Document::FullParse, Tool::FromGui);
}

void VToolDetail::Create(const qint64 _id, VDetail &newDetail, VMainGraphicsScene *scene, VDomDocument *doc,
                         VContainer *data, const Document::Documents &parse, Tool::Sources typeCreation){
    qint64 id = _id;
    if(typeCreation == Tool::FromGui){
        id = data->AddDetail(newDetail);
    } else {
        data->UpdateDetail(id, newDetail);
        if(parse != Document::FullParse){
            doc->UpdateToolData(id, data);
        }
    }
    if(parse == Document::FullParse){
        VToolDetail *detail = new VToolDetail(doc, data, id, typeCreation, scene);
        scene->addItem(detail);
        connect(detail, &VToolDetail::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(detail, &VToolDetail::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
        QMap<qint64, VDataTool*>* tools = doc->getTools();
        tools->insert(id, detail);
    }
}

void VToolDetail::FullUpdateFromFile(){
    RefreshGeometry();
}

void VToolDetail::FullUpdateFromGui(int result){
    Q_UNUSED(result);
}

void VToolDetail::AddToFile(){
    VDetail detail = VAbstractTool::data.GetDetail(id);
    QDomElement domElement = doc->createElement("detail");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "name", detail.getName());
    AddAttribute(domElement, "mx", detail.getMx());
    AddAttribute(domElement, "my", detail.getMy());

    for(qint32 i = 0; i < detail.CountNode(); ++i){
       AddNode(domElement, detail[i]);
    }

    QDomElement element;
    bool ok = doc->GetActivDetailsElement(element);
    if(ok){
        element.appendChild(domElement);
    }
}

QVariant VToolDetail::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value){
    if (change == ItemPositionHasChanged && scene()) {
        // value - это новое положение.
        QPointF newPos = value.toPointF();
        //qDebug()<<newPos;
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("mx", QString().setNum(toMM(newPos.x())));
            domElement.setAttribute("my", QString().setNum(toMM(newPos.y())));
            //I don't now why but signal does not work.
            doc->FullUpdateTree();
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

void VToolDetail::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit ChoosedTool(id, Scene::Detail);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VToolDetail::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    QMenu menu;
    //QAction *actionOption = menu.addAction(tr("Options"));
    QAction *actionRemove = menu.addAction(tr("Delete"));
    if(_referens > 1){
        actionRemove->setEnabled(false);
    } else {
        actionRemove->setEnabled(true);
    }
    QAction *selectedAction = menu.exec(event->screenPos());
//    if(selectedAction == actionOption){
//        dialog = QSharedPointer<Dialog>(new Dialog(getData()));

//        connect(qobject_cast< VMainGraphicsScene * >(tool->scene()), &VMainGraphicsScene::ChoosedObject,
//                dialog.data(), &Dialog::ChoosedObject);
//        connect(dialog.data(), &Dialog::DialogClosed, tool,
//                &Tool::FullUpdateFromGui);
//        connect(doc, &VDomDocument::FullUpdateFromFile, dialog.data(), &Dialog::UpdateList);

//        tool->setDialog();

//        dialog->show();
//    }
    if(selectedAction == actionRemove){
        //deincrement referens
        RemoveReferens();
        //remove form xml file
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            QDomNode element = domElement.parentNode();
            if(!element.isNull()){
                element.removeChild(domElement);
                //update xml file
                emit FullUpdateTree();
                //remove form scene
                emit RemoveTool(this);
            }
        }
    }
}

void VToolDetail::RemoveReferens(){
    VDetail detail = VAbstractTool::data.GetDetail(id);
    for(qint32 i = 0; i< detail.CountNode(); ++i){
        doc->DecrementReferens(detail[i].getId());
    }
}

void VToolDetail::AddNode(QDomElement &domElement, VNodeDetail &node){
    QDomElement nod = doc->createElement("node");

    AddAttribute(nod, "idObject", node.getId());
    if(node.getTypeNode() == NodeDetail::Contour){
        AddAttribute(nod, "nodeType", "Contour");
    } else {
        AddAttribute(nod, "nodeType", "Modeling");
    }
    switch(node.getTypeTool()){
    case(Tool::AlongLineTool):
        AddAttribute(nod, "type", "AlongLineTool");
        break;
    case(Tool::ArcTool):
        AddAttribute(nod, "type", "ArcTool");
        break;
    case(Tool::BisectorTool):
        AddAttribute(nod, "type", "BisectorTool");
        break;
    case(Tool::EndLineTool):
        AddAttribute(nod, "type", "EndLineTool");
        break;
    case(Tool::LineIntersectTool):
        AddAttribute(nod, "type", "LineIntersectTool");
        break;
    case(Tool::LineTool):
        AddAttribute(nod, "type", "LineTool");
        break;
    case(Tool::NodeArc):
        AddAttribute(nod, "type", "NodeArc");
        break;
    case(Tool::NodePoint):
        AddAttribute(nod, "type", "NodePoint");
        break;
    case(Tool::NodeSpline):
        AddAttribute(nod, "type", "NodeSpline");
        break;
    case(Tool::NodeSplinePath):
        AddAttribute(nod, "type", "NodeSplinePath");
        break;
    case(Tool::NormalTool):
        AddAttribute(nod, "type", "NormalTool");
        break;
    case(Tool::PointOfContact):
        AddAttribute(nod, "type", "PointOfContact");
        break;
    case(Tool::ShoulderPointTool):
        AddAttribute(nod, "type", "ShoulderPointTool");
        break;
    case(Tool::SplinePathTool):
        AddAttribute(nod, "type", "SplinePathTool");
        break;
    case(Tool::SplineTool):
        AddAttribute(nod, "type", "SplineTool");
        break;
    default:
        qWarning()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
        break;
    }
    domElement.appendChild(nod);
}

void VToolDetail::RefreshGeometry(){
    QPainterPath path = VAbstractTool::data.ContourPath(id);
    this->setPath(path);
}
