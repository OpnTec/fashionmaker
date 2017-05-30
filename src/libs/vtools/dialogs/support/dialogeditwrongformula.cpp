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

#include <qiterator.h>
#include <QAbstractItemView>
#include <QApplication>
#include <QCheckBox>
#include <QCursor>
#include <QDialog>
#include <QFont>
#include <QHeaderView>
#include <QLabel>
#include <QMapIterator>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSharedPointer>
#include <QShowEvent>
#include <QSize>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextCursor>
#include <QToolButton>
#include <QWidget>
#include <Qt>
#include <new>

#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vpatterndb/variables/varcradius.h"
#include "../vpatterndb/variables/vcurveangle.h"
#include "../vpatterndb/variables/vcurvelength.h"
#include "../vpatterndb/variables/vincrement.h"
#include "../vpatterndb/variables/vlineangle.h"
#include "../vpatterndb/variables/vlinelength.h"
#include "../vpatterndb/variables/vmeasurement.h"
#include "../ifc/xml/vdomdocument.h"
#include "../vmisc/def.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../tools/dialogtool.h"
#include "ui_dialogeditwrongformula.h"

template <class T> class QSharedPointer;

enum {ColumnName = 0, ColumnFullName};

//---------------------------------------------------------------------------------------------------------------------
DialogEditWrongFormula::DialogEditWrongFormula(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogEditWrongFormula), formula(QString()), formulaBaseHeight(0),
      checkZero(false), checkLessThanZero(false), postfix(QString()), restoreCursor(false)
{
    ui->setupUi(this);
    InitVariables();
    InitFormulaUI(ui);
    this->formulaBaseHeight = ui->plainTextEditFormula->height();
    ui->plainTextEditFormula->installEventFilter(this);
    ui->filterFormulaInputs->setClearButtonEnabled(true);

    connect(ui->filterFormulaInputs, &QLineEdit::textChanged, this, &DialogEditWrongFormula::FilterVariablesEdited);

    InitOkCancel(ui);
    flagFormula = false;
    CheckState();

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogEditWrongFormula::PutHere);
    connect(ui->tableWidget, &QTableWidget::itemDoubleClicked, this, &DialogEditWrongFormula::PutVal);

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
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
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
    SCASSERT(plainTextEditFormula != nullptr)
    SCASSERT(labelResultCalculation != nullptr)
    Eval(plainTextEditFormula->toPlainText(), flagFormula, labelResultCalculation, postfix, checkZero,
         checkLessThanZero);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ValChanged show description when current variable changed
 * @param row number of row
 */
