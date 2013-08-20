#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include "vtoolshoulderpoint.h"
#include <QDebug>
#include <QMenu>
#pragma GCC diagnostic pop

VToolShoulderPoint::VToolShoulderPoint(VDomDocument *doc, VContainer *data, const qint64 &id,
                                       const QString &typeLine, const QString &formula, const qint64 &p1Line,
                                       const qint64 &p2Line, const qint64 &pShoulder, Tool::Enum typeCreation,
                                       QGraphicsItem * parent):
    VToolLinePoint(doc, data, id, typeLine, formula, p1Line, 0, parent), p2Line(p2Line), pShoulder(pShoulder),
    dialogShoulderPoint(QSharedPointer<DialogShoulderPoint>()){

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }

}

void VToolShoulderPoint::setDialog(){
    Q_ASSERT(!dialogShoulderPoint.isNull());
    if(!dialogShoulderPoint.isNull()){
        VPointF p = VAbstractTool::data.GetPoint(id);
        dialogShoulderPoint->setTypeLine(typeLine);
        dialogShoulderPoint->setFormula(formula);
        dialogShoulderPoint->setP1Line(basePointId, id);
        dialogShoulderPoint->setP2Line(p2Line, id);
        dialogShoulderPoint->setPShoulder(pShoulder, id);
        dialogShoulderPoint->setPointName(p.name());
    }
}

QPointF VToolShoulderPoint::FindPoint(const QPointF &p1Line, const QPointF &p2Line, const QPointF &pShoulder,
                                      const qreal &length){
    QLineF line = QLineF(p1Line, p2Line);
        qreal dist = line.length();
        if(dist>length){
            qDebug()<<"A3П2="<<length/PrintDPI*25.4<<"А30П ="<<dist/PrintDPI*25.4;
            throw"Не можу знайти точку плеча. Довжина А3П2 < А3П.";
        }
        if(dist==length){
            return line.p2();
        }
        qreal step = 0.01;
        while(1){
            line.setLength(line.length()+step);
            QLineF line2 = QLineF(pShoulder, line.p2());
            if(line2.length()>=length){
                return line.p2();
            }
        }
}

void VToolShoulderPoint::Create(QSharedPointer<DialogShoulderPoint> &dialog, VMainGraphicsScene *scene,
                                VDomDocument *doc, VContainer *data){
    QString formula = dialog->getFormula();
    qint64 p1Line = dialog->getP1Line();
    qint64 p2Line = dialog->getP2Line();
    qint64 pShoulder = dialog->getPShoulder();
    QString typeLine = dialog->getTypeLine();
    QString pointName = dialog->getPointName();
    Create(0, formula, p1Line, p2Line, pShoulder, typeLine, pointName, 5, 10, scene, doc, data,
           Document::FullParse, Tool::FromGui);
}

void VToolShoulderPoint::Create(const qint64 _id, const QString &formula, const qint64 &p1Line,
                                const qint64 &p2Line, const qint64 &pShoulder, const QString &typeLine,
                                const QString &pointName, const qreal &mx, const qreal &my,
                                VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data,
                                Document::Enum parse, Tool::Enum typeCreation){
    VPointF firstPoint = data->GetPoint(p1Line);
    VPointF secondPoint = data->GetPoint(p2Line);
    VPointF shoulderPoint = data->GetPoint(pShoulder);

    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if(errorMsg.isEmpty()){
        QPointF fPoint = VToolShoulderPoint::FindPoint(firstPoint.toQPointF(), secondPoint.toQPointF(),
                                                       shoulderPoint.toQPointF(), result*PrintDPI/25.4);
        qint64 id =  _id;
        if(typeCreation == Tool::FromGui){
            id = data->AddPoint(VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
        } else {
            data->UpdatePoint(id,VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            if(parse != Document::FullParse){
                QMap<qint64, VDataTool*>* tools = doc->getTools();
                VDataTool *tool = tools->value(id);
                tool->VDataTool::setData(data);
                tools->insert(id, tool);
            }
        }
        data->AddLine(p1Line, id);
        data->AddLine(p2Line, id);
        VAbstractTool::AddRecord(id, Tools::ShoulderPointTool, doc);
        if(parse == Document::FullParse){
            VToolShoulderPoint *point = new VToolShoulderPoint(doc, data, id, typeLine, formula,
                                                               p1Line, p2Line, pShoulder,
                                                               typeCreation);
            scene->addItem(point);
            connect(point, &VToolShoulderPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(point, &VToolShoulderPoint::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            tools->insert(id,point);
        }
    }
}

void VToolShoulderPoint::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        typeLine = domElement.attribute("typeLine", "");
        formula = domElement.attribute("length", "");
        basePointId = domElement.attribute("p1Line", "").toLongLong();
        p2Line = domElement.attribute("p2Line", "").toLongLong();
        pShoulder = domElement.attribute("pShoulder", "").toLongLong();
    }
    RefreshGeometry();
}

void VToolShoulderPoint::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", dialogShoulderPoint->getPointName());
            domElement.setAttribute("typeLine", dialogShoulderPoint->getTypeLine());
            domElement.setAttribute("length", dialogShoulderPoint->getFormula());
            domElement.setAttribute("p1Line", QString().setNum(dialogShoulderPoint->getP1Line()));
            domElement.setAttribute("p2Line", QString().setNum(dialogShoulderPoint->getP2Line()));
            domElement.setAttribute("pShoulder", QString().setNum(dialogShoulderPoint->getPShoulder()));
            emit FullUpdateTree();
        }
    }
    dialogShoulderPoint.clear();
}

void VToolShoulderPoint::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogShoulderPoint, this, event);
}

void VToolShoulderPoint::AddToFile(){
    VPointF point = VAbstractTool::data.GetPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "shoulder");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "mx", point.mx()/PrintDPI*25.4);
    AddAttribute(domElement, "my", point.my()/PrintDPI*25.4);

    AddAttribute(domElement, "typeLine", typeLine);
    AddAttribute(domElement, "length", formula);
    AddAttribute(domElement, "p1Line", basePointId);
    AddAttribute(domElement, "p2Line", p2Line);
    AddAttribute(domElement, "pShoulder", pShoulder);

    AddToCalculation(domElement);
}
