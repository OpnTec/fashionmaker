#pragma GCC diagnostic ignored "-Weffc++"
#include "vabstracttool.h"
#include <QDebug>
#pragma GCC diagnostic pop

VAbstractTool::VAbstractTool(VDomDocument *doc, VContainer *data, qint64 id, QObject *parent):
    VDataTool(data, parent), baseColor(Qt::black), currentColor(Qt::black){
    this->doc = doc;
    this->id = id;
    nameActivDraw = doc->GetNameActivDraw();
    ignoreContextMenuEvent = false;//don't ignore context menu events;

    connect(this->doc, &VDomDocument::ChangedActivDraw, this, &VAbstractTool::ChangedActivDraw);
    connect(this->doc, &VDomDocument::ChangedNameDraw, this, &VAbstractTool::ChangedNameDraw);
    connect(this, &VAbstractTool::toolhaveChange, this->doc, &VDomDocument::haveLiteChange);
    connect(this->doc, &VDomDocument::FullUpdateFromFile, this, &VAbstractTool::FullUpdateFromFile);
    connect(this, &VAbstractTool::FullUpdateTree, this->doc, &VDomDocument::FullUpdateTree);
    connect(this->doc, &VDomDocument::ShowTool, this, &VAbstractTool::ShowTool);
}

void VAbstractTool::ChangedNameDraw(const QString oldName, const QString newName){
    if(nameActivDraw == oldName){
        nameActivDraw = newName;
    }
}

void VAbstractTool::ChangedActivDraw(const QString newName){
    if(nameActivDraw == newName){
        ignoreContextMenuEvent = false;
    } else {
        ignoreContextMenuEvent = true;
    }
}

void VAbstractTool::ShowTool(qint64 id, Qt::GlobalColor color, bool enable){
    Q_UNUSED(id);
    Q_UNUSED(color);
    Q_UNUSED(enable);
}

void VAbstractTool::AddAttribute(QDomElement &domElement, const QString &name, const qint64 &value){
    QDomAttr domAttr = doc->createAttribute(name);
    domAttr.setValue(QString().setNum(value));
    domElement.setAttributeNode(domAttr);
}

void VAbstractTool::AddAttribute(QDomElement &domElement, const QString &name, const qint32 &value){
    QDomAttr domAttr = doc->createAttribute(name);
    domAttr.setValue(QString().setNum(value));
    domElement.setAttributeNode(domAttr);
}

void VAbstractTool::AddAttribute(QDomElement &domElement, const QString &name, const qreal &value){
    QDomAttr domAttr = doc->createAttribute(name);
    domAttr.setValue(QString().setNum(value));
    domElement.setAttributeNode(domAttr);
}

void VAbstractTool::AddAttribute(QDomElement &domElement, const QString &name, const QString &value){
    QDomAttr domAttr = doc->createAttribute(name);
    domAttr.setValue(value);
    domElement.setAttributeNode(domAttr);
}

VAbstractTool::~VAbstractTool(){
}

void VAbstractTool::AddToCalculation(const QDomElement &domElement){
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
        qCritical()<<"Не можу знайти тег калькуляції."<< Q_FUNC_INFO;
    }
    emit toolhaveChange();
}

const VContainer *VAbstractTool::getData()const{
    return &data;
}

void VAbstractTool::setData(const VContainer &value){
    data = value;
}

void VAbstractTool::setDialog(){
}

void VAbstractTool::AddRecord(const qint64 id, Tools::Enum toolType, VDomDocument *doc){
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
