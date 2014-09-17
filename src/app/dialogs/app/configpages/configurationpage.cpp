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
 **  Copyright (C) 2014 Valentina project
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
#include "../../../widgets/vapplication.h"
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
      unitCombo(nullptr), osOptionCheck(nullptr), langChanged(false), unitChanged(false), labelLangChanged(false)
{
    QGroupBox *saveGroup = SaveGroup();
    QGroupBox *langGroup = LangGroup();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(saveGroup);
    mainLayout->addWidget(langGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

//---------------------------------------------------------------------------------------------------------------------
void ConfigurationPage::Apply()
{
    qApp->getSettings()->setValue("configuration/autosave/state", autoSaveCheck->isChecked());
    qApp->getSettings()->setValue("configuration/autosave/time", autoTime->value());

    QTimer *autoSaveTimer = qApp->getAutoSaveTimer();
    SCASSERT(autoSaveTimer);

    if (autoSaveCheck->isChecked())
    {
        autoSaveTimer->start(autoTime->value()*60000);
    }
    else
    {
        autoSaveTimer->stop();
    }

    qApp->getSettings()->setValue("configuration/osSeparator", osOptionCheck->isChecked());

    if (langChanged)
    {
        QString locale = qvariant_cast<QString>(langCombo->itemData(langCombo->currentIndex()));
        qApp->getSettings()->setValue("configuration/locale", locale);
        langChanged = false;
        QString text = QString(tr("Setup user interface language updated and will be used the next time start") + " " +
                               QApplication::applicationName());
        QMessageBox::information(this, QApplication::applicationName(), text);
    }
    if (this->unitChanged)
    {
        QString unit = qvariant_cast<QString>(this->unitCombo->itemData(this->unitCombo->currentIndex()));
        qApp->getSettings()->setValue("configuration/unit", unit);
        this->unitChanged = false;
        QString text = QString(tr("Default unit updated and will be used the next pattern creation"));
        QMessageBox::information(this, QApplication::applicationName(), text);
    }
    if (labelLangChanged)
    {
        QString locale = qvariant_cast<QString>(labelCombo->itemData(labelCombo->currentIndex()));
        qApp->getSettings()->setValue("configuration/label_language", locale);
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
    QSettings *settings = qApp->getSettings();
    SCASSERT(settings != nullptr);

    QGroupBox *saveGroup = new QGroupBox(tr("Save"));

    autoSaveCheck = new QCheckBox(tr("Auto-save modified pattern"));
    bool autoSaveValue = settings->value("configuration/autosave/state", 1).toBool();
    autoSaveCheck->setChecked(autoSaveValue);

    QLabel *intervalLabel = new QLabel(tr("Interval:"));

    autoTime = new QSpinBox();
    bool ok = true;
    qint32 autoTimeValue = settings->value("configuration/autosave/time", 5).toInt(&ok);
    if (ok == false)
    {
        autoTimeValue = 5;
    }
    autoTime->setRange(1, 60);
    autoTime->setValue(autoTimeValue);
    autoTime->setSuffix(tr("min"));

    QHBoxLayout *autosaveLayout = new QHBoxLayout;
    autosaveLayout->addWidget(autoSaveCheck);
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
    QSettings *settings = qApp->getSettings();
    SCASSERT(settings != nullptr);

    QGroupBox *langGroup = new QGroupBox(tr("Language"));
    QLabel *guiLabel = new QLabel(tr("GUI language"));
    langCombo = new QComboBox;

    // format systems language
    QString checkedLocale = settings->value("configuration/locale", QLocale::system().name()).toString();

    QString m_langPath = qApp->translationsPath();
    QDir dir(m_langPath);
    QStringList fileNames = dir.entryList(QStringList("valentina_*.qm"));

    for (int i = 0; i < fileNames.size(); ++i)
    {
        // get locale extracted by filename
        QString locale;
        locale = fileNames.at(i);                  // "valentina_de_De.qm"
        locale.truncate(locale.lastIndexOf('.'));   // "valentina_de_De"
        locale.remove(0, locale.indexOf('_') + 1);   // "de_De"

        QLocale loc = QLocale(locale);
        QString lang = loc.nativeLanguageName();
        QIcon ico(QString("%1/%2.png").arg("://icon/flags").arg(QLocale::countryToString(loc.country())));

        langCombo->addItem(ico, lang, locale);
    }

    // English language is internal and doens't have own *.qm file.
    QIcon ico(QString("%1/%2.png").arg("://icon/flags").arg(QLocale::countryToString(QLocale::UnitedStates)));
    QString lang = QLocale("en_US").nativeLanguageName();
    langCombo->addItem(ico, lang, "en_US");

    // set default translators and language checked
    qint32 index = langCombo->findData(checkedLocale);
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
    bool osOptionValue = settings->value("configuration/osSeparator", 1).toBool();
    osOptionCheck->setChecked(osOptionValue);

    QHBoxLayout *separatorLayout = new QHBoxLayout;
    separatorLayout->addWidget(separatorLabel);
    separatorLayout->addWidget(osOptionCheck);

    //----------------------- Unit setup
    this->unitCombo = new QComboBox;
    QLabel *unitLabel = new QLabel(tr("Default unit"));

    QString checkedUnit = settings->value("configuration/unit", "cm").toString();

    this->unitCombo->addItem(tr("Centimeters"), "cm");
    this->unitCombo->addItem(tr("Millimiters"), "mm");
    this->unitCombo->addItem(tr("Inches"), "in");

    // set default unit
    qint32 indexUnit = this->unitCombo->findData(checkedUnit);
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

    QString checkedLabelLocale = settings->value("configuration/label_language",
                                                 QLocale::system().bcp47Name()).toString();

    SetLabelComboBox(VApplication::LabelLanguages());

    index = labelCombo->findData(checkedLabelLocale);
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
void ConfigurationPage::SetLabelComboBox(const QStringList &list)
{
    for (int i = 0; i < list.size(); ++i)
    {
        QLocale loc = QLocale(list.at(i));
        labelCombo->addItem(loc.nativeLanguageName(), list.at(i));
    }
}
