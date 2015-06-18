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
#include "../../../options.h"
#include "../../../core/vapplication.h"
#include "../../../../libs/vmisc/vsettings.h"
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

//---------------------------------------------------------------------------------------------------------------------
ConfigurationPage::ConfigurationPage(QWidget *parent)
    : QWidget(parent), autoSaveCheck(nullptr), autoTime(nullptr), langCombo(nullptr), labelCombo(nullptr),
      unitCombo(nullptr), osOptionCheck(nullptr), langChanged(false), unitChanged(false), labelLangChanged(false),
      sendReportCheck(nullptr), askPointDeletionCheck(nullptr), toolBarStyleCheck(nullptr)
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
    qApp->Settings()->SetAutosaveState(autoSaveCheck->isChecked());
    qApp->Settings()->SetAutosaveTime(autoTime->value());

    QTimer *autoSaveTimer = qApp->getAutoSaveTimer();
    SCASSERT(autoSaveTimer);

    autoSaveCheck->isChecked() ? autoSaveTimer->start(autoTime->value()*60000) : autoSaveTimer->stop();

    qApp->Settings()->SetOsSeparator(osOptionCheck->isChecked());
    qApp->Settings()->SetSendReportState(sendReportCheck->isChecked());
    qApp->Settings()->SetConfirmItemDelete(askPointDeletionCheck->isChecked());
    qApp->Settings()->SetToolBarStyle(toolBarStyleCheck->isChecked());

    if (langChanged)
    {
        const QString locale = qvariant_cast<QString>(langCombo->itemData(langCombo->currentIndex()));
        qApp->Settings()->SetLocale(locale);
        langChanged = false;
        const QString text = tr("Setup user interface language updated and will be used the next time start") + " " +
                                QApplication::applicationName();
        QMessageBox::information(this, QApplication::applicationName(), text);
    }
    if (this->unitChanged)
    {
        const QString unit = qvariant_cast<QString>(this->unitCombo->itemData(this->unitCombo->currentIndex()));
        qApp->Settings()->SetUnit(unit);
        this->unitChanged = false;
        const QString text = tr("Default unit updated and will be used the next pattern creation");
        QMessageBox::information(this, QApplication::applicationName(), text);
    }
    if (labelLangChanged)
    {
        const QString locale = qvariant_cast<QString>(labelCombo->itemData(labelCombo->currentIndex()));
        qApp->Settings()->SetLabelLanguage(locale);
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
    QGroupBox *saveGroup = new QGroupBox(tr("Save"));

    autoSaveCheck = new QCheckBox(tr("Auto-save modified pattern"));
    autoSaveCheck->setChecked(qApp->Settings()->GetAutosaveState());

    autoTime = new QSpinBox();
    autoTime->setRange(1, 60);
    autoTime->setValue(qApp->Settings()->GetAutosaveTime());
    autoTime->setSuffix(tr("min"));

    QHBoxLayout *autosaveLayout = new QHBoxLayout;
    autosaveLayout->addWidget(autoSaveCheck);
    autosaveLayout->addWidget(new QLabel(tr("Interval:")));
    autosaveLayout->addWidget(autoTime);

    QVBoxLayout *saveLayout = new QVBoxLayout;
    saveLayout->addLayout(autosaveLayout);
    saveGroup->setLayout(saveLayout);
    return saveGroup;
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *ConfigurationPage::LangGroup()
{
    QGroupBox *langGroup = new QGroupBox(tr("Language"));
    QLabel *guiLabel = new QLabel(tr("GUI language"));
    langCombo = new QComboBox;

    QDir dir(qApp->translationsPath());
    const QStringList fileNames = dir.entryList(QStringList("valentina_*.qm"));

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
    qint32 index = langCombo->findData(qApp->Settings()->GetLocale());
    if (index != -1)
    {
        langCombo->setCurrentIndex(index);
    }
    connect(langCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &ConfigurationPage::LangChanged);

    QHBoxLayout *guiLangLayout = new QHBoxLayout;
    guiLangLayout->addWidget(guiLabel);
    guiLangLayout->addWidget(langCombo);

    //-------------------- Decimal separator setup
    QLabel *separatorLabel = new QLabel(tr("Decimal separator parts"));

    osOptionCheck = new QCheckBox(tr("With OS options (%1)").arg(QLocale::system().decimalPoint().toLatin1()));
    osOptionCheck->setChecked(qApp->Settings()->GetOsSeparator());

    QHBoxLayout *separatorLayout = new QHBoxLayout;
    separatorLayout->addWidget(separatorLabel);
    separatorLayout->addWidget(osOptionCheck);

    //----------------------- Unit setup
    this->unitCombo = new QComboBox;
    QLabel *unitLabel = new QLabel(tr("Default unit"));

    this->unitCombo->addItem(tr("Centimeters"), "cm");
    this->unitCombo->addItem(tr("Millimiters"), "mm");
    this->unitCombo->addItem(tr("Inches"), "in");

    // set default unit
    qint32 indexUnit = this->unitCombo->findData(qApp->Settings()->GetUnit());
    if (indexUnit != -1)
    {
        this->unitCombo->setCurrentIndex(indexUnit);
    }
    connect(this->unitCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &ConfigurationPage::UnitChanged);

    QHBoxLayout *UnitLayout = new QHBoxLayout;
    UnitLayout->addWidget(unitLabel);
    UnitLayout->addWidget(this->unitCombo);

    //----------------------- Label language

    QLabel *labelName = new QLabel(tr("Label language"));
    labelCombo = new QComboBox;

    SetLabelComboBox(VApplication::LabelLanguages());

    index = labelCombo->findData(qApp->Settings()->GetLabelLanguage());
    if (index != -1)
    {
        labelCombo->setCurrentIndex(index);
    }
    connect(labelCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &ConfigurationPage::LabelLangChanged);

    QHBoxLayout *labelLangLayout = new QHBoxLayout;
    labelLangLayout->addWidget(labelName);
    labelLangLayout->addWidget(labelCombo);

    //-----------------------

    QVBoxLayout *langLayout = new QVBoxLayout;
    langLayout->addLayout(guiLangLayout);
    langLayout->addLayout(separatorLayout);
    langLayout->addLayout(UnitLayout);
    langLayout->addLayout(labelLangLayout);
    langGroup->setLayout(langLayout);

    return langGroup;
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *ConfigurationPage::SendGroup()
{
    QGroupBox *sendGroup = new QGroupBox(tr("Send crash reports"));

    sendReportCheck = new QCheckBox(tr("Send crash reports (recommended)"));
    sendReportCheck->setChecked(qApp->Settings()->GetSendReportState());

    QLabel *description = new QLabel(tr("After each crash Valentina collect information that may help us fix a "
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
    QGroupBox *drawGroup = new QGroupBox(tr("Pattern Editing"));

    askPointDeletionCheck = new QCheckBox(tr("Confirm item deletion"));
    askPointDeletionCheck->setChecked(qApp->Settings()->GetConfirmItemDelete());

    QVBoxLayout *editLayout = new QVBoxLayout;
    editLayout->addWidget(askPointDeletionCheck);

    drawGroup->setLayout(editLayout);
    return drawGroup;
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *ConfigurationPage::ToolBarGroup()
{
    QGroupBox *toolBarGroup = new QGroupBox(tr("Toolbar"));

    toolBarStyleCheck = new QCheckBox(tr("The text appears under the icon. (recommended for beginners.)"));
    toolBarStyleCheck->setChecked(qApp->Settings()->GetToolBarStyle());

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
