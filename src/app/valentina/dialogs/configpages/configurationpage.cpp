/************************************************************************
 **
 **  @file   configurationpage.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 6, 2014
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

#include "configurationpage.h"
#include "../../options.h"
#include "../../core/vapplication.h"
#include "../vmisc/vsettings.h"
#include <QDir>
#include <QGroupBox>
#include <QLabel>
#include <QSettings>
#include <QTimer>
#include <QSpinBox>
#include <QComboBox>
#include <QMessageBox>
#include <QCheckBox>
#include <QIcon>
#include <QVBoxLayout>
#include <QDirIterator>
#include <QFormLayout>

//---------------------------------------------------------------------------------------------------------------------
ConfigurationPage::ConfigurationPage(QWidget *parent)
    : QWidget(parent),
      autoSaveCheck(nullptr),
      autoTime(nullptr),
      langCombo(nullptr),
      labelCombo(nullptr),
      unitCombo(nullptr),
      osOptionCheck(nullptr),
      langChanged(false),
      unitChanged(false),
      labelLangChanged(false),
      sendReportCheck(nullptr),
      askPointDeletionCheck(nullptr),
      toolBarStyleCheck(nullptr),
      saveGroup(nullptr),
      intervalLabel(nullptr),
      langGroup(nullptr),
      guiLabel(nullptr),
      separatorLabel(nullptr),
      unitLabel(nullptr),
      languageLabel(nullptr),
      sendGroup(nullptr),
      description(nullptr),
      drawGroup(nullptr),
      toolBarGroup(nullptr)
{
    QGroupBox *saveGroup = SaveGroup();
    QGroupBox *langGroup = LangGroup();
    QGroupBox *sendGroup = SendGroup();
    QGroupBox *drawGroup = DrawGroup();
    QGroupBox *toolBarGroup = ToolBarGroup();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(saveGroup);
    mainLayout->addWidget(langGroup);
    mainLayout->addWidget(sendGroup);
    mainLayout->addWidget(drawGroup);
    mainLayout->addWidget(toolBarGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

//---------------------------------------------------------------------------------------------------------------------
void ConfigurationPage::Apply()
{
    VSettings *settings = qApp->ValentinaSettings();
    settings->SetAutosaveState(autoSaveCheck->isChecked());
    settings->SetAutosaveTime(autoTime->value());

    QTimer *autoSaveTimer = qApp->getAutoSaveTimer();
    SCASSERT(autoSaveTimer);

    autoSaveCheck->isChecked() ? autoSaveTimer->start(autoTime->value()*60000) : autoSaveTimer->stop();

    settings->SetOsSeparator(osOptionCheck->isChecked());
    settings->SetSendReportState(sendReportCheck->isChecked());
    settings->SetConfirmItemDelete(askPointDeletionCheck->isChecked());
    settings->SetToolBarStyle(toolBarStyleCheck->isChecked());

    if (langChanged)
    {
        const QString locale = qvariant_cast<QString>(langCombo->itemData(langCombo->currentIndex()));
        settings->SetLocale(locale);
        langChanged = false;
        qApp->LoadTranslation(locale);
    }
    if (this->unitChanged)
    {
        const QString unit = qvariant_cast<QString>(this->unitCombo->itemData(this->unitCombo->currentIndex()));
        settings->SetUnit(unit);
        this->unitChanged = false;
        const QString text = tr("Default unit updated and will be used the next pattern creation");
        QMessageBox::information(this, QApplication::applicationName(), text);
    }
    if (labelLangChanged)
    {
        const QString locale = qvariant_cast<QString>(labelCombo->itemData(labelCombo->currentIndex()));
        settings->SetLabelLanguage(locale);
        labelLangChanged = false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void ConfigurationPage::LangChanged()
{
    langChanged = true;
}

//---------------------------------------------------------------------------------------------------------------------
void ConfigurationPage::UnitChanged()
{
    this->unitChanged = true;
}

//---------------------------------------------------------------------------------------------------------------------
void ConfigurationPage::LabelLangChanged()
{
    labelLangChanged = true;
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *ConfigurationPage::SaveGroup()
{
    saveGroup = new QGroupBox(tr("Save"));

    autoSaveCheck = new QCheckBox(tr("Auto-save modified pattern"));
    autoSaveCheck->setChecked(qApp->ValentinaSettings()->GetAutosaveState());

    autoTime = new QSpinBox();
    autoTime->setRange(1, 60);
    autoTime->setValue(qApp->ValentinaSettings()->GetAutosaveTime());
    autoTime->setSuffix(tr("min"));

    QHBoxLayout *autosaveLayout = new QHBoxLayout;
    autosaveLayout->addWidget(autoSaveCheck);
    intervalLabel = new QLabel(tr("Interval:"));
    autosaveLayout->addWidget(intervalLabel);
    autosaveLayout->addWidget(autoTime);

    QVBoxLayout *saveLayout = new QVBoxLayout;
    saveLayout->addLayout(autosaveLayout);
    saveGroup->setLayout(saveLayout);
    return saveGroup;
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *ConfigurationPage::LangGroup()
{
    langGroup = new QGroupBox(tr("Language"));
    guiLabel  = new QLabel(tr("GUI language:"));
    langCombo = new QComboBox;

    QStringList fileNames;
    QDirIterator it(qApp->translationsPath(), QStringList() << QStringList("valentina_*.qm"), QDir::Files,
                    QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        it.next();
        fileNames.append(it.fileName());
    }

    for (int i = 0; i < fileNames.size(); ++i)
    {
        // get locale extracted by filename
        QString locale;
        locale = fileNames.at(i);                  // "valentina_de_De.qm"
        locale.truncate(locale.lastIndexOf('.'));   // "valentina_de_De"
        locale.remove(0, locale.indexOf('_') + 1);   // "de_De"

        QLocale loc = QLocale(locale);
        QString lang = loc.nativeLanguageName();
        QIcon ico(QString("%1/%2.png").arg("://flags").arg(QLocale::countryToString(loc.country())));

        langCombo->addItem(ico, lang, locale);
    }

    if (langCombo->count() == 0)
    {
        // English language is internal and doens't have own *.qm file.
        QIcon ico(QString("%1/%2.png").arg("://flags").arg(QLocale::countryToString(QLocale::UnitedStates)));
        QString lang = QLocale("en_US").nativeLanguageName();
        langCombo->addItem(ico, lang, "en_US");
    }

    // set default translators and language checked
    const VSettings *settings = qApp->ValentinaSettings();
    qint32 index = langCombo->findData(settings->GetLocale());
    if (index != -1)
    {
        langCombo->setCurrentIndex(index);
    }
    connect(langCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &ConfigurationPage::LangChanged);

    QFormLayout *langLayout = new QFormLayout;
    langLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    langLayout->addRow(guiLabel, langCombo);

    //-------------------- Decimal separator setup
    separatorLabel = new QLabel(tr("Decimal separator parts:"));
    osOptionCheck  = new QCheckBox(tr("With OS options (%1)").arg(QLocale::system().decimalPoint().toLatin1()));
    osOptionCheck->setChecked(settings->GetOsSeparator());
    langLayout->addRow(separatorLabel, osOptionCheck);

    //----------------------- Unit setup
    this->unitCombo = new QComboBox;
    this->unitCombo->addItem(tr("Centimeters"), "cm");
    this->unitCombo->addItem(tr("Millimiters"), "mm");
    this->unitCombo->addItem(tr("Inches"), "in");

    // set default unit
    const qint32 indexUnit = this->unitCombo->findData(settings->GetUnit());
    if (indexUnit != -1)
    {
        this->unitCombo->setCurrentIndex(indexUnit);
    }
    connect(this->unitCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &ConfigurationPage::UnitChanged);

    unitLabel = new QLabel(tr("Default unit:"));
    langLayout->addRow(unitLabel, this->unitCombo);

    //----------------------- Label language
    labelCombo = new QComboBox;

    SetLabelComboBox(VApplication::LabelLanguages());

    index = labelCombo->findData(settings->GetLabelLanguage());
    if (index != -1)
    {
        labelCombo->setCurrentIndex(index);
    }
    connect(labelCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &ConfigurationPage::LabelLangChanged);

    languageLabel = new QLabel(tr("Label language:"));
    langLayout->addRow(languageLabel, labelCombo);

    langGroup->setLayout(langLayout);
    return langGroup;
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *ConfigurationPage::SendGroup()
{
    sendGroup = new QGroupBox(tr("Send crash reports"));

    sendReportCheck = new QCheckBox(tr("Send crash reports (recommended)"));
    sendReportCheck->setChecked(qApp->ValentinaSettings()->GetSendReportState());

    description = new QLabel(tr("After each crash Valentina collect information that may help us fix a "
                                "problem. We do not collect any personal information. Find more about what "
                                "<a href=\"https://bitbucket.org/dismine/valentina/wiki/manual/"
                                "Crash_reports\">kind of information</a> we collect."));
    description->setTextFormat(Qt::RichText);
    description->setTextInteractionFlags(Qt::TextBrowserInteraction);
    description->setOpenExternalLinks(true);
    description->setWordWrap(true);

    QVBoxLayout *sendLayout = new QVBoxLayout;
    sendLayout->addWidget(sendReportCheck);
    sendLayout->addWidget(description);

    sendGroup->setLayout(sendLayout);
    return sendGroup;
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *ConfigurationPage::DrawGroup()
{
    drawGroup = new QGroupBox(tr("Pattern Editing"));

    askPointDeletionCheck = new QCheckBox(tr("Confirm item deletion"));
    askPointDeletionCheck->setChecked(qApp->ValentinaSettings()->GetConfirmItemDelete());

    QVBoxLayout *editLayout = new QVBoxLayout;
    editLayout->addWidget(askPointDeletionCheck);

    drawGroup->setLayout(editLayout);
    return drawGroup;
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *ConfigurationPage::ToolBarGroup()
{
    toolBarGroup = new QGroupBox(tr("Toolbar"));

    toolBarStyleCheck = new QCheckBox(tr("The text appears under the icon. (recommended for beginners.)"));
    toolBarStyleCheck->setChecked(qApp->ValentinaSettings()->GetToolBarStyle());

    QVBoxLayout *editLayout = new QVBoxLayout;
    editLayout->addWidget(toolBarStyleCheck);

    toolBarGroup->setLayout(editLayout);
    return toolBarGroup;
}

//---------------------------------------------------------------------------------------------------------------------
void ConfigurationPage::SetLabelComboBox(const QStringList &list)
{
    for (int i = 0; i < list.size(); ++i)
    {
        QLocale loc = QLocale(list.at(i));
        labelCombo->addItem(loc.nativeLanguageName(), list.at(i));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void ConfigurationPage::changeEvent(QEvent *event)
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
void ConfigurationPage::RetranslateUi()
{
    toolBarStyleCheck->setText(tr("The text appears under the icon. (recommended for beginners.)"));
    askPointDeletionCheck->setText(tr("Confirm item deletion"));

    saveGroup->setTitle(tr("Save"));
    autoSaveCheck->setText(tr("Auto-save modified pattern"));
    autoTime->setSuffix(tr("min"));
    intervalLabel->setText(tr("Interval:"));

    langGroup->setTitle(tr("Language"));
    guiLabel->setText(tr("GUI language:"));

    separatorLabel->setText(tr("Decimal separator parts:"));
    osOptionCheck->setText(tr("With OS options (%1)").arg(QLocale::system().decimalPoint().toLatin1()));

    unitLabel->setText(tr("Default unit:"));
    this->unitCombo->setItemText(0, tr("Centimeters"));
    this->unitCombo->setItemText(1, tr("Millimiters"));
    this->unitCombo->setItemText(2, tr("Inches"));

    languageLabel->setText(tr("Label language:"));

    sendGroup->setTitle(tr("Send crash reports"));
    sendReportCheck->setText(tr("Send crash reports (recommended)"));
    description->setText(tr("After each crash Valentina collect information that may help us fix a "
                            "problem. We do not collect any personal information. Find more about what "
                            "<a href=\"https://bitbucket.org/dismine/valentina/wiki/manual/"
                            "Crash_reports\">kind of information</a> we collect."));

    drawGroup->setTitle(tr("Pattern Editing"));
    askPointDeletionCheck->setText(tr("Confirm item deletion"));
    toolBarGroup->setTitle(tr("Toolbar"));
    toolBarStyleCheck->setText(tr("The text appears under the icon. (recommended for beginners.)"));
}
