#pragma GCC diagnostic ignored "-Weffc++"
#include "vabstracttool.h"
#include <QDebug>
#pragma GCC diagnostic pop

VAbstractTool::VAbstractTool(VDomDocument *doc, VContainer *data, qint64 id, QObject *parent):
    VDataTool(data, parent){
    this->doc = doc;

    this->id = id;
    nameActivDraw = doc->GetNameActivDraw();
    ignoreContextMenuEvent = false;//don't ignore context menu events;

    connect(this->doc, &VDomDocument::ChangedActivDraw, this, &VAbstractTool::ChangedActivDraw);
    connect(this->doc, &VDomDocument::ChangedNameDraw, this, &VAbstractTool::ChangedNameDraw);
    connect(this, &VAbstractTool::toolhaveChange, this->doc, &VDomDocument::haveLiteChange);
    connect(this->doc, &VDomDocument::FullUpdateFromFile, this, &VAbstractTool::FullUpdateFromFile);
    connect(this, &VAbstractTool::FullUpdateTree, this->doc, &VDomDocument::FullUpdateTree);
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
        calcElement.appendChild(domElement);
    } else {
        qCritical()<<"Не можу знайти тег калькуляції."<< Q_FUNC_INFO;
    }
}

const VContainer *VAbstractTool::getData()const{
    return &data;
}

void VAbstractTool::setData(const VContainer &value){
    data = value;
}

void VAbstractTool::setDialog(){
}
