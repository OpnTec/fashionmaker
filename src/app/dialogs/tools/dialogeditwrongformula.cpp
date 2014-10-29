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

//---------------------------------------------------------------------------------------------------------------------
DialogEditWrongFormula::DialogEditWrongFormula(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogEditWrongFormula), formula(QString()), formulaBaseHeight(0),
      checkZero(false), postfix(QString()), restoreCursor(false)
{
    ui->setupUi(this);
    InitVariables(ui);
    InitFormulaUI(ui);
    this->formulaBaseHeight = ui->plainTextEditFormula->height();
    ui->plainTextEditFormula->installEventFilter(this);

    InitOkCancel(ui);
    flagFormula = false;
    CheckState();

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogEditWrongFormula::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogEditWrongFormula::PutVal);

    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogEditWrongFormula::EvalFormula);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogEditWrongFormula::FormulaChanged);
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogEditWrongFormula::DeployFormulaTextEdit);

    //Disable Qt::WaitCursor
#ifndef QT_NO_CURSOR
    if (QApplication::overrideCursor() != nullptr)
    {
        if (QApplication::overrideCursor()->shape() == Qt::WaitCursor)
        {
            restoreCursor = true;
            QApplication::restoreOverrideCursor();
        }
    }
#endif
}

//---------------------------------------------------------------------------------------------------------------------
DialogEditWrongFormula::~DialogEditWrongFormula()
{
#ifndef QT_NO_CURSOR
    if (restoreCursor)
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);
    }
#endif
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::DialogAccepted()
{
    formula = ui->plainTextEditFormula->toPlainText();
    formula.replace("\n", " ");
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
void DialogEditWrongFormula::DeployFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::EvalFormula()
{
    SCASSERT(plainTextEditFormula != nullptr);
    SCASSERT(labelResultCalculation != nullptr);
    Eval(plainTextEditFormula->toPlainText(), flagFormula, labelResultCalculation, postfix, checkZero);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::CheckState()
{
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(flagFormula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::setFormula(const QString &value)
{
    formula = qApp->FormulaToUser(value);
    // increase height if needed. TODO : see if I can get the max number of caracters in one line
    // of this PlainTextEdit to change 80 to this value
    if (formula.length() > 80)
    {
        this->DeployFormulaTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(formula);
    MoveCursorToEnd(ui->plainTextEditFormula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::setCheckZero(bool value)
{
    checkZero = value;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::setPostfix(const QString &value)
{
    postfix = value;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogEditWrongFormula::getFormula() const
{
    return qApp->FormulaFromUser(formula);
}
