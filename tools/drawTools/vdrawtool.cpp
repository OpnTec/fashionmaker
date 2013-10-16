/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#include "vdrawtool.h"
#include <QDebug>

qreal VDrawTool::factor = 1;

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

void VDrawTool::AddRecord(const qint64 id, Tool::Tools toolType, VDomDocument *doc){
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

void VDrawTool::SetFactor(qreal factor){
    if(factor <= 2 && factor >= 0.5){
        this->factor = factor;
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
