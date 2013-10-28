#include "vtooltriangle.h"

VToolTriangle::VToolTriangle(VDomDocument *doc, VContainer *data, const qint64 &id,
                             const qint64 &axisP1Id, const qint64 &axisP2Id, const qint64 &firstPointId,
                             const qint64 &secondPointId, Tool::Sources typeCreation, QGraphicsItem *parent)
    :VToolPoint(doc, data, id, parent), axisP1Id(axisP1Id), axisP2Id(axisP2Id), firstPointId(firstPointId),
      secondPointId(secondPointId), dialogTriangle(QSharedPointer<DialogTriangle>()) {
    ignoreFullUpdate = true;
    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VToolTriangle::setDialog(){
    Q_ASSERT(!dialogTriangle.isNull());
    VPointF p = VAbstractTool::data.GetPoint(id);
    dialogTriangle->setAxisP1Id(axisP1Id, id);
    dialogTriangle->setAxisP2Id(axisP2Id, id);
    dialogTriangle->setFirstPointId(firstPointId, id);
    dialogTriangle->setSecondPointId(secondPointId, id);
    dialogTriangle->setPointName(p.name());
}

void VToolTriangle::Create(QSharedPointer<DialogTriangle> &dialog, VMainGraphicsScene *scene,
                           VDomDocument *doc, VContainer *data){
    qint64 axisP1Id = dialog->getAxisP1Id();
    qint64 axisP2Id = dialog->getAxisP2Id();
    qint64 firstPointId = dialog->getFirstPointId();
    qint64 secondPointId = dialog->getSecondPointId();
    QString pointName = dialog->getPointName();
    Create(0, pointName, axisP1Id, axisP2Id, firstPointId, secondPointId, 5, 10, scene, doc, data,
           Document::FullParse, Tool::FromGui);
}

void VToolTriangle::Create(const qint64 _id, const QString &pointName, const qint64 &axisP1Id,
                           const qint64 &axisP2Id, const qint64 &firstPointId, const qint64 &secondPointId,
                           const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VDomDocument *doc,
                           VContainer *data, const Document::Documents &parse, Tool::Sources typeCreation){
    VPointF axisP1 = data->GetPoint(axisP1Id);
    VPointF axisP2 = data->GetPoint(axisP2Id);
    VPointF firstPoint = data->GetPoint(firstPointId);
    VPointF secondPoint = data->GetPoint(secondPointId);

    QPointF point = FindPoint(axisP1.toQPointF(), axisP2.toQPointF(), firstPoint.toQPointF(),
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
    VDrawTool::AddRecord(id, Tool::Triangle, doc);
    if(parse == Document::FullParse){
        VToolTriangle *point = new VToolTriangle(doc, data, id, axisP1Id, axisP2Id, firstPointId,
                                                 secondPointId, typeCreation);
        scene->addItem(point);
        connect(point, &VToolTriangle::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(point, &VToolTriangle::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolTriangle::SetFactor);
        doc->AddTool(id, point);
        doc->IncrementReferens(axisP1Id);
        doc->IncrementReferens(axisP2Id);
        doc->IncrementReferens(firstPointId);
        doc->IncrementReferens(secondPointId);
    }
}

QPointF VToolTriangle::FindPoint(const QPointF axisP1, const QPointF axisP2, const QPointF firstPoint,
                                 const QPointF secondPoint){
    qreal c = QLineF(firstPoint, secondPoint).length();
    qreal a = QLineF(axisP2, firstPoint).length();
    qreal b = QLineF(axisP2, secondPoint).length();
    if(c*c == a*a + b*b){
        QLineF l1(axisP2, firstPoint);
        QLineF l2(axisP2, secondPoint);
        if(l1.angleTo(l2) == 90 || l2.angleTo(l1) == 90){
            return axisP2;
        }
    }

    QLineF line = QLineF(axisP1, axisP2);
    qreal step = 0.01;
    while(1){
        line.setLength(line.length()+step);
        a = QLineF(line.p2(), firstPoint).length();
        b = QLineF(line.p2(), secondPoint).length();
        if(static_cast<qint32>(c*c) == static_cast<qint32>(a*a + b*b)){
            QLineF l1(axisP2, firstPoint);
            QLineF l2(axisP2, secondPoint);
            if(l1.angleTo(l2) == 90 || l2.angleTo(l1) == 90){
                return line.p2();
            }
        }
        if(c*c < a*a + b*b){
            qWarning()<<tr("Can't find point.")<<Q_FUNC_INFO;
            return line.p2();
        }
    }
}

void VToolTriangle::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        axisP1Id = domElement.attribute("axisP1", "").toLongLong();
        axisP2Id = domElement.attribute("axisP2", "").toLongLong();
        firstPointId = domElement.attribute("firstPoint", "").toLongLong();
        secondPointId = domElement.attribute("secondPoint", "").toLongLong();
    }
    VToolPoint::RefreshPointGeometry(VDrawTool::data.GetPoint(id));
}

void VToolTriangle::FullUpdateFromGui(int result){
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

void VToolTriangle::RemoveReferens(){
    doc->DecrementReferens(axisP1Id);
    doc->DecrementReferens(axisP2Id);
    doc->DecrementReferens(firstPointId);
    doc->DecrementReferens(secondPointId);
}

void VToolTriangle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogTriangle, this, event);
}

void VToolTriangle::AddToFile(){
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

    AddToCalculation(domElement);
}
