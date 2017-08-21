/************************************************************************
 **
 **  @file   dialogdatetimeformats.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   19 8, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

#include "dialogdatetimeformats.h"
#include "ui_dialogdatetimeformats.h"

#ifdef Q_CC_MSVC
    #include <ciso646>
#endif /* Q_CC_MSVC */

//---------------------------------------------------------------------------------------------------------------------
DialogDateTimeFormats::DialogDateTimeFormats(const QDate &date, const QStringList &predefinedFormats,
                                             const QStringList &userDefinedFormats, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogDateTimeFormats),
      m_dateMode(true),
      m_date(date),
      m_time(),
      m_predefined(predefinedFormats)
{
    ui->setupUi(this);

    Init(predefinedFormats, userDefinedFormats);
}

//---------------------------------------------------------------------------------------------------------------------
DialogDateTimeFormats::DialogDateTimeFormats(const QTime &time, const QStringList &predefinedFormats,
                                             const QStringList &userDefinedFormats, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogDateTimeFormats),
      m_dateMode(false),
      m_date(),
      m_time(time),
      m_predefined(predefinedFormats)
{
    ui->setupUi(this);

    Init(predefinedFormats, userDefinedFormats);
}

//---------------------------------------------------------------------------------------------------------------------
DialogDateTimeFormats::~DialogDateTimeFormats()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList DialogDateTimeFormats::GetFormats() const
{
    QStringList formats;

    for (int i=0; i<ui->listWidget->count(); ++i)
    {
        if (const QListWidgetItem *lineItem = ui->listWidget->item(i))
        {
            const QString format = lineItem->data(Qt::UserRole).toString();
            if (not format.isEmpty())
            {
                formats.append(lineItem->data(Qt::UserRole).toString());
            }
        }
    }

    return formats;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDateTimeFormats::AddLine()
{
    int row = ui->listWidget->count();
    ui->listWidget->insertItem(++row, new QListWidgetItem(tr("<empty>")));
    ui->listWidget->setCurrentRow(row);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDateTimeFormats::RemoveLine()
{
    ui->listWidget->blockSignals(true);
    if (QListWidgetItem *curLine = ui->listWidget->currentItem())
    {
        if (not m_predefined.contains(curLine->data(Qt::UserRole).toString()))
        {
            delete ui->listWidget->takeItem(ui->listWidget->currentRow());
        }
    }
    ui->listWidget->blockSignals(false);
    ShowFormatDetails();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDateTimeFormats::SaveFormat(const QString &text)
{
    if (QListWidgetItem *curLine = ui->listWidget->currentItem())
    {
        if (not GetFormats().contains(text))
        {
            const QString preview = m_dateMode ? m_date.toString(text) : m_time.toString(text);
            curLine->setText(preview);
            curLine->setData(Qt::UserRole, text);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDateTimeFormats::ShowFormatDetails()
{
    if (ui->listWidget->count() > 0)
    {
        if (const QListWidgetItem *line = ui->listWidget->currentItem())
        {
            ui->lineEditFormat->blockSignals(true);
            ui->lineEditFormat->setText(line->data(Qt::UserRole).toString());
            ui->lineEditFormat->blockSignals(false);
        }
    }

    SetupControls();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDateTimeFormats::Init(const QStringList &predefined, const QStringList &userDefined)
{
    SetFormatLines(predefined, userDefined);

    ui->lineEditFormat->setClearButtonEnabled(true);

    connect(ui->toolButtonRemove, &QToolButton::clicked, this, &DialogDateTimeFormats::RemoveLine);
    connect(ui->toolButtonAdd, &QToolButton::clicked, this, &DialogDateTimeFormats::AddLine);
    connect(ui->lineEditFormat, &QLineEdit::textEdited, this, &DialogDateTimeFormats::SaveFormat);
    connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, &DialogDateTimeFormats::ShowFormatDetails);

    ui->listWidget->setCurrentRow(0);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDateTimeFormats::SetFormatLines(const QStringList &predefined, const QStringList &userDefined)
{
    ui->listWidget->blockSignals(true);
    ui->listWidget->clear();

    int row = -1;

    for (int i=0; i<predefined.size(); ++i)
    {
        ui->listWidget->insertItem(++row, AddListLine(predefined.at(i)));
    }

    for (int i=0; i<userDefined.size(); ++i)
    {
        ui->listWidget->insertItem(++row, AddListLine(userDefined.at(i)));
    }

    ui->listWidget->blockSignals(false);

    if (ui->listWidget->count() > 0)
    {
        ui->listWidget->setCurrentRow(0);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDateTimeFormats::SetupControls()
{
    const bool enabled = ui->listWidget->count() > 0;

    if (not enabled)
    {
        ui->lineEditFormat->blockSignals(true);
        ui->lineEditFormat->clear();
        ui->lineEditFormat->blockSignals(false);
    }

    ui->toolButtonAdd->setEnabled(true);

    ui->lineEditFormat->setEnabled(enabled);

    const QListWidgetItem *line = ui->listWidget->currentItem();
    if (line != nullptr && m_predefined.contains(line->data(Qt::UserRole).toString()))
    {
        ui->toolButtonRemove->setEnabled(false);
        ui->lineEditFormat->setReadOnly(true);
    }
    else
    {
        ui->toolButtonRemove->setEnabled(enabled);
        ui->lineEditFormat->setReadOnly(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QListWidgetItem *DialogDateTimeFormats::AddListLine(const QString &format)
{
    const QString preview = m_dateMode ? m_date.toString(format) : m_time.toString(format);
    QListWidgetItem *item = new QListWidgetItem(preview);
    item->setData(Qt::UserRole, format);
    return item;
}
