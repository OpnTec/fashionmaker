#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "vtoolalongline.h"
#include <QDialog>
#include <QMenu>
#include <QDebug>
#pragma GCC diagnostic pop
#include "../container/calculator.h"

VToolAlongLine::VToolAlongLine(VDomDocument *doc, VContainer *data, qint64 id, Draw::Mode mode, const QString &formula,
                               const qint64 &firstPointId, const qint64 &secondPointId,
                               const QString &typeLine, Tool::Enum typeCreation,
                               QGraphicsItem *parent):
    VToolLinePoint(doc, data, id, mode, typeLine, formula, firstPointId, 0, parent), secondPointId(secondPointId),
    dialogAlongLine(QSharedPointer<DialogAlongLine>()){

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VToolAlongLine::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        typeLine = domElement.attribute("typeLine", "");
        formula = domElement.attribute("length", "");
        basePointId = domElement.attribute("firstPoint", "").toLongLong();
        secondPointId = domElement.attribute("secondPoint", "").toLongLong();
    }
    RefreshGeometry();
}

void VToolAlongLine::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", dialogAlongLine->getPointName());
            domElement.setAttribute("typeLine", dialogAlongLine->getTypeLine());
            domElement.setAttribute("length", dialogAlongLine->getFormula());
            domElement.setAttribute("firstPoint", QString().setNum(dialogAlongLine->getFirstPointId()));
            domElement.setAttribute("secondPoint", QString().setNum(dialogAlongLine->getSecondPointId()));
            emit FullUpdateTree();
        }

    }
    dialogAlongLine.clear();
}

void VToolAlongLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogAlongLine, this, event);
}

void VToolAlongLine::AddToFile(){
    VPointF point;
    if(mode == Draw::Calculation){
        point = VAbstractTool::data.GetPoint(id);
    } else {
        point = VAbstractTool::data.GetModelingPoint(id);
    }
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "alongLine");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "mx", point.mx()/PrintDPI*25.4);
    AddAttribute(domElement, "my", point.my()/PrintDPI*25.4);

    AddAttribute(domElement, "typeLine", typeLine);
    AddAttribute(domElement, "length", formula);
    AddAttribute(domElement, "firstPoint", basePointId);
    AddAttribute(domElement, "secondPoint", secondPointId);

    AddToDraw(domElement);
}

void VToolAlongLine::setDialog(){
    Q_ASSERT(!dialogAlongLine.isNull());
    if(!dialogAlongLine.isNull()){
        VPointF p = VAbstractTool::data.GetPoint(id);
        dialogAlongLine->setTypeLine(typeLine);
        dialogAlongLine->setFormula(formula);
        dialogAlongLine->setFirstPointId(basePointId, id);
        dialogAlongLine->setSecondPointId(secondPointId, id);
        dialogAlongLine->setPointName(p.name());
    }
}

void VToolAlongLine::Create(QSharedPointer<DialogAlongLine> &dialog, VMainGraphicsScene *scene,
                            VDomDocument *doc, VContainer *data, Draw::Mode mode){
    QString formula = dialog->getFormula();
    qint64 firstPointId = dialog->getFirstPointId();
    qint64 secondPointId = dialog->getSecondPointId();
    QString typeLine = dialog->getTypeLine();
    QString pointName = dialog->getPointName();
    Create(0, pointName, typeLine, formula, firstPointId, secondPointId, 5, 10, scene, doc, data,
           Document::FullParse, Tool::FromGui, mode);
}

void VToolAlongLine::Create(const qint64 _id, const QString &pointName, const QString &typeLine,
                            const QString &formula, const qint64 &firstPointId, const qint64 &secondPointId,
                            const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VDomDocument *doc,
                            VContainer *data, Document::Enum parse, Tool::Enum typeCreation, Draw::Mode mode){
    VPointF firstPoint = data->GetPoint(firstPointId);
    VPointF secondPoint = data->GetPoint(secondPointId);
    QLineF line = QLineF(firstPoint.toQPointF(), secondPoint.toQPointF());
    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if(errorMsg.isEmpty()){
        line.setLength(result*PrintDPI/25.4);
        qint64 id = _id;
        if(typeCreation == Tool::FromGui){
            id = data->AddPoint(VPointF(line.p2().x(), line.p2().y(), pointName, mx, my));
        } else {
            data->UpdatePoint(id, VPointF(line.p2().x(), line.p2().y(), pointName, mx, my));
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
        VAbstractTool::AddRecord(id, Tools::AlongLineTool, doc);
        data->AddLine(firstPointId, id);
        data->AddLine(id, secondPointId);
        if(mode == Draw::Modeling){
            data->IncrementReferens(firstPointId, Scene::Point);
            data->IncrementReferens(secondPointId, Scene::Point);
        }
        if(parse == Document::FullParse){
            VToolAlongLine *point = new VToolAlongLine(doc, data, id, mode, formula, firstPointId,
                                                       secondPointId, typeLine, typeCreation);
            scene->addItem(point);
            connect(point, &VToolAlongLine::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(point, &VToolAlongLine::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            tools->insert(id,point);

        }
    }
}
