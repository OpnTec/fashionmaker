#include "vmodelingtool.h"
#include <QDebug>

VModelingTool::VModelingTool(VDomDocument *doc, VContainer *data, qint64 id, QObject *parent):
VAbstractTool(doc, data, id, parent), ignoreContextMenuEvent(false){
}

VModelingTool::~VModelingTool(){
}

void VModelingTool::setDialog(){
}

void VModelingTool::ignoreContextMenu(bool enable){
    ignoreContextMenuEvent = enable;
}

void VModelingTool::AddToModeling(const QDomElement &domElement){
    QDomElement modelingElement;
    bool ok = doc->GetActivModelingElement(modelingElement);
    if(ok){
        modelingElement.appendChild(domElement);
    } else {
        qCritical()<<"Can't find tag Modeling"<< Q_FUNC_INFO;
    }
    emit toolhaveChange();
}
