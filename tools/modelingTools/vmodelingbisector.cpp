#include "vmodelingbisector.h"
#include <QMenu>

VModelingBisector::VModelingBisector(VDomDocument *doc, VContainer *data, const qint64 &id,
                             const QString &typeLine, const QString &formula, const qint64 &firstPointId,
                             const qint64 &secondPointId, const qint64 &thirdPointId, Tool::Enum typeCreation,
                             QGraphicsItem *parent):
    VModelingLinePoint(doc, data, id, typeLine, formula, secondPointId, 0, parent), firstPointId(0),
    thirdPointId(0), dialogBisector(QSharedPointer<DialogBisector>()){
    this->firstPointId = firstPointId;
    this->thirdPointId = thirdPointId;

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

QPointF VModelingBisector::FindPoint(const QPointF &firstPoint, const QPointF &secondPoint,
                                 const QPointF &thirdPoint, const qreal &length){
    QLineF line1(secondPoint, firstPoint);
    QLineF line2(secondPoint, thirdPoint);
    qreal angle = line1.angleTo(line2);
    if(angle>180){
        angle = 360 - angle;
    }
    line1.setAngle(line1.angle()-angle/2);
    line1.setLength(length);
    return line1.p2();
}

void VModelingBisector::setDialog(){
    Q_ASSERT(!dialogBisector.isNull());
    if(!dialogBisector.isNull()){
        VPointF p = VAbstractTool::data.GetModelingPoint(id);
        dialogBisector->setTypeLine(typeLine);
        dialogBisector->setFormula(formula);
        dialogBisector->setFirstPointId(firstPointId, id);
        dialogBisector->setSecondPointId(basePointId, id);
        dialogBisector->setThirdPointId(thirdPointId, id);
        dialogBisector->setPointName(p.name());
    }
}

VModelingBisector *VModelingBisector::Create(QSharedPointer<DialogBisector> &dialog, VDomDocument *doc,
                                             VContainer *data){
    QString formula = dialog->getFormula();
    qint64 firstPointId = dialog->getFirstPointId();
    qint64 secondPointId = dialog->getSecondPointId();
    qint64 thirdPointId = dialog->getThirdPointId();
    QString typeLine = dialog->getTypeLine();
    QString pointName = dialog->getPointName();
    return Create(0, formula, firstPointId, secondPointId, thirdPointId, typeLine, pointName, 5, 10, doc,
                  data, Document::FullParse, Tool::FromGui);
}

VModelingBisector *VModelingBisector::Create(const qint64 _id, const QString &formula,
                                             const qint64 &firstPointId, const qint64 &secondPointId,
                                             const qint64 &thirdPointId, const QString &typeLine,
                                             const QString &pointName, const qreal &mx, const qreal &my,
                                             VDomDocument *doc, VContainer *data, Document::Enum parse,
                                             Tool::Enum typeCreation){
    VModelingBisector *point = 0;
    VPointF firstPoint = data->GetModelingPoint(firstPointId);
    VPointF secondPoint = data->GetModelingPoint(secondPointId);
    VPointF thirdPoint = data->GetModelingPoint(thirdPointId);

    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if(errorMsg.isEmpty()){
        QPointF fPoint = VModelingBisector::FindPoint(firstPoint.toQPointF(), secondPoint.toQPointF(),
                                                  thirdPoint.toQPointF(), result*PrintDPI/25.4);
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
        data->IncrementReferens(thirdPointId, Scene::Point, Draw::Modeling);
        if(parse == Document::FullParse){
            point = new VModelingBisector(doc, data, id, typeLine, formula, firstPointId, secondPointId,
                                          thirdPointId, typeCreation);
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            tools->insert(id,point);
        }
    }
    return point;
}

void VModelingBisector::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        typeLine = domElement.attribute("typeLine", "");
        formula = domElement.attribute("length", "");
        firstPointId = domElement.attribute("firstPoint", "").toLongLong();
        basePointId = domElement.attribute("secondPoint", "").toLongLong();
        thirdPointId = domElement.attribute("thirdPoint", "").toLongLong();
    }
    RefreshGeometry();
}

void VModelingBisector::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", dialogBisector->getPointName());
            domElement.setAttribute("typeLine", dialogBisector->getTypeLine());
            domElement.setAttribute("length", dialogBisector->getFormula());
            domElement.setAttribute("firstPoint", QString().setNum(dialogBisector->getFirstPointId()));
            domElement.setAttribute("secondPoint", QString().setNum(dialogBisector->getSecondPointId()));
            domElement.setAttribute("thirdPoint", QString().setNum(dialogBisector->getThirdPointId()));
            emit FullUpdateTree();
        }
    }
    dialogBisector.clear();
}

void VModelingBisector::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogBisector, this, event);
}

void VModelingBisector::AddToFile(){
    VPointF point = VAbstractTool::data.GetModelingPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "bisector");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "mx", point.mx()/PrintDPI*25.4);
    AddAttribute(domElement, "my", point.my()/PrintDPI*25.4);

    AddAttribute(domElement, "typeLine", typeLine);
    AddAttribute(domElement, "length", formula);
    AddAttribute(domElement, "firstPoint", firstPointId);
    AddAttribute(domElement, "secondPoint", basePointId);
    AddAttribute(domElement, "thirdPoint", thirdPointId);

    AddToModeling(domElement);
}
