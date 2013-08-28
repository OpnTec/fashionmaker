#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include "vtoolnormal.h"
#include <QMenu>
#pragma GCC diagnostic pop

VToolNormal::VToolNormal(VDomDocument *doc, VContainer *data, const qint64 &id, Draw::Mode mode,
                         const QString &typeLine,
                         const QString &formula, const qreal &angle, const qint64 &firstPointId,
                         const qint64 &secondPointId, Tool::Enum typeCreation, QGraphicsItem *parent):
    VToolLinePoint(doc, data, id, mode, typeLine, formula, firstPointId, angle, parent),
    secondPointId(secondPointId), dialogNormal(QSharedPointer<DialogNormal>()){

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }

}

void VToolNormal::setDialog(){
    Q_ASSERT(!dialogNormal.isNull());
    if(!dialogNormal.isNull()){
        VPointF p = VAbstractTool::data.GetPoint(id);
        dialogNormal->setTypeLine(typeLine);
        dialogNormal->setFormula(formula);
        dialogNormal->setAngle(angle);
        dialogNormal->setFirstPointId(basePointId, id);
        dialogNormal->setSecondPointId(secondPointId, id);
        dialogNormal->setPointName(p.name());
    }
}

void VToolNormal::Create(QSharedPointer<DialogNormal> &dialog, VMainGraphicsScene *scene, VDomDocument *doc,
                         VContainer *data, Draw::Mode mode){
    QString formula = dialog->getFormula();
    qint64 firstPointId = dialog->getFirstPointId();
    qint64 secondPointId = dialog->getSecondPointId();
    QString typeLine = dialog->getTypeLine();
    QString pointName = dialog->getPointName();
    qint32 angle = dialog->getAngle();
    Create(0, formula, firstPointId, secondPointId, typeLine, pointName, angle, 5, 10, scene, doc, data,
           Document::FullParse, Tool::FromGui, mode);
}

void VToolNormal::Create(const qint64 _id, const QString &formula, const qint64 &firstPointId,
                         const qint64 &secondPointId, const QString typeLine, const QString pointName,
                         const qreal angle, const qreal &mx, const qreal &my, VMainGraphicsScene *scene,
                         VDomDocument *doc, VContainer *data, Document::Enum parse, Tool::Enum typeCreation,
                         Draw::Mode mode){
    VPointF firstPoint = data->GetPoint(firstPointId);
    VPointF secondPoint = data->GetPoint(secondPointId);
    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if(errorMsg.isEmpty()){
        QPointF fPoint = VToolNormal::FindPoint(firstPoint.toQPointF(), secondPoint.toQPointF(),
                                                result*PrintDPI/25.4, angle);
        qint64 id = _id;
        if(typeCreation == Tool::FromGui){
            id = data->AddPoint(VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
        } else {
            data->UpdatePoint(id, VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            if(parse != Document::FullParse){
                QMap<qint64, VDataTool*>* tools = doc->getTools();
                VDataTool *tool = tools->value(id);
                if(tool != 0){
                    tool->VDataTool::setData(data);
                    tools->insert(id, tool);
                    data->IncrementReferens(id, Scene::Point);
                }
            }
        }
        data->AddLine(firstPointId, id);
        VAbstractTool::AddRecord(id, Tools::NormalTool, doc);
        if(mode == Draw::Modeling){
            data->IncrementReferens(firstPointId, Scene::Point);
            data->IncrementReferens(secondPointId, Scene::Point);
        }
        if(parse == Document::FullParse){
            VToolNormal *point = new VToolNormal(doc, data, id, mode, typeLine, formula, angle,
                                                 firstPointId, secondPointId, typeCreation);
            scene->addItem(point);
            connect(point, &VToolNormal::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(point, &VToolNormal::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            tools->insert(id,point);
        }
    }
}

QPointF VToolNormal::FindPoint(const QPointF &firstPoint, const QPointF &secondPoint, const qreal &length,
                               const qreal &angle){
    QLineF line(firstPoint, secondPoint);
    QLineF normal = line.normalVector();
    normal.setAngle(normal.angle()+angle);
    normal.setLength(length);
    return normal.p2();
}

void VToolNormal::FullUpdateFromFile(){
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

void VToolNormal::FullUpdateFromGui(int result){
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

void VToolNormal::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogNormal, this, event);
}

void VToolNormal::AddToFile(){
    VPointF point;
    if(mode == Draw::Calculation){
        point = VAbstractTool::data.GetPoint(id);
    } else {
        point = VAbstractTool::data.GetModelingPoint(id);
    }
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

    AddToDraw(domElement);
}
