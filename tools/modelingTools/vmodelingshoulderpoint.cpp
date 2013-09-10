#include "vmodelingshoulderpoint.h"
#include <QDebug>
#include <QMenu>

VModelingShoulderPoint::VModelingShoulderPoint(VDomDocument *doc, VContainer *data, const qint64 &id,
                                       const QString &typeLine, const QString &formula, const qint64 &p1Line,
                                       const qint64 &p2Line, const qint64 &pShoulder, Tool::Enum typeCreation,
                                       QGraphicsItem * parent):
    VModelingLinePoint(doc, data, id, typeLine, formula, p1Line, 0, parent), p2Line(p2Line),
    pShoulder(pShoulder), dialogShoulderPoint(QSharedPointer<DialogShoulderPoint>()){

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }

}

void VModelingShoulderPoint::setDialog(){
    Q_ASSERT(!dialogShoulderPoint.isNull());
    if(!dialogShoulderPoint.isNull()){
        VPointF p = VAbstractTool::data.GetModelingPoint(id);
        dialogShoulderPoint->setTypeLine(typeLine);
        dialogShoulderPoint->setFormula(formula);
        dialogShoulderPoint->setP1Line(basePointId, id);
        dialogShoulderPoint->setP2Line(p2Line, id);
        dialogShoulderPoint->setPShoulder(pShoulder, id);
        dialogShoulderPoint->setPointName(p.name());
    }
}

QPointF VModelingShoulderPoint::FindPoint(const QPointF &p1Line, const QPointF &p2Line, const QPointF &pShoulder,
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

VModelingShoulderPoint *VModelingShoulderPoint::Create(QSharedPointer<DialogShoulderPoint> &dialog,
                                                       VDomDocument *doc, VContainer *data){
    QString formula = dialog->getFormula();
    qint64 p1Line = dialog->getP1Line();
    qint64 p2Line = dialog->getP2Line();
    qint64 pShoulder = dialog->getPShoulder();
    QString typeLine = dialog->getTypeLine();
    QString pointName = dialog->getPointName();
    return Create(0, formula, p1Line, p2Line, pShoulder, typeLine, pointName, 5, 10, doc, data,
                  Document::FullParse, Tool::FromGui);
}

VModelingShoulderPoint *VModelingShoulderPoint::Create(const qint64 _id, const QString &formula,
                                                        const qint64 &p1Line, const qint64 &p2Line,
                                                        const qint64 &pShoulder, const QString &typeLine,
                                                        const QString &pointName, const qreal &mx,
                                                        const qreal &my, VDomDocument *doc, VContainer *data,
                                                        Document::Enum parse, Tool::Enum typeCreation){
    VModelingShoulderPoint *point = 0;
    VPointF firstPoint = data->GetModelingPoint(p1Line);
    VPointF secondPoint = data->GetModelingPoint(p2Line);
    VPointF shoulderPoint = data->GetModelingPoint(pShoulder);

    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if(errorMsg.isEmpty()){
        QPointF fPoint = VModelingShoulderPoint::FindPoint(firstPoint.toQPointF(), secondPoint.toQPointF(),
                                                       shoulderPoint.toQPointF(), result*PrintDPI/25.4);
        qint64 id =  _id;
        if(typeCreation == Tool::FromGui){
            id = data->AddModelingPoint(VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
        } else {
            data->UpdateModelingPoint(id,VPointF(fPoint.x(), fPoint.y(), pointName, mx, my));
            if(parse != Document::FullParse){
                QMap<qint64, VDataTool*>* tools = doc->getTools();
                VDataTool *tool = tools->value(id);
                if(tool != 0){
                    tool->VDataTool::setData(data);
                    data->IncrementReferens(id, Scene::Point, Draw::Modeling);
                }
            }
        }
        data->AddLine(p1Line, id, Draw::Modeling);
        data->AddLine(p2Line, id, Draw::Modeling);
        data->IncrementReferens(p1Line, Scene::Point, Draw::Modeling);
        data->IncrementReferens(p2Line, Scene::Point, Draw::Modeling);
        data->IncrementReferens(pShoulder, Scene::Point, Draw::Modeling);
        if(parse == Document::FullParse){
            point = new VModelingShoulderPoint(doc, data, id, typeLine, formula, p1Line, p2Line, pShoulder,
                                               typeCreation);
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            tools->insert(id,point);
        }
    }
    return point;
}

void VModelingShoulderPoint::FullUpdateFromFile(){
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

void VModelingShoulderPoint::FullUpdateFromGui(int result){
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

void VModelingShoulderPoint::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogShoulderPoint, this, event);
}

void VModelingShoulderPoint::AddToFile(){
    VPointF point = VAbstractTool::data.GetModelingPoint(id);
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

    AddToModeling(domElement);
}
