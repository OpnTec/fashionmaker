#include "vabstracttool.h"

VAbstractTool::VAbstractTool(VDomDocument *doc, VContainer *data, qint64 id){
    this->doc = doc;
    this->data = data;
    this->id = id;
    nameActivDraw = doc->GetNameActivDraw();
    ignoreContextMenuEvent = false;//don't ignore context menu events;

    connect(this->doc, &VDomDocument::ChangedActivDraw, this, &VAbstractTool::ChangedActivDraw);
    connect(this->doc, &VDomDocument::ChangedNameDraw, this, &VAbstractTool::ChangedNameDraw);
    connect(this, &VAbstractTool::haveLiteChange, this->doc, &VDomDocument::haveLiteChange);
    connect(this->doc, &VDomDocument::FullUpdateFromFile, this, &VAbstractTool::FullUpdateFromFile);
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

QString VAbstractTool::GetNameLine(const qint64 &firstPoint, const qint64 &secondPoint) const{
    VPointF first = data->GetPoint(firstPoint);
    VPointF second = data->GetPoint(secondPoint);
    return QString("Line_%1_%2").arg(first.name(), second.name());
}

void VAbstractTool::AddLine(const qint64 &firstPointId, const qint64 &secondPointId) const{
    QString nameLine = GetNameLine(firstPointId, secondPointId);
    VPointF firstPoint = data->GetPoint(firstPointId);
    VPointF secondPoint = data->GetPoint(secondPointId);
    data->AddLine(nameLine, QLineF(firstPoint.toQPointF(), secondPoint.toQPointF()).length());
}
