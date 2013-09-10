#include "vabstractnode.h"
#include <QDebug>

VAbstractNode::VAbstractNode(VDomDocument *doc, VContainer *data, qint64 id, qint64 idNode,
                             Draw::Mode typeobject, QObject *parent) :
    VAbstractTool(doc, data, id, parent), idNode(idNode), typeobject(typeobject){
}

VAbstractNode::~VAbstractNode(){
}

void VAbstractNode::AddToModeling(const QDomElement &domElement){
    QDomElement modelingElement;
    bool ok = doc->GetActivModelingElement(modelingElement);
    if(ok){
        modelingElement.appendChild(domElement);
    } else {
        qCritical()<<"Can't find tag Modeling"<< Q_FUNC_INFO;
    }
    emit toolhaveChange();
}
