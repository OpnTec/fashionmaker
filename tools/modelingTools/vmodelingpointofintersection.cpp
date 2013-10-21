#include "vmodelingpointofintersection.h"

VModelingPointOfIntersection::VModelingPointOfIntersection(VDomDocument *doc, VContainer *data, const qint64 &id,
                                                           const qint64 &firstPointId, const qint64 &secondPointId,
                                                           Tool::Sources typeCreation, QGraphicsItem *parent)
    :VModelingPoint(doc, data, id, parent), firstPointId(firstPointId), secondPointId(secondPointId),
    dialogPointOfIntersection(QSharedPointer<DialogPointOfIntersection>()) {
    ignoreFullUpdate = true;
    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VModelingPointOfIntersection::setDialog(){
    Q_ASSERT(!dialogPointOfIntersection.isNull());
    VPointF p = VAbstractTool::data.GetPoint(id);
    dialogPointOfIntersection->setFirstPointId(firstPointId, id);
    dialogPointOfIntersection->setSecondPointId(secondPointId, id);
    dialogPointOfIntersection->setPointName(p.name());
}

VModelingPointOfIntersection *VModelingPointOfIntersection::Create(QSharedPointer<DialogPointOfIntersection> &dialog,
                                                                   VDomDocument *doc, VContainer *data){
    qint64 firstPointId = dialog->getFirstPointId();
    qint64 secondPointId = dialog->getSecondPointId();
    QString pointName = dialog->getPointName();
    return Create(0, pointName, firstPointId, secondPointId, 5, 10, doc, data, Document::FullParse, Tool::FromGui);
}

VModelingPointOfIntersection *VModelingPointOfIntersection::Create(const qint64 _id, const QString &pointName,
                                                                   const qint64 &firstPointId,
                                                                   const qint64 &secondPointId, const qreal &mx,
                                                                   const qreal &my, VDomDocument *doc,
                                                                   VContainer *data,
                                                                   const Document::Documents &parse,
                                                                   Tool::Sources typeCreation){
    VModelingPointOfIntersection *tool = 0;
    VPointF firstPoint = data->GetPoint(firstPointId);
    VPointF secondPoint = data->GetPoint(secondPointId);

    QPointF point(firstPoint.x(), secondPoint.y());
    qint64 id = _id;
    if(typeCreation == Tool::FromGui){
        id = data->AddPoint(VPointF(point.x(), point.y(), pointName, mx, my));
    } else {
        data->UpdatePoint(id, VPointF(point.x(), point.y(), pointName, mx, my));
        if(parse != Document::FullParse){
            doc->UpdateToolData(id, data);
        }
    }
    if(parse == Document::FullParse){
        tool = new VModelingPointOfIntersection(doc, data, id, firstPointId, secondPointId, typeCreation);
        doc->AddTool(id, tool);
        doc->IncrementReferens(firstPointId);
        doc->IncrementReferens(secondPointId);
    }
    return tool;
}

void VModelingPointOfIntersection::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        firstPointId = domElement.attribute("firstPoint", "").toLongLong();
        secondPointId = domElement.attribute("secondPoint", "").toLongLong();
    }
    VModelingPoint::RefreshPointGeometry(VModelingTool::data.GetPoint(id));
}

void VModelingPointOfIntersection::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", dialogPointOfIntersection->getPointName());
            domElement.setAttribute("firstPoint", QString().setNum(dialogPointOfIntersection->getFirstPointId()));
            domElement.setAttribute("secondPoint", QString().setNum(dialogPointOfIntersection->getSecondPointId()));
            emit FullUpdateTree();
        }
    }
    dialogPointOfIntersection.clear();
}

void VModelingPointOfIntersection::RemoveReferens(){
    doc->DecrementReferens(firstPointId);
    doc->DecrementReferens(secondPointId);
}

void VModelingPointOfIntersection::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogPointOfIntersection, this, event);
}

void VModelingPointOfIntersection::AddToFile(){
    VPointF point = VAbstractTool::data.GetPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "pointOfIntersection");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "mx", toMM(point.mx()));
    AddAttribute(domElement, "my", toMM(point.my()));

    AddAttribute(domElement, "firstPoint", firstPointId);
    AddAttribute(domElement, "secondPoint", secondPointId);

    AddToModeling(domElement);
}
