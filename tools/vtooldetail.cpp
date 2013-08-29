#include "vtooldetail.h"
#include "vmodelingpoint.h"
#include "vmodelingarc.h"
#include "vmodelingspline.h"
#include "vmodelingsplinepath.h"
#include <QDebug>

VToolDetail::VToolDetail(VDomDocument *doc, VContainer *data, const qint64 &id, VDetail &oldDetail,
                         Tool::Enum typeCreation, Document::Enum parse,
                         VMainGraphicsScene *scene, QGraphicsItem *parent)
    :VAbstractTool(doc, data, id, Draw::Modeling), QGraphicsPathItem(parent),
      dialogDetail(QSharedPointer<DialogDetail>()){
    VDetail detail = data->GetDetail(id);
    for(qint32 i = 0; i< detail.CountNode(); ++i){
        switch(detail[i].getTypeTool()){
            case(Scene::Point):{
            VModelingPoint *point = VModelingPoint::Create(doc, data, detail[i].getId(), oldDetail[i].getId(),
                                                           detail[i].getMode(), parse, typeCreation);
            connect(point, &VModelingPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            tools->insert(detail[i].getId(),point);
            point->setParentItem(this);
            }
                break;
            case(Scene::Arc):{
               VModelingArc *arc = VModelingArc::Create(doc, data, detail[i].getId(), oldDetail[i].getId(),
                                                        detail[i].getMode(), parse, typeCreation);
               connect(arc, &VModelingArc::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
               QMap<qint64, VDataTool*>* tools = doc->getTools();
               tools->insert(detail[i].getId(), arc);
               arc->setParentItem(this);
            }
                break;
            case(Scene::Spline):{
            VModelingSpline *spl = VModelingSpline::Create(doc, data, detail[i].getId(), oldDetail[i].getId(),
                                                           detail[i].getMode(), parse, typeCreation);
            connect(spl, &VModelingSpline::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            tools->insert(detail[i].getId(), spl);
            spl->setParentItem(this);
            }
                break;
            case(Scene::SplinePath):{
            VModelingSplinePath *splPath = VModelingSplinePath::Create(doc, data, detail[i].getId(),
                                                                       oldDetail[i].getId(),
                                                                       detail[i].getMode(), parse,
                                                                       typeCreation);
            connect(splPath, &VModelingSplinePath::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            tools->insert(detail[i].getId(), splPath);
            splPath->setParentItem(this);
            }
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

void VToolDetail::Create(QSharedPointer<DialogDetail> &dialog, VMainGraphicsScene *scene, VDomDocument *doc,
    VContainer *data){
    VDetail detail = dialog->getDetails();
    VDetail det;
    for(qint32 i = 0; i< detail.CountNode(); ++i){
        qint64 id = 0;
        switch(detail[i].getTypeTool()){
        case(Scene::Point):{
            VPointF point;
            if(detail[i].getMode() == Draw::Calculation){
                point = data->GetPoint(detail[i].getId());
            } else {
                point = data->GetModelingPoint(detail[i].getId());
            }
            id = data->AddModelingPoint(point);
        }
            break;
        case(Scene::Arc):{
            VArc arc;
            if(detail[i].getMode() == Draw::Calculation){
                arc = data->GetArc(detail[i].getId());
            } else {
                arc = data->GetModelingArc(detail[i].getId());
            }
            id = data->AddModelingArc(arc);
        }
            break;
        case(Scene::Spline):{
            VSpline spline;
            if(detail[i].getMode() == Draw::Calculation){
                spline = data->GetSpline(detail[i].getId());
            } else {
                spline = data->GetModelingSpline(detail[i].getId());
            }
            id = data->AddModelingSpline(spline);
        }
            break;
        case(Scene::SplinePath):{
            VSplinePath splinePath;
            if(detail[i].getMode() == Draw::Calculation){
                splinePath = data->GetSplinePath(detail[i].getId());
            } else {
                splinePath = data->GetModelingSplinePath(detail[i].getId());
            }
            id = data->AddModelingSplinePath(splinePath);
        }
            break;
        case(Scene::Line):
            break;
        }
        VNodeDetail node(id, detail[i].getTypeTool(), detail[i].getMode());
        det.append(node);
    }
    det.setName(detail.getName());
    Create(0, det, detail, scene, doc, data, Document::FullParse, Tool::FromGui);
}

void VToolDetail::Create(const qint64 _id, VDetail &newDetail, VDetail &oldDetail,
                         VMainGraphicsScene *scene, VDomDocument *doc,
                         VContainer *data, Document::Enum parse, Tool::Enum typeCreation){
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
        //connect(detail, &VToolBisector::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
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
        qDebug()<<newPos;
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

void VToolDetail::AddNode(QDomElement &domElement, VNodeDetail &node){
    QDomElement nod = doc->createElement("node");

    AddAttribute(nod, "id", node.getId());
    switch(node.getTypeTool()){
    case(Scene::Line):
            break;
    case(Scene::Point):
        AddAttribute(nod, "type", "Point");
            break;
    case(Scene::Arc):
        AddAttribute(nod, "type", "Arc");
            break;
    case(Scene::Spline):
        AddAttribute(nod, "type", "Spline");
            break;
    case(Scene::SplinePath):
        AddAttribute(nod, "type", "SplinePath");
        break;
    }

    domElement.appendChild(nod);
}

void VToolDetail::RefreshGeometry(){
    QPainterPath path = VAbstractTool::data.ContourPath(id);
    this->setPath(path);
}
