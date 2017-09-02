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
      formulaBaseHeightPC(0),
      search(),
      searchPC(),
      hasChanges(false),
      renameList()
{
    ui->setupUi(this);

    ui->lineEditName->setClearButtonEnabled(true);
    ui->lineEditFind->setClearButtonEnabled(true);
    ui->lineEditNamePC->setClearButtonEnabled(true);
    ui->lineEditFindPC->setClearButtonEnabled(true);

    ui->lineEditFind->installEventFilter(this);
    ui->lineEditFindPC->installEventFilter(this);

    search = QSharedPointer<VTableSearch>(new VTableSearch(ui->tableWidgetIncrement));
    searchPC = QSharedPointer<VTableSearch>(new VTableSearch(ui->tableWidgetPC));

    formulaBaseHeight = ui->plainTextEditFormula->height();
    ui->plainTextEditFormula->installEventFilter(this);
    formulaBaseHeightPC = ui->plainTextEditFormulaPC->height();
    ui->plainTextEditFormulaPC->installEventFilter(this);

    qApp->Settings()->GetOsSeparator() ? setLocale(QLocale()) : setLocale(QLocale::c());

    qCDebug(vDialog, "Showing variables.");
    ShowUnits();

    const bool freshCall = true;
    FillIncrements(freshCall);
    FillPreviewCalculations(freshCall);
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
    ui->lineEditNamePC->setValidator( new QRegularExpressionValidator(QRegularExpression(
                                                                        QLatin1String("^$|")+NameRegExp()), this));

    connect(ui->tableWidgetIncrement, &QTableWidget::itemSelectionChanged, this,
            &DialogIncrements::ShowIncrementDetails);
    connect(ui->tableWidgetPC, &QTableWidget::itemSelectionChanged, this,
            &DialogIncrements::ShowIncrementDetails);

    connect(ui->toolButtonAdd, &QToolButton::clicked, this, &DialogIncrements::AddIncrement);
    connect(ui->toolButtonAddPC, &QToolButton::clicked, this, &DialogIncrements::AddIncrement);
    connect(ui->toolButtonRemove, &QToolButton::clicked, this, &DialogIncrements::RemoveIncrement);
    connect(ui->toolButtonRemovePC, &QToolButton::clicked, this, &DialogIncrements::RemoveIncrement);
    connect(ui->toolButtonUp, &QToolButton::clicked, this, &DialogIncrements::MoveUp);
    connect(ui->toolButtonUpPC, &QToolButton::clicked, this, &DialogIncrements::MoveUp);
    connect(ui->toolButtonDown, &QToolButton::clicked, this, &DialogIncrements::MoveDown);
    connect(ui->toolButtonDownPC, &QToolButton::clicked, this, &DialogIncrements::MoveDown);
    connect(ui->pushButtonGrow, &QPushButton::clicked, this, &DialogIncrements::DeployFormula);
    connect(ui->pushButtonGrowPC, &QPushButton::clicked, this, &DialogIncrements::DeployFormula);
    connect(ui->toolButtonExpr, &QToolButton::clicked, this, &DialogIncrements::Fx);
    connect(ui->toolButtonExprPC, &QToolButton::clicked, this, &DialogIncrements::Fx);
    connect(ui->lineEditName, &QLineEdit::textEdited, this, &DialogIncrements::SaveIncrName);
    connect(ui->lineEditNamePC, &QLineEdit::textEdited, this, &DialogIncrements::SaveIncrName);
    connect(ui->plainTextEditDescription, &QPlainTextEdit::textChanged, this, &DialogIncrements::SaveIncrDescription);
    connect(ui->plainTextEditDescriptionPC, &QPlainTextEdit::textChanged, this, &DialogIncrements::SaveIncrDescription);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogIncrements::SaveIncrFormula);
    connect(ui->plainTextEditFormulaPC, &QPlainTextEdit::textChanged, this, &DialogIncrements::SaveIncrFormula);
    connect(ui->lineEditFind, &QLineEdit::textEdited, this, [this](const QString &term){search->Find(term);});
    connect(ui->lineEditFindPC, &QLineEdit::textEdited, this, [this](const QString &term){searchPC->Find(term);});
    connect(ui->toolButtonFindPrevious, &QToolButton::clicked, this, [this](){search->FindPrevious();});
    connect(ui->toolButtonFindPreviousPC, &QToolButton::clicked, this, [this](){searchPC->FindPrevious();});
    connect(ui->toolButtonFindNext, &QToolButton::clicked, this, [this](){search->FindNext();});
    connect(ui->toolButtonFindNextPC, &QToolButton::clicked, this, [this](){searchPC->FindNext();});
    connect(ui->pushButtonRefresh, &QPushButton::clicked, this, &DialogIncrements::RefreshPattern);
    connect(ui->pushButtonRefreshPC, &QPushButton::clicked, this, &DialogIncrements::RefreshPattern);

    connect(search.data(), &VTableSearch::HasResult, this, [this] (bool state)
    {
        ui->toolButtonFindPrevious->setEnabled(state);
    });

    connect(searchPC.data(), &VTableSearch::HasResult, this, [this] (bool state)
    {
        ui->toolButtonFindPreviousPC->setEnabled(state);
    });

    connect(search.data(), &VTableSearch::HasResult, this, [this] (bool state)
    {
        ui->toolButtonFindNext->setEnabled(state);
    });

    connect(searchPC.data(), &VTableSearch::HasResult, this, [this] (bool state)
    {
        ui->toolButtonFindNextPC->setEnabled(state);
    });

    if (ui->tableWidgetIncrement->rowCount() > 0)
    {
        ui->tableWidgetIncrement->selectRow(0);
    }

    if (ui->tableWidgetPC->rowCount() > 0)
    {
        ui->tableWidgetPC->selectRow(0);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillIncrementTable fill data for increment table
 */
void DialogIncrements::FillIncrements(bool freshCall)
{
    FillIncrementsTable(ui->tableWidgetIncrement, data->DataIncrements(), false, freshCall);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::FillPreviewCalculations(bool freshCall)
{
    FillIncrementsTable(ui->tableWidgetPC, data->DataIncrements(), true, freshCall);
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
    ShowHeaderUnits(ui->tableWidgetPC, 1, unit);// calculated value
    ShowHeaderUnits(ui->tableWidgetPC, 2, unit);// formula

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
void DialogIncrements::Controls(QTableWidget *table)
{
    SCASSERT(table != nullptr)

    if (table->rowCount() > 0)
    {
        const QTableWidgetItem *nameField = table->item(table->currentRow(), 0);
        SCASSERT(nameField != nullptr)

        if (table == ui->tableWidgetIncrement)
        {
            ui->toolButtonRemove->setEnabled(not IncrementUsed(nameField->text()));
        }
        else if (table == ui->tableWidgetPC)
        {
            ui->toolButtonRemovePC->setEnabled(not IncrementUsed(nameField->text()));
        }
    }
    else
    {
        if (table == ui->tableWidgetIncrement)
        {
            ui->toolButtonRemove->setEnabled(false);
        }
        else if (table == ui->tableWidgetPC)
        {
            ui->toolButtonRemovePC->setEnabled(false);
        }
    }

    if (table->rowCount() >= 2)
    {
        if (table->currentRow() == 0)
        {
            if (table == ui->tableWidgetIncrement)
            {
                ui->toolButtonUp->setEnabled(false);
                ui->toolButtonDown->setEnabled(true);
            }
            else if (table == ui->tableWidgetPC)
            {
                ui->toolButtonUpPC->setEnabled(false);
                ui->toolButtonDownPC->setEnabled(true);
            }
        }
        else if (table->currentRow() == table->rowCount()-1)
        {
            if (table == ui->tableWidgetIncrement)
            {
                ui->toolButtonUp->setEnabled(true);
                ui->toolButtonDown->setEnabled(false);
            }
            else if (table == ui->tableWidgetPC)
            {
                ui->toolButtonUpPC->setEnabled(true);
                ui->toolButtonDownPC->setEnabled(false);
            }
        }
        else
        {
            if (table == ui->tableWidgetIncrement)
            {
                ui->toolButtonUp->setEnabled(true);
                ui->toolButtonDown->setEnabled(true);
            }
            else if (table == ui->tableWidgetPC)
            {
                ui->toolButtonUpPC->setEnabled(true);
                ui->toolButtonDownPC->setEnabled(true);
            }
        }
    }
    else
    {
        if (table == ui->tableWidgetIncrement)
        {
            ui->toolButtonUp->setEnabled(false);
            ui->toolButtonDown->setEnabled(false);
        }
        else if (table == ui->tableWidgetPC)
        {
            ui->toolButtonUpPC->setEnabled(false);
            ui->toolButtonDownPC->setEnabled(false);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::EnableDetails(QTableWidget *table, bool enabled)
{
    SCASSERT(table != nullptr)

    if (enabled)
    {
        Controls(table);
    }
    else
    {
        if (table == ui->tableWidgetIncrement)
        {
            ui->toolButtonRemove->setEnabled(enabled);
            ui->toolButtonUp->setEnabled(enabled);
            ui->toolButtonDown->setEnabled(enabled);
        }
        else if (table == ui->tableWidgetPC)
        {
            ui->toolButtonRemovePC->setEnabled(enabled);
            ui->toolButtonUpPC->setEnabled(enabled);
            ui->toolButtonDownPC->setEnabled(enabled);
        }
    }

    if (not enabled)
    { // Clear
        if (table == ui->tableWidgetIncrement)
        {
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
        else if (table == ui->tableWidgetPC)
        {
            ui->lineEditNamePC->blockSignals(true);
            ui->lineEditNamePC->clear();
            ui->lineEditNamePC->blockSignals(false);

            ui->plainTextEditDescriptionPC->blockSignals(true);
            ui->plainTextEditDescriptionPC->clear();
            ui->plainTextEditDescriptionPC->blockSignals(false);

            ui->labelCalculatedValuePC->blockSignals(true);
            ui->labelCalculatedValuePC->clear();
            ui->labelCalculatedValuePC->blockSignals(false);

            ui->plainTextEditFormulaPC->blockSignals(true);
            ui->plainTextEditFormulaPC->clear();
            ui->plainTextEditFormulaPC->blockSignals(false);
        }
    }

    if (table == ui->tableWidgetIncrement)
    {
        ui->pushButtonGrow->setEnabled(enabled);
        ui->toolButtonExpr->setEnabled(enabled);
        ui->lineEditName->setEnabled(enabled);
        ui->plainTextEditDescription->setEnabled(enabled);
        ui->plainTextEditFormula->setEnabled(enabled);
    }
    else if (table == ui->tableWidgetPC)
    {
        ui->pushButtonGrowPC->setEnabled(enabled);
        ui->toolButtonExprPC->setEnabled(enabled);
        ui->lineEditNamePC->setEnabled(enabled);
        ui->plainTextEditDescriptionPC->setEnabled(enabled);
        ui->plainTextEditFormulaPC->setEnabled(enabled);
    }
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
    int row = ui->tableWidgetIncrement->currentRow();
    FillIncrements();
    ui->tableWidgetIncrement->selectRow(row);

    row = ui->tableWidgetPC->currentRow();
    FillPreviewCalculations();
    ui->tableWidgetPC->selectRow(row);

    search->RefreshList(ui->lineEditFind->text());
    searchPC->RefreshList(ui->lineEditFindPC->text());
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
    FillPreviewCalculations();
    FillLengthsLines();
    FillLengthLinesAngles();
    FillLengthsCurves();
    FillCurvesCLengths();
    FillRadiusesArcs();
    FillAnglesCurves();

    search->RefreshList(ui->lineEditFind->text());
    searchPC->RefreshList(ui->lineEditFindPC->text());
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
        const int rowPC = ui->tableWidgetPC->currentRow();

        doc->LiteParseTree(Document::LiteParse);

        ui->tableWidgetIncrement->blockSignals(true);
        ui->tableWidgetIncrement->selectRow(row);
        ui->tableWidgetIncrement->blockSignals(false);

        ui->tableWidgetPC->blockSignals(true);
        ui->tableWidgetPC->selectRow(rowPC);
        ui->tableWidgetPC->blockSignals(false);

        hasChanges = false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::FillIncrementsTable(QTableWidget *table,
                                           const QMap<QString, QSharedPointer<VIncrement> > &increments,
                                           bool takePreviewCalculations, bool freshCall)
{
    SCASSERT(table != nullptr)

    table->blockSignals(true);
    table->clearContents();

    QMap<QString, QSharedPointer<VIncrement> >::const_iterator i;
    QMap<quint32, QString> map;
    //Sorting QHash by id
    for (i = increments.constBegin(); i != increments.constEnd(); ++i)
    {
        QSharedPointer<VIncrement> incr = i.value();
        if (takePreviewCalculations == incr->IsPreviewCalculation())
        {
            map.insert(incr->getIndex(), i.key());
        }
    }

    qint32 currentRow = -1;
    QMapIterator<quint32, QString> iMap(map);
    table->setRowCount ( map.size() );
    while (iMap.hasNext())
    {
        iMap.next();
        QSharedPointer<VIncrement> incr = increments.value(iMap.value());
        currentRow++;

        AddCell(table, incr->GetName(), currentRow, 0, Qt::AlignVCenter); // name
        AddCell(table, qApp->LocaleToString(*incr->GetValue()), currentRow, 1,
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

        AddCell(table, formula, currentRow, 2, Qt::AlignVCenter); // formula
    }

    if (freshCall)
    {
        table->resizeColumnsToContents();
        table->resizeRowsToContents();
    }
    table->horizontalHeader()->setStretchLastSection(true);
    table->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief clickedToolButtonAdd create new row in table
 */
void DialogIncrements::AddIncrement()
{
    qCDebug(vDialog, "Add new increment");

    QToolButton *button = qobject_cast<QToolButton *>(sender());
    QTableWidget *table = nullptr;

    if (button == ui->toolButtonAdd)
    {
        table = ui->tableWidgetIncrement;
    }
    else if (button == ui->toolButtonAddPC)
    {
        table = ui->tableWidgetPC;
    }
    else
    {
        return;
    }

    const QString name = GetCustomName();
    qint32 currentRow = -1;

    if (table->currentRow() == -1)
    {
        currentRow = table->rowCount();

        if (button == ui->toolButtonAdd)
        {
            doc->AddEmptyIncrement(name);
        }
        else if (button == ui->toolButtonAddPC)
        {
            doc->AddEmptyPreviewCalculation(name);
        }
    }
    else
    {
        currentRow  = table->currentRow()+1;
        const QTableWidgetItem *nameField = table->item(table->currentRow(), 0);

        if (button == ui->toolButtonAdd)
        {
            doc->AddEmptyIncrementAfter(nameField->text(), name);
        }
        else if (button == ui->toolButtonAddPC)
        {
            doc->AddEmptyPreviewCalculationAfter(nameField->text(), name);
        }
    }

    hasChanges = true;
    LocalUpdateTree();

    table->selectRow(currentRow);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief clickedToolButtonRemove remove one row from table
 */
void DialogIncrements::RemoveIncrement()
{
    QToolButton *button = qobject_cast<QToolButton *>(sender());

    QTableWidget *table = nullptr;

    if (button == ui->toolButtonRemove)
    {
        table = ui->tableWidgetIncrement;
    }
    else if (button == ui->toolButtonRemovePC)
    {
        table = ui->tableWidgetPC;
    }
    else
    {
        return;
    }

    const int row = table->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = table->item(row, 0);

    if (button == ui->toolButtonRemove)
    {
        doc->RemoveIncrement(nameField->text());
    }
    else if (button == ui->toolButtonRemovePC)
    {
        doc->RemovePreviewCalculation(nameField->text());
    }

    hasChanges = true;
    LocalUpdateTree();

    if (table->rowCount() > 0)
    {
        table->selectRow(0);
    }
    else
    {
        EnableDetails(table, false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::MoveUp()
{
    QToolButton *button = qobject_cast<QToolButton *>(sender());

    QTableWidget *table = nullptr;

    if (button == ui->toolButtonUp)
    {
        table = ui->tableWidgetIncrement;
    }
    else if (button == ui->toolButtonUpPC)
    {
        table = ui->tableWidgetPC;
    }
    else
    {
        return;
    }

    const int row = table->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = table->item(row, 0);

    if (button == ui->toolButtonUp)
    {
        doc->MoveUpIncrement(nameField->text());
    }
    else if (button == ui->toolButtonUpPC)
    {
        doc->MoveUpPreviewCalculation(nameField->text());
    }

    hasChanges = true;
    LocalUpdateTree();

    table->selectRow(row-1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::MoveDown()
{
    QToolButton *button = qobject_cast<QToolButton *>(sender());

    QTableWidget *table = nullptr;

    if (button == ui->toolButtonDown)
    {
        table = ui->tableWidgetIncrement;
    }
    else if (button == ui->toolButtonDownPC)
    {
        table = ui->tableWidgetPC;
    }
    else
    {
        return;
    }

    const int row = table->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = table->item(row, 0);

    if (button == ui->toolButtonDown)
    {
        doc->MoveDownIncrement(nameField->text());
    }
    else if (button == ui->toolButtonDownPC)
    {
        doc->MoveDownPreviewCalculation(nameField->text());
    }

    hasChanges = true;
    LocalUpdateTree();

    table->selectRow(row+1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::SaveIncrName(const QString &text)
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(sender());

    QTableWidget *table = nullptr;

    if (lineEdit == ui->lineEditName)
    {
        table = ui->tableWidgetIncrement;
    }
    else if (lineEdit == ui->lineEditNamePC)
    {
        table = ui->tableWidgetPC;
    }
    else
    {
        return;
    }

    const int row = table->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = table->item(row, 0);

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
    QVector<VFormulaField> expression = doc->ListIncrementExpressions();
    doc->ReplaceNameInFormula(expression, nameField->text(), newName);

    CacheRename(nameField->text(), newName);

    hasChanges = true;
    LocalUpdateTree();

    table->blockSignals(true);
    table->selectRow(row);
    table->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::SaveIncrDescription()
{
    QPlainTextEdit *textEdit = qobject_cast<QPlainTextEdit *>(sender());

    QTableWidget *table = nullptr;

    if (textEdit == ui->plainTextEditDescription)
    {
        table = ui->tableWidgetIncrement;
    }
    else if (textEdit == ui->plainTextEditDescriptionPC)
    {
        table = ui->tableWidgetPC;
    }
    else
    {
        return;
    }

    const int row = table->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = table->item(row, 0);
    doc->SetIncrementDescription(nameField->text(), textEdit->toPlainText());
    LocalUpdateTree();

    const QTextCursor cursor = textEdit->textCursor();
    table->blockSignals(true);
    table->selectRow(row);
    table->blockSignals(false);
    textEdit->setTextCursor(cursor);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::SaveIncrFormula()
{
    QPlainTextEdit *textEdit = qobject_cast<QPlainTextEdit *>(sender());

    QTableWidget *table = nullptr;
    QLabel *labelCalculatedValue = nullptr;

    if (textEdit == ui->plainTextEditFormula)
    {
        table = ui->tableWidgetIncrement;
        labelCalculatedValue = ui->labelCalculatedValue;
    }
    else if (textEdit == ui->plainTextEditFormulaPC)
    {
        table = ui->tableWidgetPC;
        labelCalculatedValue = ui->labelCalculatedValuePC;
    }
    else
    {
        return;
    }

    const int row = table->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = table->item(row, 0);

    // Replace line return character with spaces for calc if exist
    QString text = textEdit->toPlainText();
    text.replace("\n", " ");

    QTableWidgetItem *formulaField = table->item(row, 2);
    if (formulaField->text() == text)
    {
        QTableWidgetItem *result = table->item(row, 1);
        //Show unit in dialog lable (cm, mm or inch)
        const QString postfix = UnitsToStr(qApp->patternUnit());
        labelCalculatedValue->setText(result->text() + " " +postfix);
        return;
    }

    if (text.isEmpty())
    {
        //Show unit in dialog lable (cm, mm or inch)
        const QString postfix = UnitsToStr(qApp->patternUnit());
        labelCalculatedValue->setText(tr("Error") + " (" + postfix + "). " + tr("Empty field."));
        return;
    }

    QSharedPointer<VIncrement> incr = data->GetVariable<VIncrement>(nameField->text());
    if (not EvalIncrementFormula(text, true, incr->GetData(), labelCalculatedValue))
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

    const QTextCursor cursor = textEdit->textCursor();
    table->blockSignals(true);
    table->selectRow(row);
    table->blockSignals(false);
    textEdit->setTextCursor(cursor);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::DeployFormula()
{
    SCASSERT(ui->plainTextEditFormula != nullptr)
    SCASSERT(ui->pushButtonGrow != nullptr)
    SCASSERT(ui->plainTextEditFormulaPC != nullptr)
    SCASSERT(ui->pushButtonGrowPC != nullptr)

    QPushButton *button = qobject_cast<QPushButton *>(sender());

    QPlainTextEdit *plainTextEditFormula = nullptr;
    QPushButton *pushButtonGrow = nullptr;
    int baseHeight = 0;

    if (button == ui->pushButtonGrow)
    {
        plainTextEditFormula = ui->plainTextEditFormula;
        pushButtonGrow = ui->pushButtonGrow;
        baseHeight = formulaBaseHeight;
    }
    else if (button == ui->pushButtonGrowPC)
    {
        plainTextEditFormula = ui->plainTextEditFormulaPC;
        pushButtonGrow = ui->pushButtonGrowPC;
        baseHeight = formulaBaseHeightPC;
    }
    else
    {
        return;
    }

    const QTextCursor cursor = plainTextEditFormula->textCursor();

    if (plainTextEditFormula->height() < DIALOG_MAX_FORMULA_HEIGHT)
    {
        plainTextEditFormula->setFixedHeight(DIALOG_MAX_FORMULA_HEIGHT);
        //Set icon from theme (internal for Windows system)
        pushButtonGrow->setIcon(QIcon::fromTheme("go-next",
                                                  QIcon(":/icons/win.icon.theme/16x16/actions/go-next.png")));
    }
    else
    {
       plainTextEditFormula->setFixedHeight(baseHeight);
       //Set icon from theme (internal for Windows system)
       pushButtonGrow->setIcon(QIcon::fromTheme("go-down",
                                                 QIcon(":/icons/win.icon.theme/16x16/actions/go-down.png")));
    }

    // I found that after change size of formula field, it was filed for angle formula, field for formula became black.
    // This code prevent this.
    setUpdatesEnabled(false);
    repaint();
    setUpdatesEnabled(true);

    plainTextEditFormula->setFocus();
    plainTextEditFormula->setTextCursor(cursor);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogIncrements::Fx()
{
    QToolButton *button = qobject_cast<QToolButton *>(sender());

    QTableWidget *table = nullptr;
    QPlainTextEdit *plainTextEditFormula = nullptr;

    if (button == ui->toolButtonExpr)
    {
        table = ui->tableWidgetIncrement;
        plainTextEditFormula = ui->plainTextEditFormula;
    }
    else if (button == ui->toolButtonExprPC)
    {
        table = ui->tableWidgetPC;
        plainTextEditFormula = ui->plainTextEditFormulaPC;
    }
    else
    {
        return;
    }

    const int row = table->currentRow();

    if (row == -1)
    {
        return;
    }

    const QTableWidgetItem *nameField = table->item(row, 0);
    QSharedPointer<VIncrement> incr = data->GetVariable<VIncrement>(nameField->text());

    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(incr->GetData(), NULL_ID, this);
    dialog->setWindowTitle(tr("Edit increment"));
    dialog->SetFormula(qApp->TrVars()->TryFormulaFromUser(plainTextEditFormula->toPlainText().replace("\n", " "),
                                                          qApp->Settings()->GetOsSeparator()));
    const QString postfix = UnitsToStr(qApp->patternUnit(), true);
    dialog->setPostfix(postfix);//Show unit in dialog lable (cm, mm or inch)

    if (dialog->exec() == QDialog::Accepted)
    {
        // Fix the bug #492. https://bitbucket.org/dismine/valentina/issues/492/valentina-crashes-when-add-an-increment
        // Because of the bug need to take QTableWidgetItem twice time. Previous update "killed" the pointer.
        const QTableWidgetItem *nameField = table->item(row, 0);
        doc->SetIncrementFormula(nameField->text(), dialog->GetFormula());

        hasChanges = true;
        LocalUpdateTree();

        table->selectRow(row);
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
    QTableWidget *table = qobject_cast<QTableWidget *>(sender());

    QLineEdit *lineEditName = nullptr;
    QPlainTextEdit *plainTextEditDescription = nullptr;
    QPlainTextEdit *plainTextEditFormula = nullptr;
    QLabel *labelCalculatedValue = nullptr;

    if (table == ui->tableWidgetIncrement)
    {
        lineEditName = ui->lineEditName;
        plainTextEditDescription = ui->plainTextEditDescription;
        plainTextEditFormula = ui->plainTextEditFormula;
        labelCalculatedValue = ui->labelCalculatedValue;
    }
    else if (table == ui->tableWidgetPC)
    {
        lineEditName = ui->lineEditNamePC;
        plainTextEditDescription = ui->plainTextEditDescriptionPC;
        plainTextEditFormula = ui->plainTextEditFormulaPC;
        labelCalculatedValue = ui->labelCalculatedValuePC;
    }
    else
    {
        return;
    }

    if (table->rowCount() > 0)
    {
        EnableDetails(table, true);

        // name
        const QTableWidgetItem *nameField = table->item(table->currentRow(), 0);
        QSharedPointer<VIncrement> incr;

        try
        {
            incr = data->GetVariable<VIncrement>(nameField->text());
        }
        catch(const VExceptionBadId &e)
        {
            Q_UNUSED(e)
            EnableDetails(table, false);
            return;
        }

        lineEditName->blockSignals(true);
        lineEditName->setText(ClearIncrementName(incr->GetName()));
        lineEditName->blockSignals(false);

        plainTextEditDescription->blockSignals(true);
        plainTextEditDescription->setPlainText(incr->GetDescription());
        plainTextEditDescription->blockSignals(false);

        EvalIncrementFormula(incr->GetFormula(), false, incr->GetData(), labelCalculatedValue);
        plainTextEditFormula->blockSignals(true);

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

        plainTextEditFormula->setPlainText(formula);
        plainTextEditFormula->blockSignals(false);
    }
    else
    {
        EnableDetails(table, false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
DialogIncrements::~DialogIncrements()
{
    delete ui;
}
