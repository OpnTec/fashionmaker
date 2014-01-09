/************************************************************************
 **
 **  @file   dialogsinglepoint.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
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
 *************************************************************************/

#include "dialogsinglepoint.h"
#include "ui_dialogsinglepoint.h"

#include <QPushButton>

DialogSinglePoint::DialogSinglePoint(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogSinglePoint), name(QString()),
    point(QPointF())
{
    ui->setupUi(this);
    ui->doubleSpinBoxX->setRange(0, toMM(SceneSize));
    ui->doubleSpinBoxY->setRange(0, toMM(SceneSize));
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    labelEditNamePoint = ui->labelEditName;
    flagName = false;
    CheckState();
    connect(bOk, &QPushButton::clicked, this, &DialogSinglePoint::DialogAccepted);
    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogSinglePoint::DialogRejected);
    connect(ui->lineEditName, &QLineEdit::textChanged, this, &DialogSinglePoint::NamePointChanged);
}

void DialogSinglePoint::mousePress(const QPointF &scenePos)
{
    if (isInitialized == false)
    {
        ui->doubleSpinBoxX->setValue(toMM(scenePos.x()));
        ui->doubleSpinBoxY->setValue(toMM(scenePos.y()));
        this->show();
    }
    else
    {
        ui->doubleSpinBoxX->setValue(toMM(scenePos.x()));
        ui->doubleSpinBoxY->setValue(toMM(scenePos.y()));
    }
}

void DialogSinglePoint::DialogAccepted()
{
    point = QPointF(toPixel(ui->doubleSpinBoxX->value()), toPixel(ui->doubleSpinBoxY->value()));
    name = ui->lineEditName->text();
    emit DialogClosed(QDialog::Accepted);
}

void DialogSinglePoint::setData(const QString &name, const QPointF &point)
{
    this->name = name;
    this->point = point;
    isInitialized = true;
    ui->lineEditName->setText(name);
    ui->doubleSpinBoxX->setValue(toMM(point.x()));
    ui->doubleSpinBoxY->setValue(toMM(point.y()));
}

DialogSinglePoint::~DialogSinglePoint()
{
    delete ui;
}
