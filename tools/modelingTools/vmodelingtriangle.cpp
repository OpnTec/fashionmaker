#include "vmodelingtriangle.h"
#include "../drawTools/vtooltriangle.h"

VModelingTriangle::VModelingTriangle(VDomDocument *doc, VContainer *data, const qint64 &id,
                                     const qint64 &axisP1Id, const qint64 &axisP2Id,
                                     const qint64 &firstPointId, const qint64 &secondPointId,
                                     Tool::Sources typeCreation, QGraphicsItem *parent)
    :VModelingPoint(doc, data, id, parent), axisP1Id(axisP1Id), axisP2Id(axisP2Id),
      firstPointId(firstPointId), secondPointId(secondPointId),
      dialogTriangle(QSharedPointer<DialogTriangle>()) {
    if(typeCreation == Tool::FromGui){
      AddToFile();
    }
}

void VModelingTriangle::setDialog(){
    Q_ASSERT(!dialogTriangle.isNull());
    VPointF p = VAbstractTool::data.GetPoint(id);
    dialogTriangle->setAxisP1Id(axisP1Id, id);
    dialogTriangle->setAxisP2Id(axisP2Id, id);
    dialogTriangle->setFirstPointId(firstPointId, id);
    dialogTriangle->setSecondPointId(secondPointId, id);
    dialogTriangle->setPointName(p.name());
}

VModelingTriangle *VModelingTriangle::Create(QSharedPointer<DialogTriangle> &dialog, VDomDocument *doc,
                                             VContainer *data){
    qint64 axisP1Id = dialog->getAxisP1Id();
    qint64 axisP2Id = dialog->getAxisP2Id();
    qint64 firstPointId = dialog->getFirstPointId();
    qint64 secondPointId = dialog->getSecondPointId();
    QString pointName = dialog->getPointName();
    return Create(0, pointName, axisP1Id, axisP2Id, firstPointId, secondPointId, 5, 10, doc, data,
                  Document::FullParse, Tool::FromGui);

}

VModelingTriangle *VModelingTriangle::Create(const qint64 _id, const QString &pointName,
                                             const qint64 &axisP1Id, const qint64 &axisP2Id,
                                             const qint64 &firstPointId, const qint64 &secondPointId,
                                             const qreal &mx, const qreal &my, VDomDocument *doc,
                                             VContainer *data, const Document::Documents &parse,
                                             Tool::Sources typeCreation){
    VModelingTriangle *tool = 0;
    VPointF axisP1 = data->GetPoint(axisP1Id);
    VPointF axisP2 = data->GetPoint(axisP2Id);
    VPointF firstPoint = data->GetPoint(firstPointId);
    VPointF secondPoint = data->GetPoint(secondPointId);

    QPointF point = VToolTriangle::FindPoint(axisP1.toQPointF(), axisP2.toQPointF(), firstPoint.toQPointF(),
                                             secondPoint.toQPointF());
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
        tool = new VModelingTriangle(doc, data, id, axisP1Id, axisP2Id, firstPointId,
                                                 secondPointId, typeCreation);
        doc->AddTool(id, tool);
        doc->IncrementReferens(axisP1Id);
        doc->IncrementReferens(axisP2Id);
        doc->IncrementReferens(firstPointId);
        doc->IncrementReferens(secondPointId);
    }
    return tool;
}

void VModelingTriangle::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        axisP1Id = domElement.attribute("axisP1", "").toLongLong();
        axisP2Id = domElement.attribute("axisP2", "").toLongLong();
        firstPointId = domElement.attribute("firstPoint", "").toLongLong();
        secondPointId = domElement.attribute("secondPoint", "").toLongLong();
    }
    VModelingPoint::RefreshPointGeometry(VModelingTool::data.GetPoint(id));
}

void VModelingTriangle::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", dialogTriangle->getPointName());
            domElement.setAttribute("axisP1", QString().setNum(dialogTriangle->getAxisP1Id()));
            domElement.setAttribute("axisP2", QString().setNum(dialogTriangle->getAxisP2Id()));
            domElement.setAttribute("firstPoint", QString().setNum(dialogTriangle->getFirstPointId()));
            domElement.setAttribute("secondPoint", QString().setNum(dialogTriangle->getSecondPointId()));
            emit FullUpdateTree();
        }

    }
    dialogTriangle.clear();
}

void VModelingTriangle::RemoveReferens(){
    doc->DecrementReferens(axisP1Id);
    doc->DecrementReferens(axisP2Id);
    doc->DecrementReferens(firstPointId);
    doc->DecrementReferens(secondPointId);
}

void VModelingTriangle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogTriangle, this, event);
}

void VModelingTriangle::AddToFile(){
    VPointF point = VAbstractTool::data.GetPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "triangle");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "mx", toMM(point.mx()));
    AddAttribute(domElement, "my", toMM(point.my()));

    AddAttribute(domElement, "axisP1", axisP1Id);
    AddAttribute(domElement, "axisP2", axisP2Id);
    AddAttribute(domElement, "firstPoint", firstPointId);
    AddAttribute(domElement, "secondPoint", secondPointId);

    AddToModeling(domElement);
}
