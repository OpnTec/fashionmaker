/************************************************************************
 **
 **  @file   preferencespatternpage.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 4, 2017
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

#include "preferencespatternpage.h"
#include "ui_preferencespatternpage.h"
#include "../../core/vapplication.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../dialogdatetimeformats.h"

#include <QMessageBox>
#include <QDate>
#include <QTime>

namespace
{
QStringList ComboBoxAllStrings(QComboBox *combo)
{
    SCASSERT(combo != nullptr)

    QStringList itemsInComboBox;
    for (int index = 0; index < combo->count(); ++index)
    {
        itemsInComboBox << combo->itemText(index);
    }

    return itemsInComboBox;
}
}

//---------------------------------------------------------------------------------------------------------------------
PreferencesPatternPage::PreferencesPatternPage(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::PreferencesPatternPage)
{
    ui->setupUi(this);
    ui->graphOutputCheck->setChecked(qApp->ValentinaSettings()->GetGraphicalOutput());
    ui->undoCount->setValue(qApp->ValentinaSettings()->GetUndoCount());

    InitDefaultSeamAllowance();
    InitLabelDateTimeFormats();

    ui->forbidFlippingCheck->setChecked(qApp->ValentinaSettings()->GetForbidWorkpieceFlipping());
    ui->doublePassmarkCheck->setChecked(qApp->ValentinaSettings()->IsDoublePassmark());
    ui->checkBoxHideMainPath->setChecked(qApp->ValentinaSettings()->IsHideMainPath());
    ui->fontComboBoxLabelFont->setCurrentFont(qApp->ValentinaSettings()->GetLabelFont());
}

//---------------------------------------------------------------------------------------------------------------------
PreferencesPatternPage::~PreferencesPatternPage()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesPatternPage::Apply()
{
    VSettings *settings = qApp->ValentinaSettings();

    // Scene antialiasing
    settings->SetGraphicalOutput(ui->graphOutputCheck->isChecked());
    qApp->getSceneView()->setRenderHint(QPainter::Antialiasing, ui->graphOutputCheck->isChecked());
    qApp->getSceneView()->setRenderHint(QPainter::SmoothPixmapTransform, ui->graphOutputCheck->isChecked());

    /* Maximum number of commands in undo stack may only be set when the undo stack is empty, since setting it on a
     * non-empty stack might delete the command at the current index. Calling setUndoLimit() on a non-empty stack
     * prints a warning and does nothing.*/
    settings->SetUndoCount(ui->undoCount->value());

    settings->SetDefaultSeamAllowance(ui->defaultSeamAllowance->value());

    settings->SetForbidWorkpieceFlipping(ui->forbidFlippingCheck->isChecked());
    settings->SetHideMainPath(ui->checkBoxHideMainPath->isChecked());
    settings->SetLabelFont(ui->fontComboBoxLabelFont->currentFont());

    if (settings->IsDoublePassmark() != ui->doublePassmarkCheck->isChecked())
    {
        settings->SetDoublePassmark(ui->doublePassmarkCheck->isChecked());
        qApp->getCurrentDocument()->LiteParseTree(Document::LiteParse);
    }

    settings->SetLabelDateFormat(ui->comboBoxDateFormats->currentText());
    settings->SetLabelTimeFormat(ui->comboBoxTimeFormats->currentText());

    settings->SetUserDefinedDateFormats(ComboBoxAllStrings(ui->comboBoxDateFormats));
    settings->SetUserDefinedTimeFormats(ComboBoxAllStrings(ui->comboBoxTimeFormats));
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesPatternPage::InitDefaultSeamAllowance()
{
    ui->defaultSeamAllowance->setValue(qApp->ValentinaSettings()->GetDefaultSeamAllowance());
    ui->defaultSeamAllowance->setSuffix(UnitsToStr(StrToUnits(qApp->ValentinaSettings()->GetUnit()), true));
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesPatternPage::EditDateTimeFormats()
{
    VSettings *settings = qApp->ValentinaSettings();

    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button == ui->pushButtonEditDateFormats)
    {
        CallDateTimeFormatEditor(QDate::currentDate(), settings->PredefinedDateFormats(),
                           settings->GetUserDefinedDateFormats(), ui->comboBoxDateFormats);
    }
    else if (button == ui->pushButtonEditTimeFormats)
    {
        CallDateTimeFormatEditor(QTime::currentTime(), settings->PredefinedTimeFormats(),
                           settings->GetUserDefinedTimeFormats(), ui->comboBoxTimeFormats);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesPatternPage::InitLabelDateTimeFormats()
{
    VSettings *settings = qApp->ValentinaSettings();

    InitComboBoxFormats(ui->comboBoxDateFormats,
                        VSettings::PredefinedDateFormats() + settings->GetUserDefinedDateFormats(),
                        settings->GetLabelDateFormat());
    InitComboBoxFormats(ui->comboBoxTimeFormats,
                        VSettings::PredefinedTimeFormats() + settings->GetUserDefinedTimeFormats(),
                        settings->GetLabelTimeFormat());

    connect(ui->pushButtonEditDateFormats, &QPushButton::clicked, this, &PreferencesPatternPage::EditDateTimeFormats);
    connect(ui->pushButtonEditTimeFormats, &QPushButton::clicked, this, &PreferencesPatternPage::EditDateTimeFormats);
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesPatternPage::InitComboBoxFormats(QComboBox *box, const QStringList &items, const QString &currentFormat)
{
    SCASSERT(box != nullptr)

    box->addItems(items);
    int index = box->findText(currentFormat);
    if (index != -1)
    {
        box->setCurrentIndex(index);
    }
    else
    {
        box->setCurrentIndex(0);
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void PreferencesPatternPage::CallDateTimeFormatEditor(const T &type, const QStringList &predefinedFormats,
                                                      const QStringList &userDefinedFormats, QComboBox *box)
{
    SCASSERT(box != nullptr)

    DialogDateTimeFormats dialog(type, predefinedFormats, userDefinedFormats);

    if (QDialog::Accepted == dialog.exec())
    {
        const QString currentFormat = box->currentText();
        box->clear();
        box->addItems(dialog.GetFormats());

        int index = box->findText(currentFormat);
        if (index != -1)
        {
            box->setCurrentIndex(index);
        }
        else
        {
            box->setCurrentIndex(0);
        }
    }
}
