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
      systemBookValueLabel(nullptr)
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
    qApp->TapeSettings()->SetOsSeparator(osOptionCheck->isChecked());

    if (langChanged || systemChanged)
    {
        const QString locale = qvariant_cast<QString>(langCombo->itemData(langCombo->currentIndex()));
        qApp->TapeSettings()->SetLocale(locale);
        langChanged = false;

        const QString code = qvariant_cast<QString>(systemCombo->itemData(systemCombo->currentIndex()));
        qApp->TapeSettings()->SetPMSystemCode(code);
        systemChanged = false;

        qApp->LoadTranslation();
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
    systemAuthorValueLabel->setText(qApp->TrVars()->PMSystemAuthor(systemCombo->currentData().toString()));
    systemBookValueLabel->setText(qApp->TrVars()->PMSystemBook(systemCombo->currentData().toString()));
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
QGroupBox *TapeConfigurationPage::LangGroup()
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
    qint32 index = langCombo->findData(qApp->TapeSettings()->GetLocale());
    if (index != -1)
    {
        langCombo->setCurrentIndex(index);
    }
    connect(langCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &TapeConfigurationPage::LangChanged);

    QFormLayout *langLayout = new QFormLayout;
    langLayout->addRow(guiLabel, langCombo);

    //-------------------- Pattern making system
    QLabel *systemLabel = new QLabel(tr("Pattern making system"));
    systemCombo = new QComboBox;

    InitPMSystems(systemCombo);

    langLayout->addRow(systemLabel, systemCombo);

    //----
    QLabel *systemAuthorLabel = new QLabel(tr("Author:"));
    systemAuthorValueLabel = new QLabel(qApp->TrVars()->PMSystemAuthor(p0_S));

    langLayout->addRow(systemAuthorLabel, systemAuthorValueLabel);

    //----
    QLabel *systemBookLabel = new QLabel(tr("Book:"));
    systemBookValueLabel = new QLabel(qApp->TrVars()->PMSystemBook(p0_S));
    systemBookValueLabel->setWordWrap(true);

    langLayout->addRow(systemBookLabel, systemBookValueLabel);

    // set default pattern making system
    index = systemCombo->findData(qApp->TapeSettings()->GetPMSystemCode());
    if (index != -1)
    {
        systemCombo->setCurrentIndex(index);
    }
    connect(systemCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &TapeConfigurationPage::SystemChanged);

    //-------------------- Decimal separator setup
    QLabel *separatorLabel = new QLabel(tr("Decimal separator parts"));

    osOptionCheck = new QCheckBox(tr("With OS options (%1)").arg(QLocale::system().decimalPoint().toLatin1()));
    osOptionCheck->setChecked(qApp->TapeSettings()->GetOsSeparator());

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
void TapeConfigurationPage::InitPMSystems(QComboBox *systemCombo)
{
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p0_S), p0_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p1_S), p1_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p2_S), p2_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p3_S), p3_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p4_S), p4_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p5_S), p5_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p6_S), p6_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p7_S), p7_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p8_S), p8_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p9_S), p9_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p10_S), p10_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p11_S), p11_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p12_S), p12_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p13_S), p13_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p14_S), p14_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p15_S), p15_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p16_S), p16_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p17_S), p17_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p18_S), p18_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p19_S), p19_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p20_S), p20_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p21_S), p21_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p22_S), p22_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p23_S), p23_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p24_S), p24_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p25_S), p25_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p26_S), p26_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p27_S), p27_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p28_S), p28_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p29_S), p29_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p30_S), p30_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p31_S), p31_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p32_S), p32_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p33_S), p33_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p34_S), p34_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p35_S), p35_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p36_S), p36_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p37_S), p37_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p38_S), p38_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p39_S), p39_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p40_S), p40_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p41_S), p41_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p42_S), p42_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p43_S), p43_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p44_S), p44_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p45_S), p45_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p46_S), p46_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p47_S), p47_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p48_S), p48_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p49_S), p49_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p50_S), p50_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p51_S), p51_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p52_S), p52_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p53_S), p53_S);
    systemCombo->addItem(qApp->TrVars()->PMSystemName(p54_S), p54_S);
}
