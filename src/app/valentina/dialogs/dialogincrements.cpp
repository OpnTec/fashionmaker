/************************************************************************
 **
 **  @file   dialogincrements.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#include "dialogincrements.h"
#include "ui_dialogincrements.h"
#include "../vwidgets/vwidgetpopup.h"
#include "../vmisc/vsettings.h"
#include "../qmuparser/qmudef.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vpatterndb/calculator.h"
#include "../vtools/dialogs/support/dialogeditwrongformula.h"

#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTableWidget>
#include <QSettings>
#include <QTableWidgetItem>

#define DIALOG_MAX_FORMULA_HEIGHT 64

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogIncrements create dialog
 * @param data container with data
 * @param doc dom document container
 * @param parent parent widget
 */
DialogIncrements::DialogIncrements(VContainer *data, VPattern *doc, QWidget *parent)
    :DialogTool(data, NULL_ID, parent),
      ui(new Ui::DialogIncrements),
      data(data),
      doc(doc),
      formulaBaseHeight(0)
{
    ui->setupUi(this);

#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    ui->lineEditName->setClearButtonEnabled(true);
#endif

    formulaBaseHeight = ui->plainTextEditFormula->height();

    qApp->Settings()->GetOsSeparator() ? setLocale(QLocale::system()) : setLocale(QLocale(QLocale::C));

    qCDebug(vDialog, "Showing variables.");
    ShowUnits();

    FillIncrements();
    FillLengthsLines();
    FillLengthLinesAngles();
    FillLengthsCurves();
    FillLengthArcs();
    FillRadiusesArcs();
    FillAnglesArcs();
    FillAnglesCurves();

    connect(this, &DialogIncrements::FullUpdateTree, this->doc, &VPattern::LiteParseTree);
    connect(this->doc, &VPattern::FullUpdateFromFile, this, &DialogIncrements::FullUpdateFromFile);

    ui->tabWidget->setCurrentIndex(0);
#if QT_VERSION > QT_VERSION_CHECK(5, 1, 0)
    ui->lineEditName->setValidator( new QRegularExpressionValidator(QRegularExpression(NameRegExp()), this));
#else
    ui->lineEditName->setValidator( new QRegExpValidator(QRegExp(NameRegExp()), this));
#endif

    connect(ui->tableWidgetIncrement, &QTableWidget::itemSelectionChanged, this,
            &DialogIncrements::ShowIncrementDetails);

    connect(ui->toolButtonAdd, &QPushButton::clicked, this, &DialogIncrements::AddIncrement);
    connect(ui->toolButtonRemove, &QToolButton::clicked, this, &DialogIncrements::RemoveIncrement);
    connect(ui->toolButtonUp, &QToolButton::clicked, this, &DialogIncrements::MoveUp);
    connect(ui->toolButtonDown, &QToolButton::clicked, this, &DialogIncrements::MoveDown);
    connect(ui->pushButtonGrow, &QPushButton::clicked, this, &DialogIncrements::DeployFormula);
    connect(ui->toolButtonExpr, &QToolButton::clicked, this, &DialogIncrements::Fx);
    connect(ui->lineEditName, &QLineEdit::editingFinished, this, &DialogIncrements::SaveIncrName);
    connect(ui->plainTextEditDescription, &QPlainTextEdit::textChanged, this, &DialogIncrements::SaveIncrDescription);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogIncrements::SaveIncrFormula);

    if (ui->tableWidgetIncrement->rowCount() > 0)
    {
        ui->tableWidgetIncrement->selectRow(0);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillIncrementTable fill data for increment table
 */
void DialogIncrements::FillIncrements()
{
    ui->tableWidgetIncrement->blockSignals(true);
    ui->tableWidgetIncrement->clearContents();

    const QMap<QString, QSharedPointer<VIncrement> > increments = data->DataIncrements();
    QMap<QString, QSharedPointer<VIncrement> >::const_iterator i;
    QMap<quint32, QString> map;
    //Sorting QHash by id
    for (i = increments.constBegin(); i != increments.constEnd(); ++i)
    {
        QSharedPointer<VIncrement> incr = i.value();
        map.insert(incr->getIndex(), i.key());
    }

    qint32 currentRow = -1;
    QMapIterator<quint32, QString> iMap(map);
    ui->tableWidgetIncrement->setRowCount ( increments.size() );
    while (iMap.hasNext())
    {
        iMap.next();
        QSharedPointer<VIncrement> incr = increments.value(iMap.value());
        currentRow++;

        AddCell(ui->tableWidgetIncrement, incr->GetName(), currentRow, 0, Qt::AlignVCenter); // name
        AddCell(ui->tableWidgetIncrement, qApp->LocaleToString(*incr->GetValue()), currentRow, 1,
                Qt::AlignHCenter | Qt::AlignVCenter, incr->IsFormulaOk()); // calculated value

        QString formula;
        try
        {
            formula = qApp->TrVars()->FormulaToUser(incr->GetFormula(), qApp->Settings()->GetOsSeparator());
        }
        catch (qmu::QmuParserError &e)
        {
            Q_UNUSED(e);
            formula = incr->GetFormula();
        }

        AddCell(ui->tableWidgetIncrement, formula, currentRow, 2, Qt::AlignVCenter); // formula
    }
    ui->tableWidgetIncrement->resizeColumnsToContents();
    ui->tableWidgetIncrement->resizeRowsToContents();
    ui->tableWidgetIncrement->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetIncrement->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void DialogIncrements::FillTable(const QMap<QString, T> &varTable, QTableWidget *table)
{
    SCASSERT(table != nullptr);

    qint32 currentRow = -1;
    QMapIterator<QString, T> i(varTable);
    while (i.hasNext())
    {
        i.next();
        qreal length = *i.value()->GetValue();
        currentRow++;
        table->setRowCount ( varTable.size() );

        QTableWidgetItem *item = new QTableWidgetItem(i.key());
        item->setTextAlignment(Qt::AlignLeft);
        item->setFont(QFont("Times", 12, QFont::Bold));
        table->setItem(currentRow, 0, item);

        item = new QTableWidgetItem(qApp->LocaleToString(length));
        item->setTextAlignment(Qt::AlignHCenter);
        table->setItem(currentRow, 1, item);
    }
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
    table->verticalHeader()->setDefaultSectionSize(20);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillLengthLines fill data for table of lines lengths
 */
void DialogIncrements::FillLengthsLines()
{
    FillTable(data->DataLengthLines(), ui->tableWidgetLines);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::FillLengthLinesAngles()
{
    FillTable(data->DataAngleLines(), ui->tableWidgetLinesAngles);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillLengthSplines fill data for table of splines lengths
 */
void DialogIncrements::FillLengthsCurves()
{
    FillTable(data->DataLengthSplines(), ui->tableWidgetSplines);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillLengthArcs fill data for table of arcs lengths
 */
void DialogIncrements::FillLengthArcs()
{
    FillTable(data->DataLengthArcs(), ui->tableWidgetArcs);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::FillRadiusesArcs()
{
    FillTable(data->DataRadiusesArcs(), ui->tableWidgetRadiusesArcs);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::FillAnglesArcs()
{
    FillTable(data->DataAnglesArcs(), ui->tableWidgetAnglesArcs);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::FillAnglesCurves()
{
    FillTable(data->DataAnglesCurves(), ui->tableWidgetAnglesCurves);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::ShowUnits()
{
    const QString unit = VDomDocument::UnitsToStr(qApp->patternUnit());

    ShowHeaderUnits(ui->tableWidgetIncrement, 1, unit);// calculated value
    ShowHeaderUnits(ui->tableWidgetIncrement, 2, unit);// formula

    ShowHeaderUnits(ui->tableWidgetLines, 1, unit);// lengths
    ShowHeaderUnits(ui->tableWidgetSplines, 1, unit);// lengths
    ShowHeaderUnits(ui->tableWidgetArcs, 1, unit);// lengths
    ShowHeaderUnits(ui->tableWidgetLinesAngles, 1, "°");// angle
    ShowHeaderUnits(ui->tableWidgetRadiusesArcs, 1, unit);// radius
    ShowHeaderUnits(ui->tableWidgetAnglesArcs, 1, "°");// angle
    ShowHeaderUnits(ui->tableWidgetAnglesCurves, 1, "°");// angle
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::ShowHeaderUnits(QTableWidget *table, int column, const QString &unit)
{
    SCASSERT(table != nullptr);

    const QString header = table->horizontalHeaderItem(column)->text();
    const QString unitHeader = QString("%1 (%2)").arg(header).arg(unit);
    table->horizontalHeaderItem(column)->setText(unitHeader);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::AddCell(QTableWidget *table, const QString &text, int row, int column, int aligment, bool ok)
{
    SCASSERT(table != nullptr);

    QTableWidgetItem *item = new QTableWidgetItem(text);
    item->setTextAlignment(aligment);

    // set the item non-editable (view only), and non-selectable
    Qt::ItemFlags flags = item->flags();
    flags &= ~(Qt::ItemIsEditable); // reset/clear the flag
    item->setFlags(flags);

    if (not ok)
    {
        QBrush brush = item->foreground();
        brush.setColor(Qt::red);
        item->setForeground(brush);
    }

    table->setItem(row, column, item);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogIncrements::ClearIncrementName(const QString &name) const
{
    QString clear = name;
    const int index = clear.indexOf(CustomIncrSign);
    if (index == 0)
    {
        clear.remove(0, 1);
    }
    return clear;
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogIncrements::EvalIncrementFormula(const QString &formula, bool fromUser, VContainer *data, QLabel *label)
{
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit());//Show unit in dialog lable (cm, mm or inch)
    if (formula.isEmpty())
    {
        label->setText(tr("Error") + " (" + postfix + "). " + tr("Empty field."));
        label->setToolTip(tr("Empty field"));
        return false;
    }
    else
    {
        try
        {
            QString f;
            // Replace line return character with spaces for calc if exist
            if (fromUser)
            {
                f = qApp->TrVars()->FormulaFromUser(formula, qApp->Settings()->GetOsSeparator());
            }
            else
            {
                f = formula;
            }
            f.replace("\n", " ");
            Calculator *cal = new Calculator();
            const qreal result = cal->EvalFormula(data->PlainVariables(), f);
            delete cal;

            label->setText(qApp->LocaleToString(result) + " " + postfix);
            label->setToolTip(tr("Value"));
            return true;
        }
        catch (qmu::QmuParserError &e)
        {
            label->setText(tr("Error") + " (" + postfix + "). " + tr("Parser error: %1").arg(e.GetMsg()));
            label->setToolTip(tr("Parser error: %1").arg(e.GetMsg()));
            return false;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::Controls()
{
    if (ui->tableWidgetIncrement->rowCount() > 0)
    {
        ui->toolButtonRemove->setEnabled(true);
    }
    else
    {
        ui->toolButtonRemove->setEnabled(false);
    }

    if (ui->tableWidgetIncrement->rowCount() >= 2)
    {
        if (ui->tableWidgetIncrement->currentRow() == 0)
        {
            ui->toolButtonUp->setEnabled(false);
            ui->toolButtonDown->setEnabled(true);
        }
        else if (ui->tableWidgetIncrement->currentRow() == ui->tableWidgetIncrement->rowCount()-1)
        {
            ui->toolButtonUp->setEnabled(true);
            ui->toolButtonDown->setEnabled(false);
        }
        else
        {
            ui->toolButtonUp->setEnabled(true);
            ui->toolButtonDown->setEnabled(true);
        }
    }
    else
    {
        ui->toolButtonUp->setEnabled(false);
        ui->toolButtonDown->setEnabled(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::EnableDetails(bool enabled)
{
    if (enabled)
    {
        Controls();
    }
    else
    {
        ui->toolButtonRemove->setEnabled(enabled);
        ui->toolButtonUp->setEnabled(enabled);
        ui->toolButtonDown->setEnabled(enabled);
    }

    if (not enabled)
    { // Clear
        ui->lineEditName->blockSignals(true);
        ui->lineEditName->clear();
        ui->lineEditName->blockSignals(false);

        ui->plainTextEditDescription->blockSignals(true);
        ui->plainTextEditDescription->clear();
        ui->plainTextEditDescription->blockSignals(false);

        ui->labelCalculatedValue->blockSignals(true);
        ui->labelCalculatedValue->clear();
        ui->labelCalculatedValue->blockSignals(false);

        ui->plainTextEditFormula->blockSignals(true);
        ui->plainTextEditFormula->clear();
        ui->plainTextEditFormula->blockSignals(false);
    }

    ui->pushButtonGrow->setEnabled(enabled);
    ui->toolButtonExpr->setEnabled(enabled);
    ui->lineEditName->setEnabled(enabled);
    ui->plainTextEditDescription->setEnabled(enabled);
    ui->plainTextEditFormula->setEnabled(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update information in tables form file
 */
void DialogIncrements::FullUpdateFromFile()
{
    ui->tableWidgetLines->clearContents();
    ui->tableWidgetSplines->clearContents();
    ui->tableWidgetArcs->clearContents();
    ui->tableWidgetAnglesArcs->clearContents();
    ui->tableWidgetAnglesCurves->clearContents();
    ui->tableWidgetLinesAngles->clearContents();
    ui->tableWidgetRadiusesArcs->clearContents();

    FillIncrements();
    FillLengthsLines();
    FillLengthLinesAngles();
    FillLengthsCurves();
    FillLengthArcs();
    FillRadiusesArcs();
    FillAnglesArcs();
    FillAnglesCurves();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief clickedToolButtonAdd create new row in table
 */
void DialogIncrements::AddIncrement()
{
    qCDebug(vDialog, "Add a new increment");
    qint32 num = 1;
    QString name;
    do
    {
        name = CustomIncrSign + tr("Increment_%1").arg(num);
        num++;
    } while (data->IsUnique(name)==false);

    qint32 currentRow;

    if (ui->tableWidgetIncrement->currentRow() == -1)
    {
        currentRow  = ui->tableWidgetIncrement->rowCount();
        doc->AddEmptyIncrement(name);
    }
    else
    {
        currentRow  = ui->tableWidgetIncrement->currentRow()+1;
        const QTableWidgetItem *nameField = ui->tableWidgetIncrement->item(ui->tableWidgetIncrement->currentRow(), 0);
        doc->AddEmptyIncrementAfter(nameField->text(), name);
    }

    FullUpdateTree(Document::LiteParse);
    ui->tableWidgetIncrement->selectRow(currentRow);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief clickedToolButtonRemove remove one row from table
 */
void DialogIncrements::RemoveIncrement()
{
    const int row = ui->tableWidgetIncrement->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = ui->tableWidgetIncrement->item(row, 0);
    doc->RemoveIncrement(nameField->text());

    FullUpdateTree(Document::LiteParse);

    if (ui->tableWidgetIncrement->rowCount() > 0)
    {
        ui->tableWidgetIncrement->selectRow(0);
    }
    else
    {
        EnableDetails(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::MoveUp()
{
    const int row = ui->tableWidgetIncrement->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = ui->tableWidgetIncrement->item(row, 0);
    doc->MoveUpIncrement(nameField->text());
    FullUpdateTree(Document::LiteParse);
    ui->tableWidgetIncrement->selectRow(row-1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::MoveDown()
{
    const int row = ui->tableWidgetIncrement->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = ui->tableWidgetIncrement->item(row, 0);
    doc->MoveDownIncrement(nameField->text());
    FullUpdateTree(Document::LiteParse);
    ui->tableWidgetIncrement->selectRow(row+1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::SaveIncrName()
{
    const int row = ui->tableWidgetIncrement->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = ui->tableWidgetIncrement->item(row, 0);
    const QString newName = CustomIncrSign + ui->lineEditName->text();
    if (data->IsUnique(newName))
    {
        doc->SetIncrementName(nameField->text(), newName);
        FullUpdateTree(Document::LiteParse);
        ui->tableWidgetIncrement->blockSignals(true);
        ui->tableWidgetIncrement->selectRow(row);
        ui->tableWidgetIncrement->blockSignals(false);
    }
    else
    {
        ui->lineEditName->setText(ClearIncrementName(nameField->text()));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::SaveIncrDescription()
{
    const int row = ui->tableWidgetIncrement->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = ui->tableWidgetIncrement->item(row, 0);
    doc->SetIncrementDescription(nameField->text(), ui->plainTextEditDescription->toPlainText());

    FullUpdateTree(Document::LiteParse);

    const QTextCursor cursor = ui->plainTextEditDescription->textCursor();
    ui->tableWidgetIncrement->blockSignals(true);
    ui->tableWidgetIncrement->selectRow(row);
    ui->tableWidgetIncrement->blockSignals(false);
    ui->plainTextEditDescription->setTextCursor(cursor);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::SaveIncrFormula()
{
    const int row = ui->tableWidgetIncrement->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = ui->tableWidgetIncrement->item(row, 0);

    // Replace line return character with spaces for calc if exist
    QString text = ui->plainTextEditFormula->toPlainText();
    text.replace("\n", " ");

    QTableWidgetItem *formulaField = ui->tableWidgetIncrement->item(row, 2);
    if (formulaField->text() == text)
    {
        QTableWidgetItem *result = ui->tableWidgetIncrement->item(row, 1);
        //Show unit in dialog lable (cm, mm or inch)
        const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit());
        ui->labelCalculatedValue->setText(result->text() + " " +postfix);
        return;
    }

    if (text.isEmpty())
    {
        //Show unit in dialog lable (cm, mm or inch)
        const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit());
        ui->labelCalculatedValue->setText(tr("Error") + " (" + postfix + "). " + tr("Empty field."));
        return;
    }

    QSharedPointer<VIncrement> incr = data->GetVariable<VIncrement>(nameField->text());
    const bool ok = EvalIncrementFormula(text, true, incr->GetData(), ui->labelCalculatedValue);

    if (not ok)
    {
        return;
    }

    try
    {
        const QString formula = qApp->TrVars()->FormulaFromUser(text, qApp->Settings()->GetOsSeparator());
        doc->SetIncrementFormula(nameField->text(), formula);
    }
    catch (qmu::QmuParserError &e) // Just in case something bad will happen
    {
        Q_UNUSED(e)
        return;
    }

    FullUpdateTree(Document::LiteParse);

    const QTextCursor cursor = ui->plainTextEditFormula->textCursor();
    ui->tableWidgetIncrement->blockSignals(true);
    ui->tableWidgetIncrement->selectRow(row);
    ui->tableWidgetIncrement->blockSignals(false);
    ui->plainTextEditFormula->setTextCursor(cursor);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::DeployFormula()
{
    SCASSERT(ui->plainTextEditFormula != nullptr);
    SCASSERT(ui->pushButtonGrow != nullptr);

    const QTextCursor cursor = ui->plainTextEditFormula->textCursor();

    if (ui->plainTextEditFormula->height() < DIALOG_MAX_FORMULA_HEIGHT)
    {
        ui->plainTextEditFormula->setFixedHeight(DIALOG_MAX_FORMULA_HEIGHT);
        //Set icon from theme (internal for Windows system)
        ui->pushButtonGrow->setIcon(QIcon::fromTheme("go-next",
                                                     QIcon(":/icons/win.icon.theme/16x16/actions/go-next.png")));
    }
    else
    {
       ui->plainTextEditFormula->setFixedHeight(formulaBaseHeight);
       //Set icon from theme (internal for Windows system)
       ui->pushButtonGrow->setIcon(QIcon::fromTheme("go-down",
                                                    QIcon(":/icons/win.icon.theme/16x16/actions/go-down.png")));
    }

    // I found that after change size of formula field, it was filed for angle formula, field for formula became black.
    // This code prevent this.
    setUpdatesEnabled(false);
    repaint();
    setUpdatesEnabled(true);

    ui->plainTextEditFormula->setFocus();
    ui->plainTextEditFormula->setTextCursor(cursor);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::Fx()
{
    const int row = ui->tableWidgetIncrement->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = ui->tableWidgetIncrement->item(row, 0);
    QSharedPointer<VIncrement> incr = data->GetVariable<VIncrement>(nameField->text());

    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(incr->GetData(), NULL_ID, this);
    dialog->setWindowTitle(tr("Edit increment"));
    dialog->SetFormula(qApp->TrVars()->TryFormulaFromUser(ui->plainTextEditFormula->toPlainText().replace("\n", " "),
                                                          qApp->Settings()->GetOsSeparator()));
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    dialog->setPostfix(postfix);//Show unit in dialog lable (cm, mm or inch)

    if (dialog->exec() == QDialog::Accepted)
    {
        doc->SetIncrementFormula(nameField->text(), dialog->GetFormula());
        FullUpdateTree(Document::LiteParse);
        ui->tableWidgetIncrement->selectRow(row);
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    ui->lineEditName->blockSignals(true);
    ui->plainTextEditDescription->blockSignals(true);

    emit DialogClosed(QDialog::Accepted);
    event->accept();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        // retranslate designer form (single inheritance approach)
        ui->retranslateUi(this);
        FullUpdateFromFile();
    }
    // remember to call base class implementation
   QWidget::changeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::ShowIncrementDetails()
{
    if (ui->tableWidgetIncrement->rowCount() > 0)
    {
        EnableDetails(true);

        // name
        const QTableWidgetItem *nameField = ui->tableWidgetIncrement->item(ui->tableWidgetIncrement->currentRow(), 0);
        QSharedPointer<VIncrement> incr;

        try
        {
            incr = data->GetVariable<VIncrement>(nameField->text());
        }
        catch(const VExceptionBadId &e)
        {
            Q_UNUSED(e);
            EnableDetails(false);
            return;
        }

        ui->lineEditName->blockSignals(true);
        ui->lineEditName->setText(ClearIncrementName(incr->GetName()));
        ui->lineEditName->blockSignals(false);

        ui->plainTextEditDescription->blockSignals(true);
        ui->plainTextEditDescription->setPlainText(incr->GetDescription());
        ui->plainTextEditDescription->blockSignals(false);

        EvalIncrementFormula(incr->GetFormula(), false, incr->GetData(), ui->labelCalculatedValue);
        ui->plainTextEditFormula->blockSignals(true);

        QString formula;
        try
        {
            formula = qApp->TrVars()->FormulaToUser(incr->GetFormula(), qApp->Settings()->GetOsSeparator());
        }
        catch (qmu::QmuParserError &e)
        {
            Q_UNUSED(e);
            formula = incr->GetFormula();
        }

        ui->plainTextEditFormula->setPlainText(formula);
        ui->plainTextEditFormula->blockSignals(false);
    }
    else
    {
        EnableDetails(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
DialogIncrements::~DialogIncrements()
{
    delete ui;
}
