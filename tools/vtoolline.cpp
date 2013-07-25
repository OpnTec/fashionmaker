#include "vtoolline.h"
#include <QMenu>
#include <QDebug>

VToolLine::VToolLine(VDomDocument *doc, VContainer *data, qint64 id, qint64 firstPoint, qint64 secondPoint,
                     Tool::Enum typeCreation):VAbstractTool(doc, data, id){
    connect(this, &VToolLine::FullUpdateTree, this->doc, &VDomDocument::FullUpdateTree);
    this->firstPoint = firstPoint;
    this->secondPoint = secondPoint;

    //Лінія
    VPointF first = data->GetPoint(firstPoint);
    VPointF second = data->GetPoint(secondPoint);
    this->setLine(QLineF(first.toQPointF(), second.toQPointF()));
    this->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);

    QString nameLine = GetNameLine(firstPoint, secondPoint);
    data->AddLine(nameLine, QLineF(first.toQPointF(), second.toQPointF()).length());

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VToolLine::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        firstPoint = domElement.attribute("firstPoint", "").toLongLong();
        secondPoint = domElement.attribute("secondPoint", "").toLongLong();
    }
    VPointF first = VAbstractTool::data->GetPoint(firstPoint);
    VPointF second = VAbstractTool::data->GetPoint(secondPoint);
    this->setLine(QLineF(first.toQPointF(), second.toQPointF()));
    QString nameLine = GetNameLine(firstPoint, secondPoint);
    VAbstractTool::data->AddLine(nameLine, QLineF(first.toQPointF(), second.toQPointF()).length());
}

void VToolLine::ClosedDialogLine(int result){
    if(result == QDialog::Accepted){
        qint64 firstPoint = dialogLine->getFirstPoint();
        qint64 secondPoint = dialogLine->getSecondPoint();
        FullUpdateFromGui(firstPoint, secondPoint);
    }
    dialogLine.clear();
}

void VToolLine::ChangedActivDraw(const QString newName){
    if(nameActivDraw == newName){
        this->setPen(QPen(Qt::black, widthHairLine));
        VAbstractTool::ChangedActivDraw(newName);
    } else {
        this->setPen(QPen(Qt::gray, widthHairLine));
        VAbstractTool::ChangedActivDraw(newName);
    }
}

void VToolLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    if(!ignoreContextMenuEvent){
        QMenu menu;
        QAction *actionOption = menu.addAction("Властивості");
        QAction *selectedAction = menu.exec(event->screenPos());
        if(selectedAction == actionOption){
            dialogLine = QSharedPointer<DialogLine>(new DialogLine(VAbstractTool::data));

            connect(qobject_cast< VMainGraphicsScene * >(this->scene()), &VMainGraphicsScene::ChoosedObject,
                    dialogLine.data(), &DialogLine::ChoosedPoint);
            connect(dialogLine.data(), &DialogLine::DialogClosed, this, &VToolLine::ClosedDialogLine);

            dialogLine->setFirstPoint(firstPoint);
            dialogLine->setSecondPoint(secondPoint);

            dialogLine->show();
        }
    }
}


void VToolLine::AddToFile(){
    QDomElement domElement = doc->createElement("line");

    AddAttribute(domElement, "id", id);

    AddAttribute(domElement, "firstPoint", firstPoint);
    AddAttribute(domElement, "secondPoint", secondPoint);

    QDomElement calcElement;
    bool ok = doc->GetActivCalculationElement(calcElement);
    if(ok){
        calcElement.appendChild(domElement);
    } else {
        qCritical()<<"Не можу знайти тег калькуляції."<< Q_FUNC_INFO;
    }
}

void VToolLine::FullUpdateFromGui(qint64 firstPoint, qint64 secondPoint){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        this->firstPoint = firstPoint;
        this->secondPoint = secondPoint;

        domElement.setAttribute("firstPoint", QString().setNum(firstPoint));
        domElement.setAttribute("secondPoint", QString().setNum(secondPoint));
        emit FullUpdateTree();
    }
}
