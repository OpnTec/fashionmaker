#include "vmodelingnormal.h"
#include <QMenu>

VModelingNormal::VModelingNormal(VDomDocument *doc, VContainer *data, const qint64 &id,
                         const QString &typeLine,
                         const QString &formula, const qreal &angle, const qint64 &firstPointId,
                         const qint64 &secondPointId, Tool::Enum typeCreation, QGraphicsItem *parent):
    VModelingLinePoint(doc, data, id, typeLine, formula, firstPointId, angle, parent),
    secondPointId(secondPointId), dialogNormal(QSharedPointer<DialogNormal>()){

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }

}

void VModelingNormal::setDialog(){
    Q_ASSERT(!dialogNormal.isNull());
    if(!dialogNormal.isNull()){
        VPointF p = VAbstractTool::data.GetModelingPoint(id);
        dialogNormal->setTypeLine(typeLine);
        dialogNormal->setFormula(formula);
        dialogNormal->setAngle(angle);
        dialogNormal->setFirstPointId(basePointId, id);
        dialogNormal->setSecondPointId(secondPointId, id);
        dialogNormal->setPointName(p.name());
    }
}

VModelingNormal* VModelingNormal::Create(QSharedPointer<DialogNormal> &dialog, VDomDocument *doc,
                                         VContainer *data){
    QString formula = dialog->getFormula();
    qint64 firstPointId = dialog->getFirstPointId();
    qint64 secondPointId = dialog->getSecondPointId();
    QString typeLine = dialog->getTypeLine();
    QString pointName = dialog->getPointName();
    qreal angle = dialog->getAngle();
    return Create(0, formula, firstPointId, secondPointId, typeLine, pointName, angle, 5, 10, doc, data,
                  Document::FullParse, Tool::FromGui);
}

VModelingNormal *VModelingNormal::Create(const qint64 _id, const QString &formula,
                                         const qint64 &firstPointId, const qint64 &secondPointId,
                                         const QString typeLine, const QString pointName,
                                         const qreal angle, const qreal &mx, const qreal &my,
                                         VDomDocument *doc, VContainer *data, Document::Enum parse,
                                         Tool::Enum typeCreation){
    VModelingNormal *point = 0;
    VPointF firstPoint = data->GetModelingPoint(firstPointId);
    VPointF secondPoint = data->GetModelingPoint(secondPointId);
    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if(errorMsg.isEmpty()){
        QPointF fPoint = VModelingNormal::FindPoint(firstPoint.toQPointF(), secondPoint.toQPointF(),
                                                result*PrintDPI/25.4, angle);
        qint64 id = _id;
        if(typeCreation == Tool::FromGui){
            id = data->AddModelingPoint(VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
        } else {
            data->UpdateModelingPoint(id, VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            if(parse != Document::FullParse){
                QMap<qint64, VDataTool*>* tools = doc->getTools();
                VDataTool *tool = tools->value(id);
                if(tool != 0){
                    tool->VDataTool::setData(data);
                    data->IncrementReferens(id, Scene::Point, Draw::Modeling);
                }
            }
        }
        data->AddLine(firstPointId, id, Draw::Modeling);
        data->IncrementReferens(firstPointId, Scene::Point, Draw::Modeling);
        data->IncrementReferens(secondPointId, Scene::Point, Draw::Modeling);
        if(parse == Document::FullParse){
            point = new VModelingNormal(doc, data, id, typeLine, formula, angle, firstPointId, secondPointId,
                                        typeCreation);
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            tools->insert(id,point);
        }
    }
    return point;
}

QPointF VModelingNormal::FindPoint(const QPointF &firstPoint, const QPointF &secondPoint, const qreal &length,
                               const qreal &angle){
    QLineF line(firstPoint, secondPoint);
    QLineF normal = line.normalVector();
    normal.setAngle(normal.angle()+angle);
    normal.setLength(length);
    return normal.p2();
}

void VModelingNormal::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        typeLine = domElement.attribute("typeLine", "");
        formula = domElement.attribute("length", "");
        basePointId = domElement.attribute("firstPoint", "").toLongLong();
        secondPointId = domElement.attribute("secondPoint", "").toLongLong();
        angle = domElement.attribute("angle", "").toInt();
    }
    RefreshGeometry();
}

void VModelingNormal::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", dialogNormal->getPointName());
            domElement.setAttribute("typeLine", dialogNormal->getTypeLine());
            domElement.setAttribute("length", dialogNormal->getFormula());
            domElement.setAttribute("angle", QString().setNum(dialogNormal->getAngle()));
            domElement.setAttribute("firstPoint", QString().setNum(dialogNormal->getFirstPointId()));
            domElement.setAttribute("secondPoint", QString().setNum(dialogNormal->getSecondPointId()));
            emit FullUpdateTree();
        }
    }
    dialogNormal.clear();
}

void VModelingNormal::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogNormal, this, event);
}

void VModelingNormal::AddToFile(){
    VPointF point = VAbstractTool::data.GetModelingPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "normal");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "mx", point.mx()/PrintDPI*25.4);
    AddAttribute(domElement, "my", point.my()/PrintDPI*25.4);

    AddAttribute(domElement, "typeLine", typeLine);
    AddAttribute(domElement, "length", formula);
    AddAttribute(domElement, "angle", angle);
    AddAttribute(domElement, "firstPoint", basePointId);
    AddAttribute(domElement, "secondPoint", secondPointId);

    AddToModeling(domElement);
}
