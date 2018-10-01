/************************************************************************
 **
 **  @file   preferencespatternpage.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 4, 2017
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

#include "preferencespatternpage.h"
#include "ui_preferencespatternpage.h"
#include "../../core/vapplication.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../dialogdatetimeformats.h"
#include "../dialogknownmaterials.h"

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
      ui(new Ui::PreferencesPatternPage),
      m_knownMaterials(),
      m_oldLineUnit(Unit::Mm)
{
    ui->setupUi(this);
    RetranslateUi();

    VSettings *settings = qApp->ValentinaSettings();

    ui->graphOutputCheck->setChecked(settings->GetGraphicalOutput());
    ui->checkBoxOpenGLRender->setChecked(settings->IsOpenGLRender());
    ui->doubleSpinBoxCurveApproximation->setValue(settings->GetCurveApproximationScale());
    ui->doubleSpinBoxCurveApproximation->setMinimum(minCurveApproximationScale);
    ui->doubleSpinBoxCurveApproximation->setMaximum(maxCurveApproximationScale);
    ui->undoCount->setValue(settings->GetUndoCount());

    //----------------------- Unit setup
    // set default unit
    const Unit defUnit = QLocale().measurementSystem() == QLocale::MetricSystem ? Unit::Mm : Unit::Inch;
    const qint32 indexUnit = ui->comboBoxLineWidthUnit->findData(static_cast<int>(defUnit));
    if (indexUnit != -1)
    {
        ui->comboBoxLineWidthUnit->setCurrentIndex(indexUnit);
    }

    connect(ui->comboBoxLineWidthUnit, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]()
    {
        const Unit lineUnit = static_cast<Unit>(ui->comboBoxLineWidthUnit->currentData().toInt());
        const qreal value = UnitConvertor(ui->doubleSpinBoxLineWidth->value(), m_oldLineUnit, lineUnit);
        ui->doubleSpinBoxLineWidth->setDecimals(lineUnit == Unit::Mm ? 1 : 6);
        ui->doubleSpinBoxLineWidth->setMinimum(UnitConvertor(ui->doubleSpinBoxLineWidth->minimum(), m_oldLineUnit,
                                                             lineUnit));
        ui->doubleSpinBoxLineWidth->setMaximum(UnitConvertor(ui->doubleSpinBoxLineWidth->maximum(), m_oldLineUnit,
                                                             lineUnit));
        ui->doubleSpinBoxLineWidth->setValue(value);
        m_oldLineUnit = lineUnit;
    });

    m_oldLineUnit = static_cast<Unit>(ui->comboBoxLineWidthUnit->currentData().toInt());
    ui->doubleSpinBoxLineWidth->setDecimals(m_oldLineUnit == Unit::Mm ? 1 : 6);
    ui->doubleSpinBoxLineWidth->setMinimum(UnitConvertor(VCommonSettings::MinimalLineWidth(), Unit::Mm, m_oldLineUnit));
    ui->doubleSpinBoxLineWidth->setMaximum(UnitConvertor(VCommonSettings::MaximalLineWidth(), Unit::Mm, m_oldLineUnit));
    ui->doubleSpinBoxLineWidth->setValue(UnitConvertor(settings->GetLineWidth(), Unit::Mm, m_oldLineUnit));

    InitDefaultSeamAllowance();
    InitLabelDateTimeFormats();

    ui->forbidFlippingCheck->setChecked(settings->GetForbidWorkpieceFlipping());
    ui->doublePassmarkCheck->setChecked(settings->IsDoublePassmark());
    ui->checkBoxHideMainPath->setChecked(settings->IsHideMainPath());
    ui->fontComboBoxLabelFont->setCurrentFont(settings->GetLabelFont());

    ui->checkBoxRemeberPatternMaterials->setChecked(settings->IsRememberPatternMaterials());
    m_knownMaterials = settings->GetKnownMaterials();

    connect(ui->pushButtonKnownMaterials, &QPushButton::clicked, this, &PreferencesPatternPage::ManageKnownMaterials);
}

//---------------------------------------------------------------------------------------------------------------------
PreferencesPatternPage::~PreferencesPatternPage()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList PreferencesPatternPage::Apply()
{
    QStringList preferences;

    VSettings *settings = qApp->ValentinaSettings();

    // Scene antialiasing
    if (settings->GetGraphicalOutput() != ui->graphOutputCheck->isChecked())
    {
        if (qApp->getSceneView()->IsOpenGLRender())
        {
            preferences.append(tr("antialiasing"));
        }

        settings->SetGraphicalOutput(ui->graphOutputCheck->isChecked());
    }

    if (settings->IsOpenGLRender() != ui->checkBoxOpenGLRender->isChecked())
    {
        preferences.append(tr("scene render"));
        settings->SetOpenGLRender(ui->checkBoxOpenGLRender->isChecked());
    }

    settings->SetCurveApproximationScale(ui->doubleSpinBoxCurveApproximation->value());
    settings->SetLineWidth(UnitConvertor(ui->doubleSpinBoxLineWidth->value(), m_oldLineUnit, Unit::Mm));
    qApp->getSceneView()->SetAntialiasing(ui->graphOutputCheck->isChecked());

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

    settings->SetKnownMaterials(m_knownMaterials);
    settings->SetRememberPatternMaterials(ui->checkBoxRemeberPatternMaterials->isChecked());

    return preferences;
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesPatternPage::InitDefaultSeamAllowance()
{
    ui->defaultSeamAllowance->setValue(qApp->ValentinaSettings()->GetDefaultSeamAllowance());
    ui->defaultSeamAllowance->setSuffix(UnitsToStr(StrToUnits(qApp->ValentinaSettings()->GetUnit()), true));
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesPatternPage::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        // retranslate designer form (single inheritance approach)
        RetranslateUi();
        ui->retranslateUi(this);
    }
    // remember to call base class implementation
    QWidget::changeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesPatternPage::EditDateTimeFormats()
{
    VSettings *settings = qApp->ValentinaSettings();

    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button == ui->pushButtonEditDateFormats)
    {
        CallDateTimeFormatEditor(QDate::currentDate(), VCommonSettings::PredefinedDateFormats(),
                           settings->GetUserDefinedDateFormats(), ui->comboBoxDateFormats);
    }
    else if (button == ui->pushButtonEditTimeFormats)
    {
        CallDateTimeFormatEditor(QTime::currentTime(), VCommonSettings::PredefinedTimeFormats(),
                           settings->GetUserDefinedTimeFormats(), ui->comboBoxTimeFormats);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesPatternPage::ManageKnownMaterials()
{
    DialogKnownMaterials editor;
    editor.SetList(m_knownMaterials);

    if (QDialog::Accepted == editor.exec())
    {
        m_knownMaterials = editor.GetList();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesPatternPage::InitLabelDateTimeFormats()
{
    VSettings *settings = qApp->ValentinaSettings();

    InitComboBoxFormats(ui->comboBoxDateFormats,
                        VCommonSettings::PredefinedDateFormats() + settings->GetUserDefinedDateFormats(),
                        settings->GetLabelDateFormat());
    InitComboBoxFormats(ui->comboBoxTimeFormats,
                        VCommonSettings::PredefinedTimeFormats() + settings->GetUserDefinedTimeFormats(),
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
void PreferencesPatternPage::InitUnits()
{
    ui->comboBoxLineWidthUnit->addItem(tr("Millimiters"), static_cast<int>(Unit::Mm));
    ui->comboBoxLineWidthUnit->addItem(tr("Inches"), static_cast<int>(Unit::Inch));
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesPatternPage::RetranslateUi()
{
    ui->comboBoxLineWidthUnit->blockSignals(true);
    const int unit = ui->comboBoxLineWidthUnit->currentData().toInt();
    ui->comboBoxLineWidthUnit->clear();
    InitUnits();
    ui->comboBoxLineWidthUnit->setCurrentIndex(ui->comboBoxLineWidthUnit->findData(unit));
    ui->comboBoxLineWidthUnit->blockSignals(false);
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
