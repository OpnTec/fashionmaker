/************************************************************************
 **
 **  @file   dialogeditlabel.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 8, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2017 Valentina project
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

#include "dialogeditlabel.h"
#include "ui_dialogeditlabel.h"
#include "../vmisc/vabstractapplication.h"

#include <QDir>
#include <QMessageBox>
#include <QFileDialog>

//---------------------------------------------------------------------------------------------------------------------
DialogEditLabel::DialogEditLabel(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogEditLabel)
{
    ui->setupUi(this);

    ui->lineEditLine->setClearButtonEnabled(true);

    connect(ui->toolButtonAdd, &QToolButton::clicked, this, &DialogEditLabel::AddLine);
    connect(ui->toolButtonRemove, &QToolButton::clicked, this, &DialogEditLabel::RemoveLine);
    connect(ui->lineEditLine, &QLineEdit::textEdited, this, &DialogEditLabel::SaveLineText);
    connect(ui->toolButtonBold, &QToolButton::toggled, this, &DialogEditLabel::SaveFontStyle);
    connect(ui->toolButtonItalic, &QToolButton::toggled, this, &DialogEditLabel::SaveFontStyle);
    connect(ui->toolButtonTextLeft, &QToolButton::toggled, this, &DialogEditLabel::SaveTextFormating);
    connect(ui->toolButtonTextCenter, &QToolButton::toggled, this, &DialogEditLabel::SaveTextFormating);
    connect(ui->toolButtonTextRight, &QToolButton::toggled, this, &DialogEditLabel::SaveTextFormating);
    connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, &DialogEditLabel::ShowLineDetails);
    connect(ui->toolButtonNewLabel, &QToolButton::clicked, this, &DialogEditLabel::NewTemplate);
}

//---------------------------------------------------------------------------------------------------------------------
DialogEditLabel::~DialogEditLabel()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::ShowLineDetails()
{
    if (ui->listWidget->count() > 0)
    {
        const QListWidgetItem *line = ui->listWidget->currentItem();
        if (line)
        {
            ui->lineEditLine->blockSignals(true);
            ui->lineEditLine->setText(line->text());
            ui->lineEditLine->selectAll();
            ui->lineEditLine->setFocus();
            ui->lineEditLine->blockSignals(false);

            const QFont lineFont = line->font();

            ui->toolButtonBold->blockSignals(true);
            ui->toolButtonBold->setChecked(lineFont.bold());
            ui->toolButtonBold->blockSignals(false);

            ui->toolButtonItalic->blockSignals(true);
            ui->toolButtonItalic->setChecked(lineFont.italic());
            ui->toolButtonItalic->blockSignals(false);

            ui->toolButtonTextLeft->blockSignals(true);
            ui->toolButtonTextCenter->blockSignals(true);
            ui->toolButtonTextRight->blockSignals(true);

            const int lineAlignment = line->textAlignment();

            if (lineAlignment & Qt::AlignLeft)
            {
                ui->toolButtonTextLeft->setChecked(true);
                ui->toolButtonTextCenter->setChecked(false);
                ui->toolButtonTextRight->setChecked(false);
            }
            else if (lineAlignment & Qt::AlignCenter)
            {
                ui->toolButtonTextLeft->setChecked(false);
                ui->toolButtonTextCenter->setChecked(true);
                ui->toolButtonTextRight->setChecked(false);
            }
            else if (lineAlignment & Qt::AlignRight)
            {
                ui->toolButtonTextLeft->setChecked(false);
                ui->toolButtonTextCenter->setChecked(false);
                ui->toolButtonTextRight->setChecked(true);
            }

            ui->toolButtonTextLeft->blockSignals(false);
            ui->toolButtonTextCenter->blockSignals(false);
            ui->toolButtonTextRight->blockSignals(false);
        }
    }

    SetupControls();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::AddLine()
{
    int row = ui->listWidget->currentRow();
    ui->listWidget->insertItem(++row, new QListWidgetItem(tr("<empty>")));
    ui->listWidget->setCurrentRow(row);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::RemoveLine()
{
    ui->listWidget->blockSignals(true);
    QListWidgetItem *curLine = ui->listWidget->takeItem(ui->listWidget->currentRow());
    if (curLine)
    {
        delete curLine;
    }
    ui->listWidget->blockSignals(false);
    ShowLineDetails();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::SaveLineText(const QString &text)
{
    QListWidgetItem *curLine = ui->listWidget->currentItem();
    if (curLine)
    {
        curLine->setText(text);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::SaveFontStyle(bool checked)
{
    QListWidgetItem *curLine = ui->listWidget->currentItem();
    if (curLine)
    {
        QFont lineFont = curLine->font();

        QToolButton *button = qobject_cast<QToolButton *>(sender());
        if (button)
        {
            if (button == ui->toolButtonBold)
            {
                lineFont.setBold(checked);
            }
            else if (button == ui->toolButtonItalic)
            {
                lineFont.setItalic(checked);
            }
        }

        curLine->setFont(lineFont);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::SaveTextFormating(bool checked)
{
    QListWidgetItem *curLine = ui->listWidget->currentItem();
    if (curLine)
    {
        QToolButton *button = qobject_cast<QToolButton *>(sender());
        if (button)
        {
            ui->toolButtonTextLeft->blockSignals(true);
            ui->toolButtonTextCenter->blockSignals(true);
            ui->toolButtonTextRight->blockSignals(true);

            if (button == ui->toolButtonTextLeft)
            {
                if (checked)
                {
                    curLine->setTextAlignment(Qt::AlignLeft);

                    ui->toolButtonTextCenter->setChecked(false);
                    ui->toolButtonTextRight->setChecked(false);
                }
                else
                {
                    button->setChecked(true);
                }
            }
            else if (button == ui->toolButtonTextCenter)
            {
                if (checked)
                {
                    curLine->setTextAlignment(Qt::AlignCenter);

                    ui->toolButtonTextLeft->setChecked(false);
                    ui->toolButtonTextRight->setChecked(false);
                }
                else
                {
                    button->setChecked(true);
                }
            }
            else if (button == ui->toolButtonTextRight)
            {
                if (checked)
                {
                    curLine->setTextAlignment(Qt::AlignRight);

                    ui->toolButtonTextCenter->setChecked(false);
                    ui->toolButtonTextRight->setChecked(false);
                }
                else
                {
                    button->setChecked(true);
                }
            }

            ui->toolButtonTextLeft->blockSignals(false);
            ui->toolButtonTextCenter->blockSignals(false);
            ui->toolButtonTextRight->blockSignals(false);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::NewTemplate()
{
    if (ui->listWidget->count() > 0)
    {
        const QMessageBox::StandardButton answer = QMessageBox::question(this, tr("Create new template"),
                                                            tr("Creating new template will overwrite the current, do "
                                                               "you want to continue?"),
                                                            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (answer == QMessageBox::No)
        {
            return;
        }
    }

    ui->listWidget->blockSignals(true);
    ui->listWidget->clear();
    ui->listWidget->blockSignals(false);
    ShowLineDetails();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::ExportTemplate()
{

}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::ImportTemplate()
{

}

//---------------------------------------------------------------------------------------------------------------------
void DialogEditLabel::SetupControls()
{
    const bool enabled = ui->listWidget->count() > 0;

    if (not enabled)
    {
        ui->lineEditLine->blockSignals(true);
        ui->lineEditLine->clear();
        ui->lineEditLine->blockSignals(false);
    }

    ui->toolButtonAdd->setEnabled(true);
    ui->toolButtonImportLabel->setEnabled(true);

    ui->toolButtonRemove->setEnabled(enabled);
    ui->toolButtonBold->setEnabled(enabled);
    ui->toolButtonItalic->setEnabled(enabled);
    ui->toolButtonTextLeft->setEnabled(enabled);
    ui->toolButtonTextCenter->setEnabled(enabled);
    ui->toolButtonTextRight->setEnabled(enabled);
    ui->pushButtonInsert->setEnabled(enabled);
    ui->toolButtonNewLabel->setEnabled(enabled);
    ui->toolButtonExportLabel->setEnabled(enabled);
    ui->lineEditLine->setEnabled(enabled);
}