void DialogEditWrongFormula::ValChanged(int row)
{
    if (ui->tableWidget->rowCount() == 0)
    {
        ui->labelDescription->setText("");
        return;
    }
    QTableWidgetItem *item = ui->tableWidget->item( row, ColumnName );
    if (ui->radioButtonStandardTable->isChecked())
    {
        const QString name = qApp->TrVars()->VarFromUser(item->text());
        const QSharedPointer<VMeasurement> stable = data->GetVariable<VMeasurement>(name);
        SetDescription(item->text(), data->GetTableValue(name, qApp->patternType()),
                       VDomDocument::UnitsToStr(qApp->patternUnit(), true), stable->GetGuiText());
        return;
    }
    if (ui->radioButtonIncrements->isChecked())
    {
        const QSharedPointer<VIncrement> incr = data->GetVariable<VIncrement>(item->text());
        SetDescription(item->text(), data->GetTableValue(item->text(), qApp->patternType()),
                       VDomDocument::UnitsToStr(qApp->patternUnit(), true), incr->GetDescription());
        return;
    }
    if (ui->radioButtonLengthLine->isChecked())
    {
        SetDescription(item->text(),
                       *data->GetVariable<VLengthLine>(qApp->TrVars()->VarFromUser(item->text()))->GetValue(),
                       VDomDocument::UnitsToStr(qApp->patternUnit(), true), tr("Line length"));
        return;
    }
    if (ui->radioButtonLengthSpline->isChecked())
    {
        SetDescription(item->text(),
                       *data->GetVariable<VCurveLength>(qApp->TrVars()->VarFromUser(item->text()))->GetValue(),
                       VDomDocument::UnitsToStr(qApp->patternUnit(), true), tr("Curve length"));
        return;
    }
    if (ui->radioButtonAngleLine->isChecked())
    {
        SetDescription(item->text(),
                       *data->GetVariable<VLineAngle>(qApp->TrVars()->VarFromUser(item->text()))->GetValue(),
                       degreeSymbol, tr("Line Angle"));
        return;
    }
    if (ui->radioButtonRadiusesArcs->isChecked())
    {
        SetDescription(item->text(),
                       *data->GetVariable<VArcRadius>(qApp->TrVars()->VarFromUser(item->text()))->GetValue(),
                       VDomDocument::UnitsToStr(qApp->patternUnit(), true), tr("Arc radius"));
        return;
    }
    if (ui->radioButtonAnglesCurves->isChecked())
    {
        SetDescription(item->text(),
                       *data->GetVariable<VCurveAngle>(qApp->TrVars()->VarFromUser(item->text()))->GetValue(),
                       degreeSymbol, tr("Curve angle"));
        return;
    }
    if (ui->radioButtonFunctions->isChecked())
    {
        ui->labelDescription->setText(item->toolTip());
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PutHere put variable into edit
 */
void DialogEditWrongFormula::PutHere()
{
    const QTableWidgetItem *item = ui->tableWidget->currentItem();
    if (item != nullptr)
    {
        QTextCursor cursor = ui->plainTextEditFormula->textCursor();
        cursor.insertText(ui->tableWidget->item(item->row(), ColumnName)->text());
        ui->plainTextEditFormula->setTextCursor(cursor);
        ui->plainTextEditFormula->setFocus();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PutVal put variable into edit
 * @param item chosen item of table widget
 */
void DialogEditWrongFormula::PutVal(QTableWidgetItem *item)
{
    SCASSERT(item != nullptr)
    QTextCursor cursor = ui->plainTextEditFormula->textCursor();
    cursor.insertText(ui->tableWidget->item(item->row(), ColumnName)->text());
    ui->plainTextEditFormula->setTextCursor(cursor);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Measurements show in list measurements
 */
void DialogEditWrongFormula::Measurements()
{
    ui->checkBoxHideEmpty->setEnabled(true);
    ShowMeasurements(data->DataMeasurements());
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
void DialogEditWrongFormula::RadiusArcs()
{
    ui->checkBoxHideEmpty->setEnabled(false);
    ShowVariable(data->DataRadiusesArcs());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::AnglesCurves()
{
    ui->checkBoxHideEmpty->setEnabled(false);
    ShowVariable(data->DataAnglesCurves());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LengthCurves show in list lengths of curves variables
 */
void DialogEditWrongFormula::LengthCurves()
{
    ui->checkBoxHideEmpty->setEnabled(false);
    ShowVariable(data->DataLengthCurves());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::CurvesCLength()
{
    ui->checkBoxHideEmpty->setEnabled(false);
    ShowVariable(data->DataCurvesCLength());
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
/**
 * @brief Functions show in list functions
 */
void DialogEditWrongFormula::Functions()
{
    ui->checkBoxHideEmpty->setEnabled(false);
    ShowFunctions();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::CheckState()
{
    SCASSERT(bOk != nullptr)
    bOk->setEnabled(flagFormula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::showEvent(QShowEvent *event)
{
    DialogTool::showEvent( event );
    if ( event->spontaneous() )
    {
        return;
    }

    setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    setMinimumSize(QSize(0, 0));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::SetFormula(const QString &value)
{
    formula = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
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
void DialogEditWrongFormula::setCheckLessThanZero(bool value)
{
    checkLessThanZero = value;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::setPostfix(const QString &value)
{
    postfix = value;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogEditWrongFormula::GetFormula() const
{
    return qApp->TrVars()->TryFormulaFromUser(formula, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::InitVariables()
{
    connect(ui->tableWidget, &QTableWidget::currentCellChanged, this, &DialogEditWrongFormula::ValChanged);

    ui->radioButtonStandardTable->setChecked(true);
    Measurements();

    // clear text filter every time when new radio button selected
    auto ClearFilterFormulaInputs = [this] () { ui->filterFormulaInputs->clear(); };

    connect(ui->radioButtonStandardTable, &QRadioButton::clicked, this, &DialogEditWrongFormula::Measurements);
    connect(ui->radioButtonStandardTable, &QRadioButton::clicked, this, ClearFilterFormulaInputs);

    connect(ui->radioButtonIncrements, &QRadioButton::clicked, this, &DialogEditWrongFormula::Increments);
    connect(ui->radioButtonIncrements, &QRadioButton::clicked, this, ClearFilterFormulaInputs);

    connect(ui->radioButtonLengthLine, &QRadioButton::clicked, this, &DialogEditWrongFormula::LengthLines);
    connect(ui->radioButtonLengthLine, &QRadioButton::clicked, this, ClearFilterFormulaInputs);

    connect(ui->radioButtonLengthSpline, &QRadioButton::clicked, this, &DialogEditWrongFormula::LengthCurves);
    connect(ui->radioButtonLengthSpline, &QRadioButton::clicked, this, ClearFilterFormulaInputs);

    connect(ui->radioButtonAngleLine, &QRadioButton::clicked, this, &DialogEditWrongFormula::AngleLines);
    connect(ui->radioButtonAngleLine, &QRadioButton::clicked, this, ClearFilterFormulaInputs);

    connect(ui->radioButtonRadiusesArcs, &QRadioButton::clicked, this, &DialogEditWrongFormula::RadiusArcs);
    connect(ui->radioButtonRadiusesArcs, &QRadioButton::clicked, this, ClearFilterFormulaInputs);

    connect(ui->radioButtonAnglesCurves, &QRadioButton::clicked, this, &DialogEditWrongFormula::AnglesCurves);
    connect(ui->radioButtonAnglesCurves, &QRadioButton::clicked, this, ClearFilterFormulaInputs);

    connect(ui->radioButtonCLength, &QRadioButton::clicked, this, &DialogEditWrongFormula::CurvesCLength);
    connect(ui->radioButtonCLength, &QRadioButton::clicked, this, ClearFilterFormulaInputs);

    connect(ui->radioButtonFunctions, &QRadioButton::clicked, this, &DialogEditWrongFormula::Functions);
    connect(ui->radioButtonFunctions, &QRadioButton::clicked, this, ClearFilterFormulaInputs);

    connect(ui->checkBoxHideEmpty, &QCheckBox::stateChanged, this, &DialogEditWrongFormula::Measurements);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::SetDescription(const QString &name, qreal value, const QString &unit,
                                            const QString &description)
{
    const QString desc = QString("%1(%2 %3) - %4").arg(name).arg(value).arg(unit).arg(description);
    ui->labelDescription->setText(desc);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowVariable show variables in list
 * @param var container with variables
 */
template <class key, class val>
void DialogEditWrongFormula::ShowVariable(const QMap<key, val> &var)
{
    ui->tableWidget->blockSignals(true);
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnHidden(ColumnFullName, true);
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
            ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
            QTableWidgetItem *item = new QTableWidgetItem(iMap.key());
            item->setFont(QFont("Times", 12, QFont::Bold));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, ColumnName, item);
        }
    }
    ui->tableWidget->blockSignals(false);
    ui->tableWidget->selectRow(0);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowMeasurements show measurements in table
 * @param var container with measurements
 */
void DialogEditWrongFormula::ShowMeasurements(const QMap<QString, QSharedPointer<VMeasurement> > &var)
{
    ui->tableWidget->blockSignals(true);
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnHidden(ColumnFullName, false);
    ui->labelDescription->setText("");

    QMapIterator<QString, QSharedPointer<VMeasurement>> iMap(var);
    while (iMap.hasNext())
    {
        iMap.next();
        if (ui->checkBoxHideEmpty->isEnabled() && ui->checkBoxHideEmpty->isChecked() && iMap.value()->IsNotUsed())
        {
            continue; //skip this measurement
        }
        if (iMap.value()->Filter(toolId) == false)
        {// If we create this variable don't show
            ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
            QTableWidgetItem *itemName = new QTableWidgetItem(iMap.key());
            itemName->setFont(QFont("Times", 12, QFont::Bold));
            itemName->setToolTip(itemName->text());

            QTableWidgetItem *itemFullName = new QTableWidgetItem();
            itemFullName->setFont(QFont("Times", 12, QFont::Bold));
            if (iMap.value()->IsCustom())
            {
                itemFullName->setText(iMap.value()->GetGuiText());
            }
            else
            {
                itemFullName->setText(qApp->TrVars()->GuiText(iMap.value()->GetName()));
            }

            itemFullName->setToolTip(itemFullName->text());
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, ColumnName, itemName);
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, ColumnFullName, itemFullName);
        }
    }
    ui->tableWidget->blockSignals(false);
    ui->tableWidget->selectRow(0);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowFunctions show functions in list
 */
void DialogEditWrongFormula::ShowFunctions()
{
    ui->tableWidget->blockSignals(true);
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnHidden(ColumnFullName, true);
    ui->labelDescription->setText("");

    QMap<QString, qmu::QmuTranslation>::const_iterator i = qApp->TrVars()->GetFunctions().constBegin();
    while (i != qApp->TrVars()->GetFunctions().constEnd())
    {
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
        QTableWidgetItem *item = new QTableWidgetItem(i.value().translate());
        item->setFont(QFont("Times", 12, QFont::Bold));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, ColumnName, item);
        item->setToolTip(i.value().getMdisambiguation());
        ++i;
    }

    ui->tableWidget->blockSignals(false);
    ui->tableWidget->selectRow(0);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::FilterVariablesEdited(const QString &filter)
{
    ui->tableWidget->blockSignals(true);

    // If filter is empty findItems() for unknown reason returns nullptr items.
    // See issue #586. https://bitbucket.org/dismine/valentina/issues/586/valentina-crashes-if-clear-input-filter
    if (filter.isEmpty())
    {
        // show all rows
        for (auto i = 0; i < ui->tableWidget->rowCount(); ++i)
        {
            ui->tableWidget->showRow(i);
        }
    }
    else
    {
        // hide all rows
        for (auto i = 0; i < ui->tableWidget->rowCount(); i++)
        {
            ui->tableWidget->hideRow(i);
        }

        // show rows with matched filter
        for (auto item : ui->tableWidget->findItems(filter, Qt::MatchContains))
        {
            // If filter is empty findItems() for unknown reason returns nullptr items.
            if (item)
            {
                ui->tableWidget->showRow(item->row());
            }
        }
    }

    ui->tableWidget->blockSignals(false);
}
