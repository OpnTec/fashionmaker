/************************************************************************
 **
 **  @file   preferencesconfigurationpage.cpp
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

#include "preferencesconfigurationpage.h"
#include "ui_preferencesconfigurationpage.h"
#include "../../core/vapplication.h"

#include <QDir>
#include <QDirIterator>
#include <QMessageBox>
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
    ui->autoSaveCheck->setChecked(qApp->ValentinaSettings()->GetAutosaveState());

    InitLanguages();
    connect(ui->langCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]()
    {
        m_langChanged = true;
    });

    //-------------------- Decimal separator setup
    ui->osOptionCheck->setText(tr("With OS options") + QString(" (%1)").arg(QLocale().decimalPoint()));
    ui->osOptionCheck->setChecked(qApp->ValentinaSettings()->GetOsSeparator());

    //----------------------- Unit setup
    InitUnits();
    connect(ui->unitCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]()
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
    connect(ui->labelCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]()
    {
        m_labelLangChanged = true;
    });

    //---------------------- Pattern making system
    InitPMSystems(ui->systemCombo);
    ui->systemBookValueLabel->setFixedHeight(4 * QFontMetrics(ui->systemBookValueLabel->font()).lineSpacing());
    connect(ui->systemCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]()
    {
        m_systemChanged = true;
        QString text = qApp->TrVars()->PMSystemAuthor(CURRENT_DATA(ui->systemCombo).toString());
        ui->systemAuthorValueLabel->setText(text);
        ui->systemAuthorValueLabel->setToolTip(text);

        text = qApp->TrVars()->PMSystemBook(CURRENT_DATA(ui->systemCombo).toString());
        ui->systemBookValueLabel->setPlainText(text);
    });

    // set default pattern making system
    index = ui->systemCombo->findData(qApp->ValentinaSettings()->GetPMSystemCode());
    if (index != -1)
    {
        ui->systemCombo->setCurrentIndex(index);
    }
    //---------------------- Send crash reports
    ui->sendReportCheck->setChecked(qApp->ValentinaSettings()->GetSendReportState());
    ui->description = new QLabel(tr("After each crash Valentina collects information that may help us fix the "
                                    "problem. We do not collect any personal information. Find more about what %1"
                                    "kind of information%2 we collect.")
                                 .arg("<a href=\"https://wiki.valentinaproject.org/wiki/UserManual:Crash_reports\">")
                                 .arg("</a>"));

    //----------------------------- Pattern Editing
    connect(ui->resetWarningsButton, &QPushButton::released, []()
    {
        VSettings *settings = qApp->ValentinaSettings();

        settings->SetConfirmItemDelete(true);
        settings->SetConfirmFormatRewriting(true);
    });

    //----------------------- Toolbar
    ui->toolBarStyleCheck->setChecked(qApp->ValentinaSettings()->GetToolBarStyle());
}

//---------------------------------------------------------------------------------------------------------------------
PreferencesConfigurationPage::~PreferencesConfigurationPage()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesConfigurationPage::Apply()
{
    VSettings *settings = qApp->ValentinaSettings();
    settings->SetAutosaveState(ui->autoSaveCheck->isChecked());
    settings->SetAutosaveTime(ui->autoTime->value());

    QTimer *autoSaveTimer = qApp->getAutoSaveTimer();
    SCASSERT(autoSaveTimer)

    ui->autoSaveCheck->isChecked() ? autoSaveTimer->start(ui->autoTime->value()*60000) : autoSaveTimer->stop();

    settings->SetOsSeparator(ui->osOptionCheck->isChecked());
    settings->SetSendReportState(ui->sendReportCheck->isChecked());
    settings->SetToolBarStyle(ui->toolBarStyleCheck->isChecked());

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
        const QString text = tr("The Default unit has been updated and will be used as the default for the next "
                                "pattern you create.");
        QMessageBox::information(this, QApplication::applicationName(), text);
    }
    if (m_labelLangChanged)
    {
        const QString locale = qvariant_cast<QString>(ui->labelCombo->currentData());
        settings->SetLabelLanguage(locale);
        m_labelLangChanged = false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesConfigurationPage::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        // retranslate designer form (single inheritance approach)
        RetranslateUi();
    }
    // remember to call base class implementation
    QWidget::changeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesConfigurationPage::SetLabelComboBox(const QStringList &list)
{
    for (int i = 0; i < list.size(); ++i)
    {
        QLocale loc = QLocale(list.at(i));
        ui->labelCombo->addItem(loc.nativeLanguageName(), list.at(i));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesConfigurationPage::InitLanguages()
{
    QStringList fileNames;
    QDirIterator it(qApp->translationsPath(), QStringList("valentina_*.qm"), QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        it.next();
        fileNames.append(it.fileName());
    }

    bool englishUS = false;
    const QString en_US = QStringLiteral("en_US");

    for (int i = 0; i < fileNames.size(); ++i)
    {
        // get locale extracted by filename
        QString locale;
        locale = fileNames.at(i);                  // "valentina_de_De.qm"
        locale.truncate(locale.lastIndexOf('.'));  // "valentina_de_De"
        locale.remove(0, locale.indexOf('_') + 1); // "de_De"

        if (not englishUS)
        {
            englishUS = (en_US == locale);
        }

        QLocale loc = QLocale(locale);
        QString lang = loc.nativeLanguageName();
        QIcon ico(QString("%1/%2.png").arg("://flags").arg(QLocale::countryToString(loc.country())));

        ui->langCombo->addItem(ico, lang, locale);
    }

    if (ui->langCombo->count() == 0 || not englishUS)
    {
        // English language is internal and doens't have own *.qm file.
        QIcon ico(QString("%1/%2.png").arg("://flags").arg(QLocale::countryToString(QLocale::UnitedStates)));
        QString lang = QLocale(en_US).nativeLanguageName();
        ui->langCombo->addItem(ico, lang, en_US);
    }

    // set default translators and language checked
    qint32 index = ui->langCombo->findData(qApp->ValentinaSettings()->GetLocale());
    if (index != -1)
    {
        ui->langCombo->setCurrentIndex(index);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesConfigurationPage::InitUnits()
{
    ui->unitCombo->addItem(tr("Centimeters"), "cm");
    ui->unitCombo->addItem(tr("Millimiters"), "mm");
    ui->unitCombo->addItem(tr("Inches"), "in");

    // set default unit
    const qint32 indexUnit = ui->unitCombo->findData(qApp->ValentinaSettings()->GetUnit());
    if (indexUnit != -1)
    {
        ui->unitCombo->setCurrentIndex(indexUnit);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void PreferencesConfigurationPage::RetranslateUi()
{
    ui->osOptionCheck->setText(tr("With OS options") + QString(" (%1)").arg(QLocale().decimalPoint()));
    ui->description->setText(tr("After each crash Valentina collects information that may help us fix the "
                                "problem. We do not collect any personal information. Find more about what %1"
                                "kind of information%2 we collect.")
                             .arg("<a href=\"https://wiki.valentinaproject.org/wiki/UserManual:Crash_reports\">")
                             .arg("</a>"));
}
