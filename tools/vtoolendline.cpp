#include "vtoolendline.h"
#include <QDebug>
#include <QMenu>

#include "../widgets/vmaingraphicsscene.h"

VToolEndLine::VToolEndLine(VDomDocument *doc, VContainer *data, const qint64 &id,
                           const QString &typeLine, const QString &formula, const qint32 &angle,
                           const qint64 &basePointId, Tool::Enum typeCreation,
                           QGraphicsItem *parent):VToolPoint(doc, data, id, parent){
    ignoreContextMenuEvent = false;//don't ignore context menu events;
    connect(this, &VToolEndLine::FullUpdateTree, this->doc, &VDomDocument::FullUpdateTree);
    this->typeLine = typeLine;
    this->formula = formula;
    this->angle = angle;
    this->basePointId = basePointId;

    //Лінія, що з'єднує дві точки
    VPointF basePoint = data->GetPoint(basePointId);
    VPointF point = data->GetPoint(id);
    mainLine = new QGraphicsLineItem(QLineF(basePoint.toQPointF(), point.toQPointF()), this);
    mainLine->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    if(typeLine == "none"){
        mainLine->setVisible(false);
    } else {
        mainLine->setVisible(true);
    }

    QString nameLine = GetNameLine(basePointId, id);
    QLineF line = QLineF(basePoint.toQPointF(), point.toQPointF());
    data->AddLine(nameLine, line.length());

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VToolEndLine::FullUpdateFromFile(){
    QString name;
    qreal mx, my;
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        name = domElement.attribute("name", "");
        mx = domElement.attribute("mx", "").toDouble()*PrintDPI/25.4;
        my = domElement.attribute("my", "").toDouble()*PrintDPI/25.4;
        typeLine = domElement.attribute("typeLine", "");
        formula = domElement.attribute("length", "");
        basePointId = domElement.attribute("basePoint", "").toLongLong();
        angle = domElement.attribute("angle", "").toInt();
    }
    VPointF point = data->GetPoint(id);
    RefreshBaseGeometry(name, point.x(), point.y(), mx, my);
    VPointF basePoint = data->GetPoint(basePointId);
    mainLine->setLine(QLineF(basePoint.toQPointF(), point.toQPointF()));

    QString nameLine = GetNameLine(basePointId, id);
    QLineF line = QLineF(basePoint.toQPointF(), point.toQPointF());
    data->AddLine(nameLine, line.length());
}

void VToolEndLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    if(!ignoreContextMenuEvent){
        QMenu menu;
        QAction *actionOption = menu.addAction("Властивості");
        QAction *selectedAction = menu.exec(event->screenPos());
        if(selectedAction == actionOption){
            dialogEndLine = QSharedPointer<DialogEndLine>(new DialogEndLine(data));

            connect(qobject_cast< VMainGraphicsScene * >(this->scene()), &VMainGraphicsScene::ChoosedObject,
                    dialogEndLine.data(), &DialogEndLine::ChoosedPoint);
            connect(dialogEndLine.data(), &DialogEndLine::DialogClosed, this, &VToolEndLine::ClosedDialogEndLine);
            connect(doc, &VDomDocument::FullUpdateFromFile, dialogEndLine.data(), &DialogEndLine::UpdateList);

            VPointF p = data->GetPoint(id);

            dialogEndLine->setTypeLine(typeLine);
            dialogEndLine->setFormula(formula);
            dialogEndLine->setAngle(angle);
            dialogEndLine->setBasePointId(basePointId);
            dialogEndLine->setPointName(p.name());

            dialogEndLine->show();
        }
    }
}

void VToolEndLine::ClosedDialogEndLine(int result){
    if(result == QDialog::Accepted){
        QString pointName = dialogEndLine->getPointName();
        QString typeLine = dialogEndLine->getTypeLine();
        QString formula = dialogEndLine->getFormula();
        qint32 angle = dialogEndLine->getAngle();
        qint64 basePointId = dialogEndLine->getBasePointId();

        VPointF basePoint = data->GetPoint(basePointId);
        QLineF line = QLineF(basePoint.toQPointF(), QPointF(basePoint.x()+100, basePoint.y()));
        Calculator cal(data);
        QString errorMsg;
        qreal result = cal.eval(formula, &errorMsg);
        if(errorMsg.isEmpty()){
            line.setLength(result/PrintDPI*25.4);
            line.setAngle(angle);

            FullUpdateFromGui(pointName, typeLine, formula, angle, basePointId);
        }

    }
    dialogEndLine.clear();
}

void VToolEndLine::AddToFile(){
    VPointF point = data->GetPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "endLine");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "mx", point.mx()/PrintDPI*25.4);
    AddAttribute(domElement, "my", point.my()/PrintDPI*25.4);

    AddAttribute(domElement, "typeLine", typeLine);
    AddAttribute(domElement, "length", formula);
    AddAttribute(domElement, "angle", angle);
    AddAttribute(domElement, "basePoint", basePointId);

    QDomElement calcElement;
    bool ok = doc->GetActivCalculationElement(calcElement);
    if(ok){
        calcElement.appendChild(domElement);
    } else {
        qCritical()<<"Не можу знайти тег калькуляції."<< Q_FUNC_INFO;
    }
}

void VToolEndLine::FullUpdateFromGui(const QString &name, const QString &typeLine, const QString &formula,
                                     const qreal &angle, const qint64 &basePointId){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        this->typeLine = typeLine;
        this->formula = formula;
        this->angle = angle;
        this->basePointId = basePointId;

        domElement.setAttribute("name", name);

        domElement.setAttribute("typeLine", typeLine);
        domElement.setAttribute("length", formula);
        domElement.setAttribute("angle", QString().setNum(angle));
        domElement.setAttribute("basePoint", QString().setNum(basePointId));
        emit FullUpdateTree();
    }
}
