#pragma GCC diagnostic ignored "-Weffc++"
#include "vtoolbisector.h"
#include <QMenu>
#pragma GCC diagnostic warning "-Weffc++"

VToolBisector::VToolBisector(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &typeLine,
                             const QString &formula, const qint64 &firstPointId, const qint64 &secondPointId,
                             const qint64 &thirdPointId, Tool::Enum typeCreation, QGraphicsItem *parent):
    VToolLinePoint(doc, data, id, typeLine, formula, secondPointId, 0, parent){
    this->firstPointId = firstPointId;
    this->thirdPointId = thirdPointId;

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

QPointF VToolBisector::FindPoint(const QPointF &firstPoint, const QPointF &secondPoint,
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

void VToolBisector::setDialog(){
    Q_ASSERT(!dialogBisector.isNull());
    if(!dialogBisector.isNull()){
        VPointF p = VAbstractTool::data.GetPoint(id);
        dialogBisector->setTypeLine(typeLine);
        dialogBisector->setFormula(formula);
        dialogBisector->setFirstPointId(firstPointId, id);
        dialogBisector->setSecondPointId(basePointId, id);
        dialogBisector->setThirdPointId(thirdPointId, id);
        dialogBisector->setPointName(p.name());
    }
}

void VToolBisector::Create(QSharedPointer<DialogBisector> &dialog, VMainGraphicsScene *scene,
                           VDomDocument *doc, VContainer *data){
    QString formula = dialog->getFormula();
    qint64 firstPointId = dialog->getFirstPointId();
    qint64 secondPointId = dialog->getSecondPointId();
    qint64 thirdPointId = dialog->getThirdPointId();
    QString typeLine = dialog->getTypeLine();
    QString pointName = dialog->getPointName();
    Create(0, formula, firstPointId, secondPointId, thirdPointId, typeLine, pointName, 5, 10, scene, doc, data,
           Document::FullParse, Tool::FromGui);
}

void VToolBisector::Create(const qint64 _id, const QString &formula, const qint64 &firstPointId,
                           const qint64 &secondPointId, const qint64 &thirdPointId, const QString &typeLine,
                           const QString &pointName, const qreal &mx, const qreal &my,
                           VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data,
                           Document::Enum parse, Tool::Enum typeCreation){
    VPointF firstPoint = data->GetPoint(firstPointId);
    VPointF secondPoint = data->GetPoint(secondPointId);
    VPointF thirdPoint = data->GetPoint(thirdPointId);

    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if(errorMsg.isEmpty()){
        QPointF fPoint = VToolBisector::FindPoint(firstPoint.toQPointF(), secondPoint.toQPointF(),
                                                  thirdPoint.toQPointF(), result*PrintDPI/25.4);
        qint64 id = _id;
        if(typeCreation == Tool::FromGui){
            id = data->AddPoint(VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
        } else {
            data->UpdatePoint(id, VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            if(parse != Document::FullParse){
                QMap<qint64, VDataTool*>* tools = doc->getTools();
                VDataTool *tool = tools->value(id);
                tool->VDataTool::setData(data);
                tools->insert(id, tool);
            }
        }
        data->AddLine(firstPointId, id);
        VAbstractTool::AddRecord(id, Tools::BisectorTool, doc);
        if(parse == Document::FullParse){
            VToolBisector *point = new VToolBisector(doc, data, id, typeLine, formula,
                                                     firstPointId, secondPointId, thirdPointId,
                                                     typeCreation);
            scene->addItem(point);
            connect(point, &VToolBisector::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            tools->insert(id,point);   
        }
    }
}

void VToolBisector::FullUpdateFromFile(){
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

void VToolBisector::FullUpdateFromGui(int result){
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

void VToolBisector::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogBisector, this, event);
}

void VToolBisector::AddToFile(){
    VPointF point = VAbstractTool::data.GetPoint(id);
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

    AddToCalculation(domElement);
}
