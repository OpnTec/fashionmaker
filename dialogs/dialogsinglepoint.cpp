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
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#include "dialogsinglepoint.h"
#include "ui_dialogsinglepoint.h"

DialogSinglePoint::DialogSinglePoint(const VContainer *data, QWidget *parent) :
    DialogTool(data, Draw::Calculation, parent), ui(new Ui::DialogSinglePoint), name(QString()),
    point(QPointF()){
    ui->setupUi(this);
    ui->doubleSpinBoxX->setRange(0,PaperSize/PrintDPI*25.4);
    ui->doubleSpinBoxY->setRange(0,PaperSize/PrintDPI*25.4);
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    labelEditNamePoint = ui->labelEditName;
    flagName = false;
    CheckState();
    connect(bOk, &QPushButton::clicked, this, &DialogSinglePoint::DialogAccepted);
    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogSinglePoint::DialogRejected);
    connect(ui->lineEditName,&QLineEdit::textChanged, this, &DialogSinglePoint::NamePointChanged);
}

void DialogSinglePoint::mousePress(QPointF scenePos){
    if(isInitialized == false){
        ui->doubleSpinBoxX->setValue(scenePos.x()/PrintDPI*25.4);
        ui->doubleSpinBoxY->setValue(scenePos.y()/PrintDPI*25.4);
        this->show();
    } else {
        ui->doubleSpinBoxX->setValue(scenePos.x()/PrintDPI*25.4);
        ui->doubleSpinBoxY->setValue(scenePos.y()/PrintDPI*25.4);
    }
}

void DialogSinglePoint::DialogAccepted(){
    point = QPointF(ui->doubleSpinBoxX->value()*PrintDPI/25.4,
                    ui->doubleSpinBoxY->value()*PrintDPI/25.4);
    name = ui->lineEditName->text();
    emit DialogClosed(QDialog::Accepted);
}

void DialogSinglePoint::setData(const QString name, const QPointF point){
    this->name = name;
    this->point = point;
    isInitialized = true;
    ui->lineEditName->setText(name);
    ui->doubleSpinBoxX->setValue(point.x()/PrintDPI*25.4);
    ui->doubleSpinBoxY->setValue(point.y()/PrintDPI*25.4);
}

DialogSinglePoint::~DialogSinglePoint(){
    delete ui;
}

