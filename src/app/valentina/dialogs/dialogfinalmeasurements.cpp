/************************************************************************
 **
 **  @file   dialogfinalmeasurements.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   26 9, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#include "dialogfinalmeasurements.h"
#include "ui_dialogfinalmeasurements.h"
#include "../vmisc/vsettings.h"
#include "../qmuparser/qmudef.h"
#include "../qmuparser/qmutokenparser.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vpatterndb/calculator.h"
#include "../vtools/dialogs/support/dialogeditwrongformula.h"

#define DIALOG_MAX_FORMULA_HEIGHT 64

namespace
{
#if QT_VERSION < QT_VERSION_CHECK(5, 6, 0)
template <typename T>
void Move(QVector<T> &vector, int from, int to)
{
    Q_ASSERT_X(from >= 0 && from < vector.size(), "QVector::move(int,int)", "'from' is out-of-range");
    Q_ASSERT_X(to >= 0 && to < vector.size(), "QVector::move(int,int)", "'to' is out-of-range");
    if (from == to) // don't detach when no-op
    {
        return;
    }
    T * const b = vector.begin();
    if (from < to)
    {
        std::rotate(b + from, b + from + 1, b + to + 1);
    }
    else
    {
        std::rotate(b + to, b + from, b + from + 1);
    }
}
#endif // QT_VERSION < QT_VERSION_CHECK(5, 6, 0)
}

//---------------------------------------------------------------------------------------------------------------------
DialogFinalMeasurements::DialogFinalMeasurements(VPattern *doc, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogFinalMeasurements),
      m_doc(doc),
      m_data(doc->GetCompleteData()),
      m_measurements(doc->GetFinalMeasurements()),
      m_search(),
      formulaBaseHeight(0),
      m_isInitialized(false)
{
    ui->setupUi(this);

    ui->lineEditName->setClearButtonEnabled(true);
    ui->lineEditFind->setClearButtonEnabled(true);

    ui->lineEditFind->installEventFilter(this);

    m_search = QSharedPointer<VTableSearch>(new VTableSearch(ui->tableWidget));

    formulaBaseHeight = ui->plainTextEditFormula->height();
    ui->plainTextEditFormula->installEventFilter(this);

    qApp->Settings()->GetOsSeparator() ? setLocale(QLocale()) : setLocale(QLocale::c());

    qCDebug(vDialog, "Showing variables.");
    ShowUnits();

    const bool freshCall = true;
    FillFinalMeasurements(freshCall);

    connect(m_doc, &VPattern::FullUpdateFromFile, this, &DialogFinalMeasurements::FullUpdateFromFile);

    ui->lineEditName->setValidator( new QRegularExpressionValidator(QRegularExpression(
                                                                        QLatin1String("^$|")+NameRegExp()), this));

    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged, this,
            &DialogFinalMeasurements::ShowFinalMeasurementDetails);

    connect(ui->toolButtonAdd, &QToolButton::clicked, this, &DialogFinalMeasurements::Add);
    connect(ui->toolButtonRemove, &QToolButton::clicked, this, &DialogFinalMeasurements::Remove);
    connect(ui->toolButtonUp, &QToolButton::clicked, this, &DialogFinalMeasurements::MoveUp);
    connect(ui->toolButtonDown, &QToolButton::clicked, this, &DialogFinalMeasurements::MoveDown);
    connect(ui->pushButtonGrow, &QPushButton::clicked, this, &DialogFinalMeasurements::DeployFormula);
    connect(ui->toolButtonExpr, &QToolButton::clicked, this, &DialogFinalMeasurements::Fx);
    connect(ui->lineEditName, &QLineEdit::textEdited, this, &DialogFinalMeasurements::SaveName);
    connect(ui->plainTextEditDescription, &QPlainTextEdit::textChanged, this,
            &DialogFinalMeasurements::SaveDescription);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogFinalMeasurements::SaveFormula);
    connect(ui->lineEditFind, &QLineEdit::textEdited, this, [this](const QString &term){m_search->Find(term);});
    connect(ui->toolButtonFindPrevious, &QToolButton::clicked, this, [this](){m_search->FindPrevious();});
    connect(ui->toolButtonFindNext, &QToolButton::clicked, this, [this](){m_search->FindNext();});

    connect(m_search.data(), &VTableSearch::HasResult, this, [this] (bool state)
    {
        ui->toolButtonFindPrevious->setEnabled(state);
    });

    connect(m_search.data(), &VTableSearch::HasResult, this, [this] (bool state)
    {
        ui->toolButtonFindNext->setEnabled(state);
    });

    if (ui->tableWidget->rowCount() > 0)
    {
        ui->tableWidget->selectRow(0);
    }
}

//---------------------------------------------------------------------------------------------------------------------
DialogFinalMeasurements::~DialogFinalMeasurements()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFinalMeasurements::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    ui->lineEditName->blockSignals(true);
    ui->plainTextEditDescription->blockSignals(true);

    QDialog::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFinalMeasurements::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        // retranslate designer form (single inheritance approach)
        ui->retranslateUi(this);
        FullUpdateFromFile();
    }
    // remember to call base class implementation
    QDialog::changeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogFinalMeasurements::eventFilter(QObject *object, QEvent *event)
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
        return QDialog::eventFilter(object, event);
    }
    return false;// pass the event to the widget
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFinalMeasurements::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    if ( event->spontaneous() )
    {
        return;
    }

    if (m_isInitialized)
    {
        return;
    }
    // do your init stuff here

    const QSize sz = qApp->Settings()->GetFinalMeasurementsDialogSize();
    if (not sz.isEmpty())
    {
        resize(sz);
    }

    m_isInitialized = true;//first show windows are held
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFinalMeasurements::resizeEvent(QResizeEvent *event)
{
    // remember the size for the next time this dialog is opened, but only
    // if widget was already initialized, which rules out the resize at
    // dialog creating, which would
    if (m_isInitialized)
    {
        qApp->Settings()->SetFinalMeasurementsDialogSize(size());
    }
    QDialog::resizeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFinalMeasurements::ShowFinalMeasurementDetails()
{
    if (ui->tableWidget->rowCount() > 0 && m_measurements.size() == ui->tableWidget->rowCount())
    {
        EnableDetails(true);

        const VFinalMeasurement &m = m_measurements.at(ui->tableWidget->currentRow());

        ui->lineEditName->blockSignals(true);
        ui->lineEditName->setText(m.name);
        ui->lineEditName->blockSignals(false);

        ui->plainTextEditDescription->blockSignals(true);
        ui->plainTextEditDescription->setPlainText(m.description);
        ui->plainTextEditDescription->blockSignals(false);

        EvalUserFormula(m.formula, false);
        ui->plainTextEditFormula->blockSignals(true);

        const QString formula = VTranslateVars::TryFormulaToUser(m.formula, qApp->Settings()->GetOsSeparator());

        ui->plainTextEditFormula->setPlainText(formula);
        ui->plainTextEditFormula->blockSignals(false);
    }
    else
    {
        EnableDetails(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFinalMeasurements::Add()
{
    const int currentRow = ui->tableWidget->currentRow()+1;

    VFinalMeasurement m;
    m.name = tr("measurement");
    m.formula = "0";

    m_measurements.append(m);

    UpdateTree();
    ui->tableWidget->selectRow(currentRow);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFinalMeasurements::Remove()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1 || row >= m_measurements.size())
    {
        return;
    }

    m_measurements.remove(row);

    UpdateTree();

    if (ui->tableWidget->rowCount() > 0)
    {
        ui->tableWidget->selectRow(0);
    }
    else
    {
        EnableDetails(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFinalMeasurements::MoveUp()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1 || row == 0 || row >= m_measurements.size())
    {
        return;
    }

#if QT_VERSION < QT_VERSION_CHECK(5, 6, 0)
    Move(m_measurements, row, row-1);
#else
    m_measurements.move(row, row-1);
#endif

    UpdateTree();

    ui->tableWidget->selectRow(row-1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFinalMeasurements::MoveDown()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1 || row == ui->tableWidget->rowCount()-1 || row >= m_measurements.size())
    {
        return;
    }

#if QT_VERSION < QT_VERSION_CHECK(5, 6, 0)
    Move(m_measurements, row, row+1);
#else
    m_measurements.move(row, row+1);
#endif

    UpdateTree();

    ui->tableWidget->selectRow(row+1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFinalMeasurements::SaveName(const QString &text)
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1 || row >= m_measurements.size())
    {
        return;
    }

    m_measurements[row].name = text.isEmpty() ? tr("measurement") : text;

    UpdateTree();

    ui->tableWidget->blockSignals(true);
    ui->tableWidget->selectRow(row);
    ui->tableWidget->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFinalMeasurements::SaveDescription()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1 || row >= m_measurements.size())
    {
        return;
    }

    const QTextCursor cursor = ui->plainTextEditDescription->textCursor();

    m_measurements[row].description = ui->plainTextEditDescription->toPlainText();

    UpdateTree();

    ui->tableWidget->blockSignals(true);
    ui->tableWidget->selectRow(row);
    ui->tableWidget->blockSignals(false);
    ui->plainTextEditDescription->setTextCursor(cursor);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFinalMeasurements::SaveFormula()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1 || row >= m_measurements.size())
    {
        return;
    }

    // Replace line return character with spaces for calc if exist
    QString text = ui->plainTextEditFormula->toPlainText();
    text.replace("\n", " ");

    QTableWidgetItem *formulaField = ui->tableWidget->item(row, 2);
    if (formulaField->text() == text)
    {
        QTableWidgetItem *result = ui->tableWidget->item(row, 1);
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

    if (not EvalUserFormula(text, true))
    {
        return;
    }

    try
    {
        m_measurements[row].formula = qApp->TrVars()->FormulaFromUser(text, qApp->Settings()->GetOsSeparator());
    }
    catch (qmu::QmuParserError &e) // Just in case something bad will happen
    {
        Q_UNUSED(e)
        return;
    }

    const QTextCursor cursor = ui->plainTextEditFormula->textCursor();

    UpdateTree();

    ui->tableWidget->blockSignals(true);
    ui->tableWidget->selectRow(row);
    ui->tableWidget->blockSignals(false);
    ui->plainTextEditFormula->setTextCursor(cursor);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFinalMeasurements::DeployFormula()
{
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
void DialogFinalMeasurements::Fx()
{
    const int row = ui->tableWidget->currentRow();

    if (row == -1 || row >= m_measurements.size())
    {
        return;
    }

    QScopedPointer<DialogEditWrongFormula> dialog(new DialogEditWrongFormula(&m_data, NULL_ID, this));
    dialog->setWindowTitle(tr("Edit measurement"));
    dialog->SetFormula(qApp->TrVars()->TryFormulaFromUser(ui->plainTextEditFormula->toPlainText().replace("\n", " "),
                                                          qApp->Settings()->GetOsSeparator()));
    const QString postfix = UnitsToStr(qApp->patternUnit(), true);
    dialog->setPostfix(postfix);//Show unit in dialog lable (cm, mm or inch)

    if (dialog->exec() == QDialog::Accepted)
    {
        m_measurements[row].formula = dialog->GetFormula();
        UpdateTree();

        ui->tableWidget->selectRow(row);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFinalMeasurements::FullUpdateFromFile()
{
    m_data = m_doc->GetCompleteData();
    m_measurements = m_doc->GetFinalMeasurements();

    FillFinalMeasurements();

    m_search->RefreshList(ui->lineEditFind->text());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFinalMeasurements::FillFinalMeasurements(bool freshCall)
{
    ui->tableWidget->blockSignals(true);
    ui->tableWidget->clearContents();

    ui->tableWidget->setRowCount(m_measurements.size());
    for (int i=0; i < m_measurements.size(); ++i)
    {
        const VFinalMeasurement &m = m_measurements.at(i);

        AddCell(m.name, i, 0, Qt::AlignVCenter); // name

        bool ok = true;
        const qreal result = EvalFormula(m.formula, ok);
        AddCell(qApp->LocaleToString(result), i, 1, Qt::AlignHCenter | Qt::AlignVCenter, ok); // calculated value

        const QString formula = VTranslateVars::TryFormulaFromUser(m.formula, qApp->Settings()->GetOsSeparator());
        AddCell(formula, i, 2, Qt::AlignVCenter); // formula

    }

    if (freshCall)
    {
        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->resizeRowsToContents();
    }
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFinalMeasurements::ShowUnits()
{
    const QString unit = UnitsToStr(qApp->patternUnit());

    {
        // calculated value
        const QString header = ui->tableWidget->horizontalHeaderItem(1)->text();
        const QString unitHeader = QString("%1 (%2)").arg(header).arg(unit);
        ui->tableWidget->horizontalHeaderItem(1)->setText(unitHeader);
    }

    {
        // formula
        const QString header = ui->tableWidget->horizontalHeaderItem(2)->text();
        const QString unitHeader = QString("%1 (%2)").arg(header).arg(unit);
        ui->tableWidget->horizontalHeaderItem(2)->setText(unitHeader);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFinalMeasurements::AddCell(const QString &text, int row, int column, int aligment, bool ok)
{
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

    ui->tableWidget->setItem(row, column, item);
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogFinalMeasurements::EvalUserFormula(const QString &formula, bool fromUser)
{
    const QString postfix = UnitsToStr(qApp->patternUnit());//Show unit in dialog lable (cm, mm or inch)
    if (formula.isEmpty())
    {
        ui->labelCalculatedValue->setText(tr("Error") + " (" + postfix + "). " + tr("Empty field."));
        ui->labelCalculatedValue->setToolTip(tr("Empty field"));
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
            const qreal result = cal->EvalFormula(m_data.DataVariables(), f);

            if (qIsInf(result) || qIsNaN(result))
            {
                ui->labelCalculatedValue->setText(tr("Error") + " (" + postfix + ").");
                ui->labelCalculatedValue->setToolTip(tr("Invalid result. Value is infinite or NaN. Please, check your "
                                                        "calculations."));
                return false;
            }

            ui->labelCalculatedValue->setText(qApp->LocaleToString(result) + " " + postfix);
            ui->labelCalculatedValue->setToolTip(tr("Value"));
            return true;
        }
        catch (qmu::QmuParserError &e)
        {
            ui->labelCalculatedValue->setText(tr("Error") + " (" + postfix + "). " +
                                              tr("Parser error: %1").arg(e.GetMsg()));
            ui->labelCalculatedValue->setToolTip(tr("Parser error: %1").arg(e.GetMsg()));
            return false;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogFinalMeasurements::Controls()
{
    ui->toolButtonRemove->setEnabled(ui->tableWidget->rowCount() > 0);

    if (ui->tableWidget->rowCount() >= 2)
    {
        if (ui->tableWidget->currentRow() == 0)
        {
            ui->toolButtonUp->setEnabled(false);
            ui->toolButtonDown->setEnabled(true);
        }
        else if (ui->tableWidget->currentRow() == ui->tableWidget->rowCount()-1)
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
void DialogFinalMeasurements::EnableDetails(bool enabled)
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
void DialogFinalMeasurements::UpdateTree()
{
    int row = ui->tableWidget->currentRow();
    FillFinalMeasurements();
    ui->tableWidget->selectRow(row);

    m_search->RefreshList(ui->lineEditFind->text());
}

//---------------------------------------------------------------------------------------------------------------------
qreal DialogFinalMeasurements::EvalFormula(const QString &formula, bool &ok)
{
    qreal result = 0;
    if (formula.isEmpty())
    {
        ok = false;
        return result;
    }
    else
    {
        try
        {
            QString f = formula;
            // Replace line return character with spaces for calc if exist
            f.replace("\n", " ");
            QScopedPointer<Calculator> cal(new Calculator());
            result = cal->EvalFormula(m_data.DataVariables(), f);

            if (qIsInf(result) || qIsNaN(result))
            {
                ok = false;
                return 0;
            }
        }
        catch (qmu::QmuParserError &)
        {
            ok = false;
            return 0;
        }
    }

    ok = true;
    return result;
}
