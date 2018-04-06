/************************************************************************
 **
 **  @file   dialogexporttocsv.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   1 6, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "dialogexporttocsv.h"
#include "ui_dialogexporttocsv.h"

#include "../vcommonsettings.h"
#include "../qxtcsvmodel.h"
#include "../vabstractapplication.h"

#include <QPushButton>
#include <QShowEvent>
#include <QTextCodec>

//---------------------------------------------------------------------------------------------------------------------
DialogExportToCSV::DialogExportToCSV(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogExportToCSV),
      isInitialized(false),
      m_fileName()
{
    ui->setupUi(this);

    const QList<int> mibs = QTextCodec::availableMibs();
    for (auto mib : mibs)
    {
        ui->comboBoxCodec->addItem(QTextCodec::codecForMib(mib)->name(), mib);
    }

    ui->comboBoxCodec->setCurrentIndex(ui->comboBoxCodec->findData(VCommonSettings::GetDefCSVCodec()));

    SetSeparator(VCommonSettings::GetDefCSVSeparator());

    QPushButton *bDefaults = ui->buttonBox->button(QDialogButtonBox::RestoreDefaults);
    SCASSERT(bDefaults != nullptr)
    connect(bDefaults, &QPushButton::clicked, this, [this]()
    {
        ui->comboBoxCodec->blockSignals(true);
        ui->checkBoxWithHeader->blockSignals(true);
        ui->buttonGroup->blockSignals(true);

        ui->checkBoxWithHeader->setChecked(qApp->Settings()->GetDefCSVWithHeader());
        ui->comboBoxCodec->setCurrentIndex(ui->comboBoxCodec->findData(VCommonSettings::GetDefCSVCodec()));

        SetSeparator(VCommonSettings::GetDefCSVSeparator());

        ui->comboBoxCodec->blockSignals(false);
        ui->checkBoxWithHeader->blockSignals(false);
        ui->buttonGroup->blockSignals(false);

        ShowPreview();
    });

    ui->groupBoxPreview->setVisible(false);

    connect(ui->comboBoxCodec, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](){ShowPreview();});
    connect(ui->checkBoxWithHeader, &QCheckBox::stateChanged, this, [this](){ShowPreview();});
    connect(ui->buttonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, [this](){ShowPreview();});
}

//---------------------------------------------------------------------------------------------------------------------
DialogExportToCSV::~DialogExportToCSV()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogExportToCSV::IsWithHeader() const
{
    return ui->checkBoxWithHeader->isChecked();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogExportToCSV::SetWithHeader(bool value)
{
    ui->checkBoxWithHeader->setChecked(value);
}

//---------------------------------------------------------------------------------------------------------------------
int DialogExportToCSV::GetSelectedMib() const
{
    if (ui->comboBoxCodec->currentIndex() != -1)
    {
        return ui->comboBoxCodec->currentData().toInt();
    }
    else
    {
        return VCommonSettings::GetDefCSVCodec();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogExportToCSV::SetSelectedMib(int value)
{
    const int index = ui->comboBoxCodec->findData(value);
    if (index != -1)
    {
        ui->comboBoxCodec->setCurrentIndex(index);
    }
    else
    {
        ui->comboBoxCodec->setCurrentIndex(ui->comboBoxCodec->findData(VCommonSettings::GetDefCSVCodec()));
    }
}

//---------------------------------------------------------------------------------------------------------------------
QChar DialogExportToCSV::GetSeparator() const
{
    if (ui->radioButtonTab->isChecked())
    {
        return QChar('\t');
    }
    else if (ui->radioButtonSemicolon->isChecked())
    {
        return QChar(';');
    }
    else if (ui->radioButtonSpace->isChecked())
    {
        return QChar(' ');
    }
    else
    {
        return VCommonSettings::GetDefCSVSeparator();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogExportToCSV::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        // retranslate designer form (single inheritance approach)
        ui->retranslateUi(this);
    }

    // remember to call base class implementation
    QDialog::changeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogExportToCSV::showEvent(QShowEvent *event)
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

    resize(1, 1);
    adjustSize();
    if (not m_fileName.isEmpty())
    {
        ShowPreview();
    }
    else
    {
        setMaximumSize(size());
        setMinimumSize(size());
    }

    isInitialized = true;//first show windows are held
}

//---------------------------------------------------------------------------------------------------------------------
void DialogExportToCSV::ShowPreview()
{
    if (m_fileName.isEmpty())
    {
        return;
    }

    ui->groupBoxPreview->setVisible(true);

    QxtCsvModel csv(m_fileName, nullptr, IsWithHeader(), GetSeparator(), QTextCodec::codecForMib(GetSelectedMib()));

    const int columns = csv.columnCount();
    const int rows = csv.rowCount();

    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(columns);
    ui->tableWidget->setRowCount(rows);

    ui->tableWidget->horizontalHeader()->setVisible(IsWithHeader());
    if (IsWithHeader())
    {
        for(int column=0; column<columns; ++column)
        {
            QTableWidgetItem *header = new QTableWidgetItem(csv.headerText(column));
            ui->tableWidget->setHorizontalHeaderItem(column, header);
        }
        ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    }

    for (int row=0; row < rows; ++row)
    {
        for(int column=0; column<columns; ++column)
        {
            QTableWidgetItem *item = new QTableWidgetItem(csv.text(row, column));
            item->setToolTip(csv.text(row, column));

            // set the item non-editable (view only), and non-selectable
            Qt::ItemFlags flags = item->flags();
            flags &= ~(Qt::ItemIsEditable); // reset/clear the flag
            item->setFlags(flags);

            ui->tableWidget->setItem(row, column, item);
        }
    }

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogExportToCSV::SetSeparator(const QChar &separator)
{
    switch(separator.toLatin1())
    {
        case '\t':
            ui->radioButtonTab->setChecked(true);
            break;
        case ';':
            ui->radioButtonSemicolon->setChecked(true);
            break;
        case ' ':
            ui->radioButtonSpace->setChecked(true);
            break;
        case ',':
        default:
            ui->radioButtonComma->setChecked(true);
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogExportToCSV::ShowFilePreview(const QString &fileName)
{
    m_fileName = fileName;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogExportToCSV::MakeHelpCodecsList()
{
    QString out("\n");
    const QList<int> list = QTextCodec::availableMibs();
    for (int i = 0; i < list.size(); ++i)
    {
        out += QLatin1String("\t* ") + QTextCodec::codecForMib(list.at(i))->name();
        if (i < list.size()-1)
        {
            out += QLatin1String(",\n");
        }
        else
        {
            out += QLatin1String(".\n");
        }
    }
    return out;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogExportToCSV::MakeHelpSeparatorList()
{
    QString out("\n");
    out += QLatin1String("\t* 'Tab',\n");
    out += QLatin1String("\t* ';',\n");
    out += QLatin1String("\t* 'Space',\n");
    out += QLatin1String("\t* ','.\n");
    return out;
}
