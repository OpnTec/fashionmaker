/************************************************************************
 **
 **  @file   dialogeditwrongformula.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   29 5, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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
#include <QTimer>

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
DialogEditWrongFormula::DialogEditWrongFormula(const VContainer *data, quint32 toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogEditWrongFormula),
      formula(),
      formulaBaseHeight(0),
      checkZero(false),
      checkLessThanZero(false),
      postfix(),
      restoreCursor(false),
      timerFormula(new QTimer(this)),
      flagFormula(false)
{
    ui->setupUi(this);

    timerFormula->setSingleShot(true);
    connect(timerFormula, &QTimer::timeout, this, &DialogEditWrongFormula::EvalFormula);

#if defined(Q_OS_MAC)
    setWindowFlags(Qt::Window);
#endif

    InitVariables();
    this->formulaBaseHeight = ui->plainTextEditFormula->height();
    ui->plainTextEditFormula->installEventFilter(this);
    ui->filterFormulaInputs->setClearButtonEnabled(true);

    connect(ui->filterFormulaInputs, &QLineEdit::textChanged, this, &DialogEditWrongFormula::FilterVariablesEdited);

    InitOkCancel(ui);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogEditWrongFormula::PutHere);
    connect(ui->tableWidget, &QTableWidget::itemDoubleClicked, this, &DialogEditWrongFormula::PutVal);

    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, [this]()
    {
        timerFormula->start(formulaTimerTimeout);
    });

    //Disable Qt::WaitCursor
#ifndef QT_NO_CURSOR
    if (QGuiApplication::overrideCursor() != nullptr)
    {
        if (QGuiApplication::overrideCursor()->shape() == Qt::WaitCursor)
        {
            restoreCursor = true;
            QGuiApplication::restoreOverrideCursor();
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
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    }
#endif
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::DialogAccepted()
{
    formula = ui->plainTextEditFormula->toPlainText();
    emit DialogClosed(QDialog::Accepted);
    emit accepted();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::DialogRejected()
{
    emit DialogClosed(QDialog::Rejected);
    emit rejected();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::EvalFormula()
{    
    FormulaData formulaData;
    formulaData.formula = ui->plainTextEditFormula->toPlainText();
    formulaData.variables = data->DataVariables();
    formulaData.labelEditFormula = ui->labelEditFormula;
    formulaData.labelResult = ui->labelResultCalculation;
    formulaData.postfix = postfix;
    formulaData.checkZero = checkZero;
    formulaData.checkLessThanZero = checkLessThanZero;

    Eval(formulaData, flagFormula);
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
        ui->labelDescription->setText(QString());
        return;
    }
    QTableWidgetItem *item = ui->tableWidget->item( row, ColumnName );
    const QString name = qApp->TrVars()->VarFromUser(item->text());

    try
    {
        if (ui->radioButtonStandardTable->isChecked())
        {
            const QSharedPointer<VMeasurement> stable = data->GetVariable<VMeasurement>(name);
            SetDescription(item->text(), *stable->GetValue(), UnitsToStr(qApp->patternUnit(), true),
                           stable->GetGuiText());
        }
        else if (ui->radioButtonIncrements->isChecked())
        {
            const QSharedPointer<VIncrement> incr = data->GetVariable<VIncrement>(name);
            SetDescription(item->text(), *incr->GetValue(), UnitsToStr(qApp->patternUnit(), true),
                           incr->GetDescription());
        }
        else if (ui->radioButtonPC->isChecked())
        {
            const QSharedPointer<VIncrement> incr = data->GetVariable<VIncrement>(name);
            SetDescription(item->text(), *incr->GetValue(), UnitsToStr(qApp->patternUnit(), true),
                           incr->GetDescription());
        }
        else if (ui->radioButtonLengthLine->isChecked())
        {
            SetDescription(item->text(), *data->GetVariable<VLengthLine>(name)->GetValue(),
                           UnitsToStr(qApp->patternUnit(), true), tr("Line length"));
        }
        else if (ui->radioButtonLengthSpline->isChecked())
        {
            SetDescription(item->text(), *data->GetVariable<VCurveLength>(name)->GetValue(),
                           UnitsToStr(qApp->patternUnit(), true), tr("Curve length"));
        }
        else if (ui->radioButtonAngleLine->isChecked())
        {
            SetDescription(item->text(), *data->GetVariable<VLineAngle>(name)->GetValue(), degreeSymbol,
                           tr("Line Angle"));
        }
        else if (ui->radioButtonRadiusesArcs->isChecked())
        {
            SetDescription(item->text(), *data->GetVariable<VArcRadius>(name)->GetValue(),
                           UnitsToStr(qApp->patternUnit(), true), tr("Arc radius"));
        }
        else if (ui->radioButtonAnglesCurves->isChecked())
        {
            SetDescription(item->text(), *data->GetVariable<VCurveAngle>(name)->GetValue(), degreeSymbol,
                           tr("Curve angle"));
        }
        else if (ui->radioButtonFunctions->isChecked())
        {
            ui->labelDescription->setText(item->toolTip());
        }
    }
    catch (const VExceptionBadId &e)
    {
        qCritical() << qUtf8Printable(e.ErrorMessage());
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
    ShowIncrementsInPreviewCalculation(false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::PreviewCalculations()
{
    ShowIncrementsInPreviewCalculation(true);
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
void DialogEditWrongFormula::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::showEvent(QShowEvent *event)
{
    QDialog::showEvent( event );
    if ( event->spontaneous() )
    {
        return;
    }

    if (isInitialized)
    {
        return;
    }
    // do your init stuff here

    const QSize sz = qApp->Settings()->GetFormulaWizardDialogSize();
    if (not sz.isEmpty())
    {
        resize(sz);
    }

    CheckState();

    isInitialized = true;//first show windows are held
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::resizeEvent(QResizeEvent *event)
{
    // remember the size for the next time this dialog is opened, but only
    // if widget was already initialized, which rules out the resize at
    // dialog creating, which would
    if (isInitialized)
    {
        qApp->Settings()->SetFormulaWizardDialogSize(size());
    }
    DialogTool::resizeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::SetFormula(const QString &value)
{
    formula = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    ui->plainTextEditFormula->setPlainText(formula);
    MoveCursorToEnd(ui->plainTextEditFormula);
    ui->plainTextEditFormula->selectAll();
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
void DialogEditWrongFormula::SetMeasurementsMode()
{
    ui->radioButtonIncrements->setDisabled(true);
    SetIncrementsMode();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::SetIncrementsMode()
{
    ui->radioButtonPC->setDisabled(true);
    SetPreviewCalculationsMode();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::SetPreviewCalculationsMode()
{
    ui->radioButtonLengthLine->setDisabled(true);
    ui->radioButtonLengthSpline->setDisabled(true);
    ui->radioButtonAngleLine->setDisabled(true);
    ui->radioButtonRadiusesArcs->setDisabled(true);
    ui->radioButtonAnglesCurves->setDisabled(true);
    ui->radioButtonCLength->setDisabled(true);
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

    connect(ui->radioButtonPC, &QRadioButton::clicked, this, &DialogEditWrongFormula::PreviewCalculations);
    connect(ui->radioButtonPC, &QRadioButton::clicked, this, ClearFilterFormulaInputs);

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
    const QString desc = QStringLiteral("%1(%2 %3) - %4").arg(name).arg(value).arg(unit, description);
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
    ui->labelDescription->setText(QString());

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
            QFont font = item->font();
            font.setBold(true);
            item->setFont(font);
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
    ui->labelDescription->setText(QString());

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
            QFont fontName = itemName->font();
            fontName.setBold(true);
            itemName->setFont(fontName);
            itemName->setToolTip(itemName->text());

            QTableWidgetItem *itemFullName = new QTableWidgetItem();
            QFont fontFullName = itemName->font();
            fontFullName.setBold(true);
            itemFullName->setFont(fontFullName);
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
    ui->labelDescription->setText(QString());

    const QMap<QString, qmu::QmuTranslation> functionsDescriptions = qApp->TrVars()->GetFunctionsDescriptions();
    const QMap<QString, qmu::QmuTranslation> functions = qApp->TrVars()->GetFunctions();
    QMap<QString, qmu::QmuTranslation>::const_iterator i = functions.constBegin();
    while (i != functions.constEnd())
    {
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
        QTableWidgetItem *item = new QTableWidgetItem(i.value().translate(qApp->Settings()->GetLocale()));
        QFont font = item->font();
        font.setBold(true);
        item->setFont(font);
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, ColumnName, item);

        functionsDescriptions.contains(i.key())
            ? item->setToolTip(functionsDescriptions.value(i.key()).translate(qApp->Settings()->GetLocale()))
            : item->setToolTip(i.value().getMdisambiguation());
        ++i;
    }

    ui->tableWidget->blockSignals(false);
    ui->tableWidget->selectRow(0);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditWrongFormula::ShowIncrementsInPreviewCalculation(bool show)
{
    ui->checkBoxHideEmpty->setEnabled(false);

    QMap<QString, QSharedPointer<VIncrement> > increments;

    const QMap<QString, QSharedPointer<VIncrement> > list = data->DataIncrements();
    QMap<QString, QSharedPointer<VIncrement> >::const_iterator i = list.constBegin();
    while (i != list.constEnd())
    {
        if(i.value()->IsPreviewCalculation() == show)
        {
            increments.insert(i.key(), i.value());
        }
        ++i;
    }

    ShowVariable(increments);
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
        const QList<QTableWidgetItem*> items = ui->tableWidget->findItems(filter, Qt::MatchContains);
        for (auto item : items)
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
