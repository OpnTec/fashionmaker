/************************************************************************
 **
 **  @file   preferencesconfigurationpage.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 4, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#include "preferencesconfigurationpage.h"
#include "ui_preferencesconfigurationpage.h"
#include "../../core/vapplication.h"
#include "../vpatterndb/pmsystems.h"

#include <QDir>
#include <QDirIterator>
#include <QTimer>

//---------------------------------------------------------------------------------------------------------------------
PreferencesConfigurationPage::PreferencesConfigurationPage(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::PreferencesConfigurationPage),
      m_langChanged(false),
      m_systemChanged(),
      m_unitChanged(false),
      m_labelLangChanged(false)
{
    ui->setupUi(this);
    RetranslateUi();

    ui->tabWidget->setCurrentIndex(0);

    // Tab General
    ui->autoSaveCheck->setChecked(qApp->ValentinaSettings()->GetAutosaveState());

    InitLanguages(ui->langCombo);
    connect(ui->langCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]()
    {
        m_langChanged = true;
    });

    //-------------------- Decimal separator setup
    ui->osOptionCheck->setChecked(qApp->ValentinaSettings()->GetOsSeparator());

    //----------------------- Unit setup
    // set default unit
    const qint32 indexUnit = ui->unitCombo->findData(qApp->ValentinaSettings()->GetUnit());
    if (indexUnit != -1)
    {
        ui->unitCombo->setCurrentIndex(indexUnit);
    }

    connect(ui->unitCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]()
    {
        m_unitChanged = true;
    });

    //----------------------- Label language
    SetLabelComboBox(VApplication::LabelLanguages());

    int index = ui->labelCombo->findData(qApp->ValentinaSettings()->GetLabelLanguage());
    if (index != -1)
    {
        ui->labelCombo->setCurrentIndex(index);
    }
    connect(ui->labelCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]()
    {
        m_labelLangChanged = true;
    });

    //---------------------- Pattern making system
    ui->systemBookValueLabel->setFixedHeight(4 * QFontMetrics(ui->systemBookValueLabel->font()).lineSpacing());
    connect(ui->systemCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]()
    {
        m_systemChanged = true;
        QString text = qApp->TrVars()->PMSystemAuthor(ui->systemCombo->currentData().toString());
        ui->systemAuthorValueLabel->setText(text);
        ui->systemAuthorValueLabel->setToolTip(text);

        text = qApp->TrVars()->PMSystemBook(ui->systemCombo->currentData().toString());
        ui->systemBookValueLabel->setPlainText(text);
    });

    // set default pattern making system
    index = ui->systemCombo->findData(qApp->ValentinaSettings()->GetPMSystemCode());
    if (index != -1)
    {
        ui->systemCombo->setCurrentIndex(index);
    }

    //----------------------------- Pattern Editing
    connect(ui->resetWarningsButton, &QPushButton::released, this, []()
    {
        VSettings *settings = qApp->ValentinaSettings();

        settings->SetConfirmItemDelete(true);
        settings->SetConfirmFormatRewriting(true);
    });

    VSettings *settings = qApp->ValentinaSettings();

    ui->checkBoxFreeCurve->setChecked(settings->IsFreeCurveMode());
    ui->checkBoxZoomFitBestCurrentPP->setChecked(settings->IsDoubleClickZoomFitBestCurrentPP());

    //----------------------- Toolbar
    ui->toolBarStyleCheck->setChecked(settings->GetToolBarStyle());

    // Theme
    ui->darkModeCheck->setChecked(settings->GetDarkMode());

    // Tool panel
    ui->checkBoxToolPanelScaling->setChecked(settings->GetToolPanelScaling());

    // Tab Scrolling
    ui->spinBoxDuration->setMinimum(VSettings::scrollingDurationMin);
    ui->spinBoxDuration->setMaximum(VSettings::scrollingDurationMax);
    ui->spinBoxDuration->setValue(settings->GetScrollingDuration());

    ui->spinBoxUpdateInterval->setMinimum(VSettings::scrollingUpdateIntervalMin);
    ui->spinBoxUpdateInterval->setMaximum(VSettings::scrollingUpdateIntervalMax);
    ui->spinBoxUpdateInterval->setValue(settings->GetScrollingUpdateInterval());

    ui->doubleSpinBoxSensor->setMinimum(VSettings::sensorMouseScaleMin);
    ui->doubleSpinBoxSensor->setMaximum(VSettings::sensorMouseScaleMax);
    ui->doubleSpinBoxSensor->setValue(settings->GetSensorMouseScale());

    ui->doubleSpinBoxWheel->setMinimum(VSettings::wheelMouseScaleMin);
    ui->doubleSpinBoxWheel->setMaximum(VSettings::wheelMouseScaleMax);
    ui->doubleSpinBoxWheel->setValue(settings->GetWheelMouseScale());

    ui->doubleSpinBoxAcceleration->setMinimum(VSettings::scrollingAccelerationMin);
    ui->doubleSpinBoxAcceleration->setMaximum(VSettings::scrollingAccelerationMax);
    ui->doubleSpinBoxAcceleration->setValue(settings->GetScrollingAcceleration());
}

//---------------------------------------------------------------------------------------------------------------------
PreferencesConfigurationPage::~PreferencesConfigurationPage()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList PreferencesConfigurationPage::Apply()
{
    // Tab General
    QStringList preferences;
    VSettings *settings = qApp->ValentinaSettings();
    settings->SetAutosaveState(ui->autoSaveCheck->isChecked());
    settings->SetAutosaveTime(ui->autoTime->value());

    QTimer *autoSaveTimer = qApp->getAutoSaveTimer();
    SCASSERT(autoSaveTimer)

    ui->autoSaveCheck->isChecked() ? autoSaveTimer->start(ui->autoTime->value()*60000) : autoSaveTimer->stop();

    settings->SetOsSeparator(ui->osOptionCheck->isChecked());
    settings->SetToolBarStyle(ui->toolBarStyleCheck->isChecked());

    if (settings->GetDarkMode() != ui->darkModeCheck->isChecked())
    {
        settings->SetDarkMode(ui->darkModeCheck->isChecked());
        preferences.append(tr("dark mode"));
    }

    if (settings->GetToolPanelScaling() != ui->checkBoxToolPanelScaling->isChecked())
    {
        settings->SetToolPanelScaling(ui->checkBoxToolPanelScaling->isChecked());
    }

    settings->SetFreeCurveMode(ui->checkBoxFreeCurve->isChecked());
    settings->SetDoubleClickZoomFitBestCurrentPP(ui->checkBoxZoomFitBestCurrentPP->isChecked());

    if (m_langChanged || m_systemChanged)
    {
        const QString locale = qvariant_cast<QString>(ui->langCombo->currentData());
        settings->SetLocale(locale);
        m_langChanged = false;

        const QString code = qvariant_cast<QString>(ui->systemCombo->currentData());
        settings->SetPMSystemCode(code);
        m_systemChanged = false;

        qApp->LoadTranslation(locale);
    }
    if (m_unitChanged)
    {
        const QString unit = qvariant_cast<QString>(ui->unitCombo->currentData());
        settings->SetUnit(unit);
        m_unitChanged = false;
        preferences.append(tr("default unit"));
    }
    if (m_labelLangChanged)
    {
        const QString locale = qvariant_cast<QString>(ui->labelCombo->currentData());
        settings->SetLabelLanguage(locale);
        m_labelLangChanged = false;
    }

    // Tab Scrolling
    settings->SetScrollingDuration(ui->spinBoxDuration->value());
    settings->SetScrollingUpdateInterval(ui->spinBoxUpdateInterval->value());
    settings->SetSensorMouseScale(ui->doubleSpinBoxSensor->value());
    settings->SetWheelMouseScale(ui->doubleSpinBoxWheel->value());
    settings->SetScrollingAcceleration(ui->doubleSpinBoxAcceleration->value());

    return preferences;
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesConfigurationPage::changeEvent(QEvent *event)
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
void PreferencesConfigurationPage::SetLabelComboBox(const QStringList &list)
{
    for (auto &name : list)
    {
        ui->labelCombo->addItem(QLocale(name).nativeLanguageName(), name);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesConfigurationPage::InitUnits()
{
    ui->unitCombo->addItem(tr("Centimeters"), unitCM);
    ui->unitCombo->addItem(tr("Millimiters"), unitMM);
    ui->unitCombo->addItem(tr("Inches"), unitINCH);
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesConfigurationPage::RetranslateUi()
{
    ui->osOptionCheck->setText(tr("With OS options") + QStringLiteral(" (%1)").arg(QLocale().decimalPoint()));

    {
    ui->unitCombo->blockSignals(true);
    const QString unit = qvariant_cast<QString>(ui->unitCombo->currentData());
    ui->unitCombo->clear();
    InitUnits();
    ui->unitCombo->setCurrentIndex(ui->unitCombo->findData(unit));
    ui->unitCombo->blockSignals(false);
    }

    {
    const QString code = qvariant_cast<QString>(ui->systemCombo->currentData());
    ui->systemCombo->blockSignals(true);
    ui->systemCombo->clear();
    InitPMSystems(ui->systemCombo);
    ui->systemCombo->setCurrentIndex(-1);
    ui->systemCombo->blockSignals(false);
    ui->systemCombo->setCurrentIndex(ui->systemCombo->findData(code));
    }
}
