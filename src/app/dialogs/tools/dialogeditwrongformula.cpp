/************************************************************************
 **
 **  @file   dialogeditwrongformula.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   29 5, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#include "dialogeditwrongformula.h"
#include "ui_dialogeditwrongformula.h"

#include <container/calculator.h>

//---------------------------------------------------------------------------------------------------------------------
DialogEditWrongFormula::DialogEditWrongFormula(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogEditWrongFormula), formula(QString())
{
    ui->setupUi(this);
    InitVariables(ui);
    labelResultCalculation = ui->labelResult;
    lineEditFormula = ui->lineEditFormula;
    labelEditFormula = ui->labelFormula;

    InitOkCancel(ui);
    flagFormula = false;
    CheckState();


    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogEditWrongFormula::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogEditWrongFormula::PutVal);

    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogEditWrongFormula::EvalFormula);
    connect(ui->lineEditFormula, &QLineEdit::textChanged, this, &DialogEditWrongFormula::FormulaChanged);

    //Disable Qt::WaitCursor
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

//---------------------------------------------------------------------------------------------------------------------
DialogEditWrongFormula::~DialogEditWrongFormula()
{
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::DialogAccepted()
{
    formula = ui->lineEditFormula->text();
    emit DialogClosed(QDialog::Accepted);
    accepted();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::DialogRejected()
{
    emit DialogClosed(QDialog::Rejected);
    rejected();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::CheckState()
{
    Q_CHECK_PTR(bOk);
    bOk->setEnabled(flagFormula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::setFormula(const QString &value)
{
    formula = qApp->FormulaToUser(value);
    ui->lineEditFormula->setText(formula);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogEditWrongFormula::getFormula() const
{
    return qApp->FormulaFromUser(formula);
}
