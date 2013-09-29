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

VToolDetail::VToolDetail(VDomDocument *doc, VContainer *data, const qint64 &id, VDetail &oldDetail,
                         Tool::Sources typeCreation, const Document::Documents &parse,
                         VMainGraphicsScene *scene, QGraphicsItem *parent)
    :VAbstractTool(doc, data, id), QGraphicsPathItem(parent),
      dialogDetail(QSharedPointer<DialogDetail>()), sceneDetails(scene){
    VDetail detail = data->GetDetail(id);
    for(qint32 i = 0; i< detail.CountNode(); ++i){
        switch(detail[i].getTypeTool()){
        case(Tool::NodePoint):{
            VNodePoint *point = VNodePoint::Create(doc, data, detail[i].getId(), oldDetail[i].getId(),
                                                   detail[i].getMode(), parse, typeCreation);
            connect(point, &VNodePoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            tools->insert(detail[i].getId(),point);
            point->setParentItem(this);
            break;
        }
        case(Tool::NodeArc):{
            VNodeArc *arc = VNodeArc::Create(doc, data, detail[i].getId(), oldDetail[i].getId(),
                                             detail[i].getMode(), parse, typeCreation);
            connect(arc, &VNodeArc::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            tools->insert(detail[i].getId(), arc);
            arc->setParentItem(this);
            break;
        }
        case(Tool::NodeSpline):{
            VNodeSpline *spl = VNodeSpline::Create(doc, data, detail[i].getId(), oldDetail[i].getId(),
                                                   detail[i].getMode(), parse, typeCreation);
            connect(spl, &VNodeSpline::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            tools->insert(detail[i].getId(), spl);
            spl->setParentItem(this);
            break;
        }
        case(Tool::NodeSplinePath):{
            VNodeSplinePath *splPath = VNodeSplinePath::Create(doc, data, detail[i].getId(),
                                                               oldDetail[i].getId(),
                                                               detail[i].getMode(), parse, typeCreation);
            connect(splPath, &VNodeSplinePath::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            tools->insert(detail[i].getId(), splPath);
            splPath->setParentItem(this);
            break;
        }
        case(Tool::AlongLineTool):{
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            VModelingAlongLine *tool = qobject_cast<VModelingAlongLine*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingAlongLine::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            tool->setParentItem(this);
            break;
        }
        case(Tool::ArcTool):{
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            VModelingArc *tool = qobject_cast<VModelingArc*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingArc::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            tool->setParentItem(this);
            break;
        }
        case(Tool::BisectorTool):{
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            VModelingBisector *tool = qobject_cast<VModelingBisector*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingBisector::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            tool->setParentItem(this);
            break;
        }
        case(Tool::EndLineTool):{
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            VModelingEndLine *tool = qobject_cast<VModelingEndLine*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingEndLine::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            tool->setParentItem(this);
            break;
        }
        case(Tool::LineIntersectTool):{
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            VModelingLineIntersect *tool = qobject_cast<VModelingLineIntersect*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingLineIntersect::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            tool->setParentItem(this);
            break;
        }
        case(Tool::LineTool):{
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            VModelingLine *tool = qobject_cast<VModelingLine*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingLine::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            tool->setParentItem(this);
            break;
        }
        case(Tool::NormalTool):{
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            VModelingNormal *tool = qobject_cast<VModelingNormal*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingNormal::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            tool->setParentItem(this);
            break;
        }
        case(Tool::PointOfContact):{
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            VModelingPointOfContact *tool = qobject_cast<VModelingPointOfContact*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingPointOfContact::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            tool->setParentItem(this);
            break;
        }
        case(Tool::ShoulderPointTool):{
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            VModelingShoulderPoint *tool = qobject_cast<VModelingShoulderPoint*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingShoulderPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            tool->setParentItem(this);
            break;
        }
        case(Tool::SplinePathTool):{
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            VModelingSplinePath *tool = qobject_cast<VModelingSplinePath*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingSplinePath::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            tool->setParentItem(this);
            break;
        }
        case(Tool::SplineTool):{
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            VModelingSpline *tool = qobject_cast<VModelingSpline*>(tools->value(detail[i].getId()));
            Q_CHECK_PTR(tool);
            connect(tool, &VModelingSpline::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            tool->setParentItem(this);
            break;
        }
        default:
            qWarning()<<"Get wrong tool type. Ignore.";
            break;
        }
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
    Create(0, det, detail, scene, doc, data, Document::FullParse, Tool::FromGui);
}

void VToolDetail::Create(const qint64 _id, VDetail &newDetail, VDetail &oldDetail,
                         VMainGraphicsScene *scene, VDomDocument *doc,
                         VContainer *data, const Document::Documents &parse, Tool::Sources typeCreation){
    qint64 id = _id;
    if(typeCreation == Tool::FromGui){
        id = data->AddDetail(newDetail);
    } else {
        data->UpdateDetail(id, newDetail);
        if(parse != Document::FullParse){
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            VDataTool *tool = tools->value(id);
            if(tool != 0){
                tool->VDataTool::setData(data);
            }

            for(qint32 i = 0; i< newDetail.CountNode(); ++i){
                VDataTool *tool = tools->value(newDetail[i].getId());
                if(tool != 0){
                    tool->VDataTool::setData(data);
                }
            }
        }
    }
    if(parse == Document::FullParse){
        VToolDetail *detail = new VToolDetail(doc, data, id, oldDetail, typeCreation, parse, scene);
        scene->addItem(detail);
        connect(detail, &VToolDetail::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        //connect(detail, &VToolBisector::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
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
