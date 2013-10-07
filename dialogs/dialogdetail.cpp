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

#include "dialogdetail.h"
#include <QDebug>

DialogDetail::DialogDetail(const VContainer *data, Draw::Draws mode, QWidget *parent) :
    DialogTool(data, mode, parent), ui(), details(VDetail()){
    ui.setupUi(this);
    bOk = ui.buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogDetail::DialogAccepted);

    QPushButton *bCansel = ui.buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogDetail::DialogRejected);
}

void DialogDetail::ChoosedObject(qint64 id, Scene::Scenes type){
    if(idDetail == 0 && mode == Draw::Modeling){
        if(type == Scene::Detail){
            idDetail = id;
            return;
        }
    }
    if(mode == Draw::Modeling){
        if(!CheckObject(id)){
            return;
        }
    }
    if(type != Scene::Line && type != Scene::Detail){
        switch(type){
        case(Scene::Arc):
            NewItem(id, Tool::NodeArc, mode, NodeDetail::Contour);
            break;
        case(Scene::Point):
            NewItem(id, Tool::NodePoint, mode, NodeDetail::Contour);
            break;
        case(Scene::Spline):
            NewItem(id, Tool::NodeSpline, mode, NodeDetail::Contour);
            break;
        case(Scene::SplinePath):
            NewItem(id, Tool::NodeSplinePath, mode, NodeDetail::Contour);
            break;
        default:
            qWarning()<<tr("Get wrong scene object. Ignore.");
            break;
        }
        this->show();
    }
}

void DialogDetail::DialogAccepted(){
    details.Clear();
    for(qint32 i = 0; i < ui.listWidget->count(); ++i){
        QListWidgetItem *item = ui.listWidget->item(i);
        details.append( qvariant_cast<VNodeDetail>(item->data(Qt::UserRole)));
    }
    details.setName(ui.lineEditNameDetail->text());
    emit DialogClosed(QDialog::Accepted);
    emit ToolTip("");
}

void DialogDetail::NewItem(qint64 id, Tool::Tools typeTool, Draw::Draws mode, NodeDetail::NodeDetails typeNode){
    QString name;
    switch(typeTool){
    case(Tool::NodePoint):{
        VPointF point;
        if(mode == Draw::Calculation){
            point = data->GetPoint(id);
        } else {
            point = data->GetModelingPoint(id);
        }
        name = point.name();
        break;
    }
    case(Tool::NodeArc):{
        VArc arc;
        if(mode == Draw::Calculation){
            arc = data->GetArc(id);
        } else {
            arc = data->GetModelingArc(id);
        }
        name = data->GetNameArc(arc.GetCenter(), id, mode);
        break;
    }
    case(Tool::NodeSpline):{
        VSpline spl;
        if(mode == Draw::Calculation){
            spl = data->GetSpline(id);
        } else {
            spl = data->GetModelingSpline(id);
        }
        name = spl.GetName();
        break;
    }
    case(Tool::NodeSplinePath):{
        VSplinePath splPath;
        if(mode == Draw::Calculation){
            splPath = data->GetSplinePath(id);
        } else {
            splPath = data->GetModelingSplinePath(id);
        }
        name = data->GetNameSplinePath(splPath, mode);
        break;
    }
    default:
        qWarning()<<tr("Get wrong tools. Ignore.");
        break;
    }

    QListWidgetItem *item = new QListWidgetItem(name);
    item->setFont(QFont("Times", 12, QFont::Bold));
    VNodeDetail node(id, typeTool, mode, typeNode);
    item->setData(Qt::UserRole, QVariant::fromValue(node));
    ui.listWidget->addItem(item);
}

VDetail DialogDetail::getDetails() const{
    return details;
}

void DialogDetail::setDetails(const VDetail &value){
    details = value;
    ui.listWidget->clear();
    for(qint32 i = 0; i < details.CountNode(); ++i){
        NewItem(details[i].getId(), details[i].getTypeTool(),details[i].getMode(), details[i].getTypeNode());
    }
    details.setName(ui.lineEditNameDetail->text());
    ui.listWidget->setFocus(Qt::OtherFocusReason);
}

