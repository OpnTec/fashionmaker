/************************************************************************
 **
 **  @file   dialogindividualmeasurements.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   22 2, 2014
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

#include "dialogindividualmeasurements.h"
#include "ui_dialogindividualmeasurements.h"
#include <QButtonGroup>

DialogIndividualMeasurements::DialogIndividualMeasurements(QWidget *parent) :
    QDialog(parent), ui(new Ui::DialogIndividualMeasurements), _name(QString()), _tablePath(QString())
{
    ui->setupUi(this);

    {
        const QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
        Q_CHECK_PTR(bOk);
        connect(bOk, &QPushButton::clicked, this, &DialogIndividualMeasurements::DialogAccepted);
    }
    {
        const QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
        Q_CHECK_PTR(bCansel);
        connect(bCansel, &QPushButton::clicked, this, &DialogIndividualMeasurements::DialogRejected);
    }

    CheckState();
    connect(ui->lineEditName, &QLineEdit::textChanged, this, &DialogIndividualMeasurements::CheckState);
    connect(ui->buttonGroupPath, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), this,
            &DialogIndividualMeasurements::CheckState);
}

DialogIndividualMeasurements::~DialogIndividualMeasurements()
{
    delete ui;
}

void DialogIndividualMeasurements::DialogAccepted()
{
    _name = ui->lineEditName->text();
    if (ui->radioButtonExistM->isChecked())
    {
        _tablePath = ui->lineEditPathExistM->text();
    }
    else
    {
        _tablePath = ui->lineEditPathNewM->text();
    }
    accept();
}

void DialogIndividualMeasurements::DialogRejected()
{
    _name = "";
    _tablePath = "";
    reject();
}

void DialogIndividualMeasurements::CheckState()
{
    bool flagName = false;
    if (ui->lineEditName->text().isEmpty() == false)
    {
        flagName = true;
    }

    bool flagPath = false;
    if (ui->radioButtonExistM->isChecked())
    {
        ui->lineEditPathExistM->setEnabled(true);
        ui->toolButtonOpenExist->setEnabled(true);

        ui->lineEditPathNewM->setEnabled(false);
        ui->toolButtonOpenNew->setEnabled(false);
    }
    else
    {
        ui->lineEditPathNewM->setEnabled(true);
        ui->toolButtonOpenNew->setEnabled(true);

        ui->toolButtonOpenExist->setEnabled(false);
        ui->lineEditPathExistM->setEnabled(false);

    }

    QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    Q_CHECK_PTR(bOk);
    bOk->setEnabled(flagName && flagPath);
}
