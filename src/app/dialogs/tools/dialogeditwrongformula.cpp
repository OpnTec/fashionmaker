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
 **  Copyright (C) 2013-2015 Valentina project
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
#include "../../container/vcontainer.h"

//---------------------------------------------------------------------------------------------------------------------
DialogEditWrongFormula::DialogEditWrongFormula(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogEditWrongFormula), formula(QString()), formulaBaseHeight(0),
      checkZero(false), postfix(QString()), restoreCursor(false)
{
    ui->setupUi(this);
    InitVariables();
    InitFormulaUI(ui);
    this->formulaBaseHeight = ui->plainTextEditFormula->height();
    ui->plainTextEditFormula->installEventFilter(this);

    InitOkCancel(ui);
    flagFormula = false;
    CheckState();

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogEditWrongFormula::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogEditWrongFormula::PutVal);

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
/**
 * @brief ValChenged show description when current variable changed
 * @param row number of row
 */
void DialogEditWrongFormula::ValChenged(int row)
{
    if (ui->listWidget->count() == 0)
    {
        ui->labelDescription->setText("");
        return;
    }
    QListWidgetItem *item = ui->listWidget->item( row );
    if (ui->radioButtonSizeGrowth->isChecked())
    {
        if (item->text()==data->HeightName())
        {
            QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->height()).arg(tr("Height"));
            ui->labelDescription->setText(desc);
        }
        if (item->text()==data->SizeName())
        {
            QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->size()).arg(tr("Size"));
            ui->labelDescription->setText(desc);
        }
        return;
    }
    if (ui->radioButtonStandardTable->isChecked())
    {
        QString name = qApp->VarFromUser(item->text());
        QSharedPointer<VMeasurement> stable = data->GetVariable<VMeasurement>(name);
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetTableValue(name))
                .arg(stable->GetGuiText());
        ui->labelDescription->setText(desc);
        return;
    }
    if (ui->radioButtonIncrements->isChecked())
    {
        QSharedPointer<VIncrement> incr = data->GetVariable<VIncrement>(item->text());
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetTableValue(item->text()))
                .arg(incr->GetDescription());
        ui->labelDescription->setText(desc);
        return;
    }
    if (ui->radioButtonLengthLine->isChecked())
    {
        QString desc = QString("%1(%2) - %3").arg(item->text())
                .arg(*data->GetVariable<VLengthLine>(qApp->VarFromUser(item->text()))->GetValue())
                .arg(tr("Line length"));
        ui->labelDescription->setText(desc);
        return;
    }
    if (ui->radioButtonLengthArc->isChecked())
    {
        QString desc = QString("%1(%2) - %3").arg(item->text())
                .arg(*data->GetVariable<VArcLength>(qApp->VarFromUser(item->text()))->GetValue())
                .arg(tr("Arc length"));
        ui->labelDescription->setText(desc);
        return;
    }
    if (ui->radioButtonLengthSpline->isChecked())
    {
        QString desc = QString("%1(%2) - %3").arg(item->text())
                .arg(*data->GetVariable<VSplineLength>(qApp->VarFromUser(item->text()))->GetValue())
                .arg(tr("Curve length"));
        ui->labelDescription->setText(desc);
        return;
    }
    if (ui->radioButtonAngleLine->isChecked())
    {
        QString desc = QString("%1(%2) - %3").arg(item->text())
                .arg(*data->GetVariable<VLineAngle>(qApp->VarFromUser(item->text()))->GetValue())
                .arg(tr("Line Angle"));
        ui->labelDescription->setText(desc);
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SizeHeight show in list base variables
 */
void DialogEditWrongFormula::SizeHeight()
{
    ui->checkBoxHideEmpty->setEnabled(false);

    ui->listWidget->blockSignals(true);
    ui->listWidget->clear();

    {
    QListWidgetItem *item = new QListWidgetItem(data->HeightName());
    item->setFont(QFont("Times", 12, QFont::Bold));
    ui->listWidget->addItem(item);
    }

    QListWidgetItem *item = new QListWidgetItem(data->SizeName());
    item->setFont(QFont("Times", 12, QFont::Bold));
    ui->listWidget->addItem(item);

    ui->listWidget->blockSignals(false);
    ui->listWidget->setCurrentRow (0);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PutHere put variable into edit
 */
void DialogEditWrongFormula::PutHere()
{
    const QListWidgetItem *item = ui->listWidget->currentItem();
    if (item != nullptr)
    {
        QTextCursor cursor = ui->plainTextEditFormula->textCursor();
        cursor.insertText(item->text());
        ui->plainTextEditFormula->setTextCursor(cursor);
        ui->plainTextEditFormula->setFocus();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PutVal put variable into edit
 * @param item chosen item of list widget
 */
void DialogEditWrongFormula::PutVal(QListWidgetItem *item)
{
    SCASSERT(item != nullptr);
    QTextCursor cursor = ui->plainTextEditFormula->textCursor();
    cursor.insertText(item->text());
    ui->plainTextEditFormula->setTextCursor(cursor);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Measurements show in list measurements
 */
void DialogEditWrongFormula::Measurements()
{
    ui->checkBoxHideEmpty->setEnabled(true);
    ShowVariable(data->DataMeasurements());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LengthLines show in list lengths of lines variables
 */
void DialogEditWrongFormula::LengthLines()
{
    ui->checkBoxHideEmpty->setEnabled(false);
    ShowVariable(data->DataLengthLines());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LengthArcs show in list lengths of arcs variables
 */
void DialogEditWrongFormula::LengthArcs()
{
    ui->checkBoxHideEmpty->setEnabled(false);
    ShowVariable(data->DataLengthArcs());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LengthCurves show in list lengths of curves variables
 */
void DialogEditWrongFormula::LengthCurves()
{
    ui->checkBoxHideEmpty->setEnabled(false);
    ShowVariable(data->DataLengthSplines());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::AngleLines()
{
    ui->checkBoxHideEmpty->setEnabled(false);
    ShowVariable(data->DataAngleLines());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Increments show in list increment variables
 */
void DialogEditWrongFormula::Increments()
{
    ui->checkBoxHideEmpty->setEnabled(false);
    ShowVariable(data->DataIncrements());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::CheckState()
{
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(flagFormula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::SetFormula(const QString &value)
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
QString DialogEditWrongFormula::GetFormula() const
{
    return qApp->FormulaFromUser(formula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::InitVariables()
{
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogEditWrongFormula::ValChenged);

    if (qApp->patternType() == MeasurementsType::Standard)
    {
        SizeHeight();
        connect(ui->radioButtonSizeGrowth, &QRadioButton::clicked, this, &DialogEditWrongFormula::SizeHeight);
    }
    else
    {
        ui->radioButtonSizeGrowth->setEnabled(false);
        ui->radioButtonStandardTable->setChecked(true);
        Measurements();
    }
    connect(ui->radioButtonStandardTable, &QRadioButton::clicked, this, &DialogEditWrongFormula::Measurements);
    connect(ui->radioButtonIncrements, &QRadioButton::clicked, this, &DialogEditWrongFormula::Increments);
    connect(ui->radioButtonLengthLine, &QRadioButton::clicked, this, &DialogEditWrongFormula::LengthLines);
    connect(ui->radioButtonLengthArc, &QRadioButton::clicked, this, &DialogEditWrongFormula::LengthArcs);
    connect(ui->radioButtonLengthSpline, &QRadioButton::clicked, this, &DialogEditWrongFormula::LengthCurves);
    connect(ui->radioButtonAngleLine, &QRadioButton::clicked, this, &DialogEditWrongFormula::AngleLines);
    connect(ui->checkBoxHideEmpty, &QCheckBox::stateChanged, this, &DialogEditWrongFormula::Measurements);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowVariable show variables in list
 * @param var container with variables
 */
template <class key, class val>
void DialogEditWrongFormula::ShowVariable(const QMap<key, val> var)
{
    ui->listWidget->blockSignals(true);
    ui->listWidget->clear();
    ui->labelDescription->setText("");

    QMapIterator<key, val> iMap(var);
    while (iMap.hasNext())
    {
        iMap.next();
        if (ui->checkBoxHideEmpty->isEnabled() && ui->checkBoxHideEmpty->isChecked() && iMap.value()->IsNotUsed())
        {
            continue; //skip this measurement
        }
        if (iMap.value()->Filter(toolId) == false)
        {// If we create this variable don't show
            QListWidgetItem *item = new QListWidgetItem(iMap.key());
            item->setFont(QFont("Times", 12, QFont::Bold));
            ui->listWidget->addItem(item);
        }
    }
    ui->listWidget->blockSignals(false);
    ui->listWidget->setCurrentRow (0);
}
