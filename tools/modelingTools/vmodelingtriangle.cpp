#include "vmodelingtriangle.h"
#include "../drawTools/vtooltriangle.h"

const QString VModelingTriangle::ToolType = QStringLiteral("triangle");

VModelingTriangle::VModelingTriangle(VDomDocument *doc, VContainer *data, const qint64 &id, const qint64 &axisP1Id,
                                     const qint64 &axisP2Id, const qint64 &firstPointId, const qint64 &secondPointId,
                                     Tool::Sources typeCreation, QGraphicsItem *parent)
    :VModelingPoint(doc, data, id, parent), axisP1Id(axisP1Id), axisP2Id(axisP2Id), firstPointId(firstPointId),
      secondPointId(secondPointId), dialogTriangle(QSharedPointer<DialogTriangle>()) {
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
    return Create(0, pointName, axisP1Id, axisP2Id, firstPointId, secondPointId, 5, 10, doc, data, Document::FullParse,
                  Tool::FromGui);
}

VModelingTriangle *VModelingTriangle::Create(const qint64 _id, const QString &pointName, const qint64 &axisP1Id,
                                             const qint64 &axisP2Id, const qint64 &firstPointId,
                                             const qint64 &secondPointId, const qreal &mx, const qreal &my,
                                             VDomDocument *doc, VContainer *data, const Document::Documents &parse,
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
        tool = new VModelingTriangle(doc, data, id, axisP1Id, axisP2Id, firstPointId, secondPointId, typeCreation);
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
        axisP1Id = domElement.attribute(AttrAxisP1, "").toLongLong();
        axisP2Id = domElement.attribute(AttrAxisP2, "").toLongLong();
        firstPointId = domElement.attribute(AttrFirstPoint, "").toLongLong();
        secondPointId = domElement.attribute(AttrSecondPoint, "").toLongLong();
    }
    VModelingPoint::RefreshPointGeometry(VModelingTool::data.GetPoint(id));
}

void VModelingTriangle::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute(AttrName, dialogTriangle->getPointName());
            domElement.setAttribute(AttrAxisP1, QString().setNum(dialogTriangle->getAxisP1Id()));
            domElement.setAttribute(AttrAxisP2, QString().setNum(dialogTriangle->getAxisP2Id()));
            domElement.setAttribute(AttrFirstPoint, QString().setNum(dialogTriangle->getFirstPointId()));
            domElement.setAttribute(AttrSecondPoint, QString().setNum(dialogTriangle->getSecondPointId()));
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
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrName, point.name());
    AddAttribute(domElement, AttrMx, toMM(point.mx()));
    AddAttribute(domElement, AttrMy, toMM(point.my()));

    AddAttribute(domElement, AttrAxisP1, axisP1Id);
    AddAttribute(domElement, AttrAxisP2, axisP2Id);
    AddAttribute(domElement, AttrFirstPoint, firstPointId);
    AddAttribute(domElement, AttrSecondPoint, secondPointId);

    AddToModeling(domElement);
}
