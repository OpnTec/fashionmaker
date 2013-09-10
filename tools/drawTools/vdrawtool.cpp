#include "vdrawtool.h"

VDrawTool::VDrawTool(VDomDocument *doc, VContainer *data, qint64 id, QObject *parent) :
    VAbstractTool(doc, data, id, parent), ignoreContextMenuEvent(false),
    nameActivDraw(doc->GetNameActivDraw()){
    connect(this->doc, &VDomDocument::ChangedActivDraw, this, &VDrawTool::ChangedActivDraw);
    connect(this->doc, &VDomDocument::ChangedNameDraw, this, &VDrawTool::ChangedNameDraw);
    connect(this->doc, &VDomDocument::ShowTool, this, &VDrawTool::ShowTool);
}

VDrawTool::~VDrawTool(){
}

void VDrawTool::setDialog(){
}

void VDrawTool::AddRecord(const qint64 id, Tools::Enum toolType, VDomDocument *doc){
    qint64 cursor = doc->getCursor();
    QVector<VToolRecord> *history = doc->getHistory();
    if(cursor <= 0){
        history->append(VToolRecord(id, toolType, doc->GetNameActivDraw()));
    } else {
        qint32 index = 0;
        for(qint32 i = 0; i<history->size(); ++i){
            VToolRecord rec = history->at(i);
            if(rec.getId() == cursor){
                index = i;
                break;
            }
        }
        history->insert(index+1, VToolRecord(id, toolType, doc->GetNameActivDraw()));
    }
}

void VDrawTool::ignoreContextMenu(bool enable){
    ignoreContextMenuEvent = enable;
}

void VDrawTool::ShowTool(qint64 id, Qt::GlobalColor color, bool enable){
    Q_UNUSED(id);
    Q_UNUSED(color);
    Q_UNUSED(enable);
}

void VDrawTool::ChangedActivDraw(const QString newName){
    if(nameActivDraw == newName){
        ignoreContextMenuEvent = false;
    } else {
        ignoreContextMenuEvent = true;
    }
}

void VDrawTool::ChangedNameDraw(const QString oldName, const QString newName){
    if(nameActivDraw == oldName){
        nameActivDraw = newName;
    }
}

void VDrawTool::AddToCalculation(const QDomElement &domElement){
    QDomElement calcElement;
    bool ok = doc->GetActivCalculationElement(calcElement);
    if(ok){
        qint64 id = doc->getCursor();
        if(id <= 0){
            calcElement.appendChild(domElement);
        } else {
            QDomElement refElement = doc->elementById(QString().setNum(doc->getCursor()));
            if(refElement.isElement()){
                calcElement.insertAfter(domElement,refElement);
                doc->setCursor(0);
            } else {
                qCritical()<<"Не можу знайти елемент після якого потрібно вставляти."<< Q_FUNC_INFO;
            }
        }
    } else {
        qCritical()<<"Can't find tag Calculation"<< Q_FUNC_INFO;
    }
    emit toolhaveChange();
}
