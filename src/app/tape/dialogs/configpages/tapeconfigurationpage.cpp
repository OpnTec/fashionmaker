/************************************************************************
 **
 **  @file   tapeTapeConfigurationPage.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   02 08, 2015
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

#include "tapeconfigurationpage.h"
#include "../../mapplication.h"
#include "../vmisc/vtapesettings.h"
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
#include <QFormLayout>
#include <QDirIterator>

//---------------------------------------------------------------------------------------------------------------------
TapeConfigurationPage::TapeConfigurationPage(QWidget *parent)
    : QWidget(parent),
      langCombo(nullptr),
      systemCombo(nullptr),
      labelCombo(nullptr),
      unitCombo(nullptr),
      osOptionCheck(nullptr),
      langChanged(false),
      systemChanged(false),
      unitChanged(false),
      labelLangChanged(false),
      sendReportCheck(nullptr),
      askPointDeletionCheck(nullptr),
      toolBarStyleCheck(nullptr),
      systemAuthorValueLabel(nullptr),
      systemBookValueLabel(nullptr),
      langGroup(nullptr),
      guiLabel(nullptr),
      systemLabel(nullptr),
      systemAuthorLabel(nullptr),
      systemBookLabel(nullptr),
      separatorLabel(nullptr)
{
    QGroupBox *langGroup = LangGroup();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(langGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

//---------------------------------------------------------------------------------------------------------------------
void TapeConfigurationPage::Apply()
{
    VTapeSettings *settings = qApp->TapeSettings();
    settings->SetOsSeparator(osOptionCheck->isChecked());

    if (langChanged || systemChanged)
    {
        const QString locale = qvariant_cast<QString>(langCombo->itemData(langCombo->currentIndex()));
        settings->SetLocale(locale);
        langChanged = false;

        const QString code = qvariant_cast<QString>(systemCombo->itemData(systemCombo->currentIndex()));
        settings->SetPMSystemCode(code);
        systemChanged = false;

        qApp->LoadTranslation(locale);

        // Part about measurments will not be updated automatically
        qApp->RetranslateTables();
        qApp->RetranslateGroups();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TapeConfigurationPage::LangChanged()
{
    langChanged = true;
}

//---------------------------------------------------------------------------------------------------------------------
void TapeConfigurationPage::SystemChanged()
{
    systemChanged = true;
#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    QString text = qApp->TrVars()->PMSystemAuthor(systemCombo->itemData(systemCombo->currentIndex()).toString());
#else
    QString text = qApp->TrVars()->PMSystemAuthor(systemCombo->currentData().toString());
#endif
    systemAuthorValueLabel->setText(text);
    systemAuthorValueLabel->setToolTip(text);
#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    text = qApp->TrVars()->PMSystemBook(systemCombo->itemData(systemCombo->currentIndex()).toString());
#else
    text = qApp->TrVars()->PMSystemBook(systemCombo->currentData().toString());
#endif
    systemBookValueLabel->setPlainText(text);
}

//---------------------------------------------------------------------------------------------------------------------
void TapeConfigurationPage::UnitChanged()
{
    this->unitChanged = true;
}

//---------------------------------------------------------------------------------------------------------------------
void TapeConfigurationPage::LabelLangChanged()
{
    labelLangChanged = true;
}

//---------------------------------------------------------------------------------------------------------------------
void TapeConfigurationPage::changeEvent(QEvent *event)
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
QGroupBox *TapeConfigurationPage::LangGroup()
{
    langGroup = new QGroupBox(tr("Language"));
    guiLabel = new QLabel(tr("GUI language:"));
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
        QString locale = fileNames.at(i);                         // "valentina_de_De.qm"
        locale.truncate(locale.lastIndexOf(QLatin1String(".")));  // "valentina_de_De"
        locale.remove(0, locale.indexOf(QLatin1String("_")) + 1); // "de_De"

        const QLocale loc(locale);
        const QString lang = loc.nativeLanguageName();
        QIcon ico(QString("%1/%2.png").arg("://flags").arg(QLocale::countryToString(loc.country())));

        langCombo->addItem(ico, lang, locale);
    }

    if (langCombo->count() == 0)
    {
        // English language is internal and doens't have own *.qm file.
        QIcon ico(QString("%1/%2.png").arg("://flags").arg(QLocale::countryToString(QLocale::UnitedStates)));
        const QString lang = QLocale("en_US").nativeLanguageName();
        langCombo->addItem(ico, lang, "en_US");
    }

    // set default translators and language checked
    const VTapeSettings *settings = qApp->TapeSettings();
    qint32 index = langCombo->findData(settings->GetLocale());
    if (index != -1)
    {
        langCombo->setCurrentIndex(index);
    }
    connect(langCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &TapeConfigurationPage::LangChanged);

    QFormLayout *langLayout = new QFormLayout;
    langLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    langLayout->addRow(guiLabel, langCombo);

    //-------------------- Pattern making system
    systemLabel = new QLabel(tr("Pattern making system:"));
    systemCombo = new QComboBox;

    InitPMSystems(systemCombo);

    langLayout->addRow(systemLabel, systemCombo);

    //----
    systemAuthorLabel = new QLabel(tr("Author:"));
    systemAuthorValueLabel = new QLabel("");

    langLayout->addRow(systemAuthorLabel, systemAuthorValueLabel);

    //----
    systemBookLabel = new QLabel(tr("Book:"));
    systemBookValueLabel = new QPlainTextEdit("");
    systemBookValueLabel->setReadOnly(true);
    systemBookValueLabel->setFixedHeight(4 * QFontMetrics(systemBookValueLabel->font()).lineSpacing());

    langLayout->addRow(systemBookLabel, systemBookValueLabel);

    connect(systemCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &TapeConfigurationPage::SystemChanged);

    // set default pattern making system
    index = systemCombo->findData(settings->GetPMSystemCode());
    if (index != -1)
    {
        systemCombo->setCurrentIndex(index);
    }

    //-------------------- Decimal separator setup
    separatorLabel = new QLabel(tr("Decimal separator parts:"));

    osOptionCheck = new QCheckBox(tr("With OS options (%1)").arg(QLocale::system().decimalPoint().toLatin1()));
    osOptionCheck->setChecked(settings->GetOsSeparator());

    langLayout->addRow(separatorLabel, osOptionCheck);
    //-----------------------
    langGroup->setLayout(langLayout);

    return langGroup;
}

//---------------------------------------------------------------------------------------------------------------------
void TapeConfigurationPage::SetLabelComboBox(const QStringList &list)
{
    for (int i = 0; i < list.size(); ++i)
    {
        QLocale loc = QLocale(list.at(i));
        labelCombo->addItem(loc.nativeLanguageName(), list.at(i));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TapeConfigurationPage::RetranslateUi()
{
    langGroup->setTitle(tr("Language"));
    guiLabel->setText(tr("GUI language"));
    systemLabel->setText(tr("Pattern making system"));

    const int index = systemCombo->currentIndex();
    systemCombo->blockSignals(true);
    systemCombo->clear();
    InitPMSystems(systemCombo);
    systemCombo->setCurrentIndex(index);
    systemCombo->blockSignals(false);

    systemAuthorLabel->setText(tr("Author:"));
    systemBookLabel->setText(tr("Book:"));

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    QString text = qApp->TrVars()->PMSystemAuthor(systemCombo->itemData(systemCombo->currentIndex()).toString());
#else
    QString text = qApp->TrVars()->PMSystemAuthor(systemCombo->currentData().toString());
#endif
    systemAuthorValueLabel->setText(text);
    systemAuthorValueLabel->setToolTip(text);
#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    text = qApp->TrVars()->PMSystemBook(systemCombo->itemData(systemCombo->currentIndex()).toString());
#else
    text = qApp->TrVars()->PMSystemBook(systemCombo->currentData().toString());
#endif
    systemBookValueLabel->setPlainText(text);

    separatorLabel->setText(tr("Decimal separator parts"));
    osOptionCheck = new QCheckBox(tr("With OS options (%1)").arg(QLocale::system().decimalPoint().toLatin1()));
}
