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
#include "../qmuparser/qmutokenparser.h"
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
#include <QtNumeric>

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
      formulaBaseHeight(0),
      search(),
      hasChanges(false),
      renameList()
{
    ui->setupUi(this);

    ui->lineEditName->setClearButtonEnabled(true);
    ui->lineEditFind->setClearButtonEnabled(true);

    ui->lineEditFind->installEventFilter(this);

    search = QSharedPointer<VTableSearch>(new VTableSearch(ui->tableWidgetIncrement));

    formulaBaseHeight = ui->plainTextEditFormula->height();
    ui->plainTextEditFormula->installEventFilter(this);

    qApp->Settings()->GetOsSeparator() ? setLocale(QLocale()) : setLocale(QLocale::c());

    qCDebug(vDialog, "Showing variables.");
    ShowUnits();

    const bool freshCall = true;
    FillIncrements(freshCall);
    FillLengthsLines();
    FillLengthLinesAngles();
    FillLengthsCurves();
    FillCurvesCLengths();
    FillRadiusesArcs();
    FillAnglesCurves();

    connect(this->doc, &VPattern::FullUpdateFromFile, this, &DialogIncrements::FullUpdateFromFile);

    ui->tabWidget->setCurrentIndex(0);
    ui->lineEditName->setValidator( new QRegularExpressionValidator(QRegularExpression(
                                                                        QLatin1String("^$|")+NameRegExp()), this));

    connect(ui->tableWidgetIncrement, &QTableWidget::itemSelectionChanged, this,
            &DialogIncrements::ShowIncrementDetails);

    connect(ui->toolButtonAdd, &QPushButton::clicked, this, &DialogIncrements::AddIncrement);
    connect(ui->toolButtonRemove, &QToolButton::clicked, this, &DialogIncrements::RemoveIncrement);
    connect(ui->toolButtonUp, &QToolButton::clicked, this, &DialogIncrements::MoveUp);
    connect(ui->toolButtonDown, &QToolButton::clicked, this, &DialogIncrements::MoveDown);
    connect(ui->pushButtonGrow, &QPushButton::clicked, this, &DialogIncrements::DeployFormula);
    connect(ui->toolButtonExpr, &QToolButton::clicked, this, &DialogIncrements::Fx);
    connect(ui->lineEditName, &QLineEdit::textEdited, this, &DialogIncrements::SaveIncrName);
    connect(ui->plainTextEditDescription, &QPlainTextEdit::textChanged, this, &DialogIncrements::SaveIncrDescription);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogIncrements::SaveIncrFormula);
    connect(ui->lineEditFind, &QLineEdit::textEdited, this, [this](const QString &term){search->Find(term);});
    connect(ui->toolButtonFindPrevious, &QToolButton::clicked, this, [this](){search->FindPrevious();});
    connect(ui->toolButtonFindNext, &QToolButton::clicked, this, [this](){search->FindNext();});
    connect(ui->pushButtonRefresh, &QPushButton::clicked, this, &DialogIncrements::RefreshPattern);

    connect(search.data(), &VTableSearch::HasResult, this, [this] (bool state)
    {
        ui->toolButtonFindPrevious->setEnabled(state);
    });
    connect(search.data(), &VTableSearch::HasResult, this, [this] (bool state)
    {
        ui->toolButtonFindNext->setEnabled(state);
    });

    if (ui->tableWidgetIncrement->rowCount() > 0)
    {
        ui->tableWidgetIncrement->selectRow(0);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillIncrementTable fill data for increment table
 */
void DialogIncrements::FillIncrements(bool freshCall)
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
            Q_UNUSED(e)
            formula = incr->GetFormula();
        }

        AddCell(ui->tableWidgetIncrement, formula, currentRow, 2, Qt::AlignVCenter); // formula
    }

    if (freshCall)
    {
        ui->tableWidgetIncrement->resizeColumnsToContents();
        ui->tableWidgetIncrement->resizeRowsToContents();
    }
    ui->tableWidgetIncrement->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetIncrement->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void DialogIncrements::FillTable(const QMap<QString, T> &varTable, QTableWidget *table)
{
    SCASSERT(table != nullptr)

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
    FillTable(data->DataLengthCurves(), ui->tableWidgetSplines);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::FillCurvesCLengths()
{
    FillTable(data->DataCurvesCLength(), ui->tableWidgetCLength);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::FillRadiusesArcs()
{
    FillTable(data->DataRadiusesArcs(), ui->tableWidgetRadiusesArcs);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::FillAnglesCurves()
{
    FillTable(data->DataAnglesCurves(), ui->tableWidgetAnglesCurves);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::ShowUnits()
{
    const QString unit = UnitsToStr(qApp->patternUnit());

    ShowHeaderUnits(ui->tableWidgetIncrement, 1, unit);// calculated value
    ShowHeaderUnits(ui->tableWidgetIncrement, 2, unit);// formula

    ShowHeaderUnits(ui->tableWidgetLines, 1, unit);// lengths
    ShowHeaderUnits(ui->tableWidgetSplines, 1, unit);// lengths
    ShowHeaderUnits(ui->tableWidgetCLength, 1, unit);// lengths
    ShowHeaderUnits(ui->tableWidgetLinesAngles, 1, degreeSymbol);// angle
    ShowHeaderUnits(ui->tableWidgetRadiusesArcs, 1, unit);// radius
    ShowHeaderUnits(ui->tableWidgetAnglesCurves, 1, degreeSymbol);// angle
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::ShowHeaderUnits(QTableWidget *table, int column, const QString &unit)
{
    SCASSERT(table != nullptr)

    const QString header = table->horizontalHeaderItem(column)->text();
    const QString unitHeader = QString("%1 (%2)").arg(header).arg(unit);
    table->horizontalHeaderItem(column)->setText(unitHeader);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::AddCell(QTableWidget *table, const QString &text, int row, int column, int aligment, bool ok)
{
    SCASSERT(table != nullptr)

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
QString DialogIncrements::GetCustomName() const
{
    qint32 num = 1;
    QString name;
    do
    {
        name = CustomIncrSign + qApp->TrVars()->InternalVarToUser(increment_) + QString().number(num);
        num++;
    } while (not data->IsUnique(name));
    return name;
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
    const QString postfix = UnitsToStr(qApp->patternUnit());//Show unit in dialog lable (cm, mm or inch)
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
            QScopedPointer<Calculator> cal(new Calculator());
            const qreal result = cal->EvalFormula(data->DataVariables(), f);

            if (qIsInf(result) || qIsNaN(result))
            {
                label->setText(tr("Error") + " (" + postfix + ").");
                label->setToolTip(tr("Invalid result. Value is infinite or NaN. Please, check your calculations."));
                return false;
            }

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
        const QTableWidgetItem *nameField = ui->tableWidgetIncrement->item(ui->tableWidgetIncrement->currentRow(), 0);
        SCASSERT(nameField != nullptr)

        ui->toolButtonRemove->setEnabled(not IncrementUsed(nameField->text()));
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
void DialogIncrements::LocalUpdateTree()
{
    doc->LiteParseIncrements();

    UpdateTree();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::UpdateTree()
{
    FillIncrements();

    search->RefreshList(ui->lineEditFind->text());
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogIncrements::IncrementUsed(const QString &name) const
{
    const QVector<VFormulaField> expressions = doc->ListExpressions();

    for(int i = 0; i < expressions.size(); ++i)
    {
        if (expressions.at(i).expression.indexOf(name) != -1)
        {
            // Eval formula
            try
            {
                QScopedPointer<qmu::QmuTokenParser> cal(new qmu::QmuTokenParser(expressions.at(i).expression, false,
                                                                                false));

                // Tokens (variables, measurements)
                if (cal->GetTokens().values().contains(name))
                {
                    return true;
                }
            }
            catch (const qmu::QmuParserError &)
            {
                // Do nothing. Because we not sure if used. A formula is broken.
            }
        }
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::CacheRename(const QString &name, const QString &newName)
{
    for (int i = 0; i < renameList.size(); ++i)
    {
        if (renameList.at(i).second == name)
        {
            renameList[i].second = newName;
            return;
        }
    }

    renameList.append(qMakePair(name, newName));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update information in tables form file
 */
void DialogIncrements::FullUpdateFromFile()
{
    hasChanges = false;

    ui->tableWidgetLines->clearContents();
    ui->tableWidgetSplines->clearContents();
    ui->tableWidgetAnglesCurves->clearContents();
    ui->tableWidgetLinesAngles->clearContents();
    ui->tableWidgetRadiusesArcs->clearContents();

    FillIncrements();
    FillLengthsLines();
    FillLengthLinesAngles();
    FillLengthsCurves();
    FillCurvesCLengths();
    FillRadiusesArcs();
    FillAnglesCurves();

    search->RefreshList(ui->lineEditFind->text());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::RefreshPattern()
{
    if (hasChanges)
    {
        QVector<VFormulaField> expressions = doc->ListExpressions();
        for (int i = 0; i < renameList.size(); ++i)
        {
            doc->ReplaceNameInFormula(expressions, renameList.at(i).first, renameList.at(i).second);
        }
        renameList.clear();

        const int row = ui->tableWidgetIncrement->currentRow();

        doc->LiteParseTree(Document::LiteParse);

        ui->tableWidgetIncrement->blockSignals(true);
        ui->tableWidgetIncrement->selectRow(row);
        ui->tableWidgetIncrement->blockSignals(false);

        hasChanges = false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief clickedToolButtonAdd create new row in table
 */
void DialogIncrements::AddIncrement()
{
    qCDebug(vDialog, "Add a new increment");

    const QString name = GetCustomName();
    qint32 currentRow = -1;

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

    hasChanges = true;
    LocalUpdateTree();

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

    hasChanges = true;
    LocalUpdateTree();

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

    hasChanges = true;
    LocalUpdateTree();

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

    hasChanges = true;
    LocalUpdateTree();

    ui->tableWidgetIncrement->selectRow(row+1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::SaveIncrName(const QString &text)
{
    const int row = ui->tableWidgetIncrement->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = ui->tableWidgetIncrement->item(row, 0);

    QString newName = text.isEmpty() ? GetCustomName() : CustomIncrSign + text;

    if (not data->IsUnique(newName))
    {
        qint32 num = 2;
        QString name = newName;
        do
        {
            name = name + QLatin1String("_") + QString().number(num);
            num++;
        } while (not data->IsUnique(name));
        newName = name;
    }

    doc->SetIncrementName(nameField->text(), newName);
    QVector<VFormulaField> expressions = doc->ListIncrementExpressions();
    doc->ReplaceNameInFormula(expressions, nameField->text(), newName);
    CacheRename(nameField->text(), newName);

    hasChanges = true;
    LocalUpdateTree();

    ui->tableWidgetIncrement->blockSignals(true);
    ui->tableWidgetIncrement->selectRow(row);
    ui->tableWidgetIncrement->blockSignals(false);
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

    LocalUpdateTree();

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
        const QString postfix = UnitsToStr(qApp->patternUnit());
        ui->labelCalculatedValue->setText(result->text() + " " +postfix);
        return;
    }

    if (text.isEmpty())
    {
        //Show unit in dialog lable (cm, mm or inch)
        const QString postfix = UnitsToStr(qApp->patternUnit());
        ui->labelCalculatedValue->setText(tr("Error") + " (" + postfix + "). " + tr("Empty field."));
        return;
    }

    QSharedPointer<VIncrement> incr = data->GetVariable<VIncrement>(nameField->text());
    if (not EvalIncrementFormula(text, true, incr->GetData(), ui->labelCalculatedValue))
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

    hasChanges = true;
    LocalUpdateTree();

    const QTextCursor cursor = ui->plainTextEditFormula->textCursor();
    ui->tableWidgetIncrement->blockSignals(true);
    ui->tableWidgetIncrement->selectRow(row);
    ui->tableWidgetIncrement->blockSignals(false);
    ui->plainTextEditFormula->setTextCursor(cursor);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::DeployFormula()
{
    SCASSERT(ui->plainTextEditFormula != nullptr)
    SCASSERT(ui->pushButtonGrow != nullptr)

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
    const QString postfix = UnitsToStr(qApp->patternUnit(), true);
    dialog->setPostfix(postfix);//Show unit in dialog lable (cm, mm or inch)

    if (dialog->exec() == QDialog::Accepted)
    {
        // Fix the bug #492. https://bitbucket.org/dismine/valentina/issues/492/valentina-crashes-when-add-an-increment
        // Because of the bug need to take QTableWidgetItem twice time. Previous update "killed" the pointer.
        const QTableWidgetItem *nameField = ui->tableWidgetIncrement->item(row, 0);
        doc->SetIncrementFormula(nameField->text(), dialog->GetFormula());

        hasChanges = true;
        LocalUpdateTree();

        ui->tableWidgetIncrement->selectRow(row);
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::closeEvent(QCloseEvent *event)
{
    RefreshPattern();

    ui->plainTextEditFormula->blockSignals(true);
    ui->lineEditName->blockSignals(true);
    ui->plainTextEditDescription->blockSignals(true);

    emit UpdateProperties();
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
bool DialogIncrements::eventFilter(QObject *object, QEvent *event)
{
    if (QLineEdit *textEdit = qobject_cast<QLineEdit *>(object))
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if ((keyEvent->key() == Qt::Key_Period) && (keyEvent->modifiers() & Qt::KeypadModifier))
            {
                if (qApp->Settings()->GetOsSeparator())
                {
                    textEdit->insert(QLocale().decimalPoint());
                }
                else
                {
                    textEdit->insert(QLocale::c().decimalPoint());
                }
                return true;
            }
        }
    }
    else
    {
        // pass the event on to the parent class
        return DialogTool::eventFilter(object, event);
    }
    return false;// pass the event to the widget
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::showEvent(QShowEvent *event)
{
    // Skip DialogTool implementation
    QDialog::showEvent(event);
    if ( event->spontaneous() )
    {
        return;
    }

    if (isInitialized)
    {
        return;
    }
    // do your init stuff here

    const QSize sz = qApp->Settings()->GetIncrementsDialogSize();
    if (not sz.isEmpty())
    {
        resize(sz);
    }

    isInitialized = true;//first show windows are held
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::resizeEvent(QResizeEvent *event)
{
    // remember the size for the next time this dialog is opened, but only
    // if widget was already initialized, which rules out the resize at
    // dialog creating, which would
    if (isInitialized)
    {
        qApp->Settings()->SetIncrementsDialogSize(size());
    }
    DialogTool::resizeEvent(event);
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
            Q_UNUSED(e)
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
            Q_UNUSED(e)
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
