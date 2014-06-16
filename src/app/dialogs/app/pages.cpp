/************************************************************************
 **
 **  @file   pages.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 2, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#include "pages.h"
#include "../../options.h"
#include "../../widgets/vapplication.h"
#include "../../widgets/vmaingraphicsview.h"

//---------------------------------------------------------------------------------------------------------------------
ConfigurationPage::ConfigurationPage(QWidget *parent):
    QWidget(parent), autoSaveCheck(nullptr), autoTime(nullptr), langCombo(nullptr), unitCombo(nullptr),
    osOptionCheck(nullptr), langChanged(false), unitChanged(false)
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
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName());
    settings.setValue("configuration/autosave/state", autoSaveCheck->isChecked());
    settings.setValue("configuration/autosave/time", autoTime->value());

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

    settings.setValue("configuration/osSeparator", osOptionCheck->isChecked());

    if (langChanged)
    {
        QString locale = qvariant_cast<QString>(langCombo->itemData(langCombo->currentIndex()));
        settings.setValue("configuration/locale", locale);
        langChanged = false;
        QString text = QString(tr("Setup user interface language updated and will be used the next time start") + " " +
                               QApplication::applicationName());
        QMessageBox::information(this, QApplication::applicationName(), text);
    }
    if (this->unitChanged)
    {
        QString unit = qvariant_cast<QString>(this->unitCombo->itemData(this->unitCombo->currentIndex()));
        settings.setValue("configuration/unit", unit);
        this->unitChanged = false;
        QString text = QString(tr("Default unit updated and will be used the next pattern load"));
        QMessageBox::information(this, QApplication::applicationName(), text);
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
QGroupBox *ConfigurationPage::SaveGroup()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName());

    QGroupBox *saveGroup = new QGroupBox(tr("Save"));

    autoSaveCheck = new QCheckBox(tr("Auto-save modified pattern"));
    bool autoSaveValue = settings.value("configuration/autosave/state", 1).toBool();
    autoSaveCheck->setChecked(autoSaveValue);

    QLabel *intervalLabel = new QLabel(tr("Interval:"));

    autoTime = new QSpinBox();
    bool ok = true;
    qint32 autoTimeValue = settings.value("configuration/autosave/time", 5).toInt(&ok);
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
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName());

    QGroupBox *langGroup = new QGroupBox(tr("Language"));
    QLabel *guiLabel = new QLabel(tr("GUI language"));
    langCombo = new QComboBox;

    // format systems language
    QString defaultLocale = QLocale::system().name();       // e.g. "de_DE"
    defaultLocale.truncate(defaultLocale.lastIndexOf('_')); // e.g. "de"
    QString checkedLocale = settings.value("configuration/locale", defaultLocale).toString();

    QString m_langPath = qApp->translationsPath();
    QDir dir(m_langPath);
    QStringList fileNames = dir.entryList(QStringList("valentina_*.qm"));

    for (int i = 0; i < fileNames.size(); ++i)
    {
        // get locale extracted by filename
        QString locale;
        locale = fileNames.at(i);                  // "valentina_de.qm"
        locale.truncate(locale.lastIndexOf('.'));   // "valentina_de"
        locale.remove(0, locale.indexOf('_') + 1);   // "de"

        QString lang = QLocale(locale).nativeLanguageName();
        QIcon ico(QString("%1/%2.png").arg("://icon/flags").arg(locale));

        langCombo->addItem(ico, lang, locale);
    }

    QIcon ico(QString("%1/%2.png").arg("://icon/flags").arg("en"));
    QString lang = QLocale("en").nativeLanguageName();
    langCombo->addItem(ico, lang, "en");

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
    bool osOptionValue = settings.value("configuration/osSeparator", 1).toBool();
    osOptionCheck->setChecked(osOptionValue);

    QHBoxLayout *separatorLayout = new QHBoxLayout;
    separatorLayout->addWidget(separatorLabel);
    separatorLayout->addWidget(osOptionCheck);

    //----------------------- Unit setup
    this->unitCombo = new QComboBox;
    QLabel *unitLabel = new QLabel(tr("Default unit"));

    QString checkedUnit = settings.value("configuration/unit", "cm").toString();

    this->unitCombo->addItem(tr("Centimeters"),"cm");
    this->unitCombo->addItem(tr("Milimiters"),"mm");
    this->unitCombo->addItem(tr("Inches"),"in");

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

    //----------------------- Unit setup

    QVBoxLayout *langLayout = new QVBoxLayout;
    langLayout->addLayout(guiLangLayout);
    langLayout->addLayout(separatorLayout);
    langLayout->addLayout(UnitLayout);
    langGroup->setLayout(langLayout);

    return langGroup;
}

//---------------------------------------------------------------------------------------------------------------------
//---------------------- Pattern Class
PatternPage::PatternPage(QWidget *parent):
    QWidget(parent), userName(0), graphOutputCheck(0), undoCount(0)
{
    QGroupBox *userGroup = UserGroup();
    QGroupBox *graphOutputGroup = GraphOutputGroup();
    QGroupBox *undoGroup = UndoGroup();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(userGroup);
    mainLayout->addWidget(graphOutputGroup);
    mainLayout->addWidget(undoGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

//---------------------------------------------------------------------------------------------------------------------
void PatternPage::Apply()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName());
    settings.setValue("pattern/user", userName->text());

    // Scene antialiasing
    settings.setValue("pattern/graphicalOutput", graphOutputCheck->isChecked());
    qApp->getSceneView()->setRenderHint(QPainter::Antialiasing, graphOutputCheck->isChecked());
    qApp->getSceneView()->setRenderHint(QPainter::SmoothPixmapTransform, graphOutputCheck->isChecked());

    /* Maximum number of commands in undo stack may only be set when the undo stack is empty, since setting it on a
     * non-empty stack might delete the command at the current index. Calling setUndoLimit() on a non-empty stack
     * prints a warning and does nothing.*/
    settings.setValue("pattern/undo", undoCount->value());
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *PatternPage::UserGroup()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName());

    QGroupBox *userGroup = new QGroupBox(tr("User"));
    QLabel *nameLabel = new QLabel(tr("User name"));

    userName = new QLineEdit;
#ifdef Q_OS_WIN
    QString user = settings.value("pattern/user", QString::fromLocal8Bit(qgetenv("USERNAME").constData())).toString();
#else
    QString user = settings.value("pattern/user", QString::fromLocal8Bit(qgetenv("USER").constData())).toString();
#endif
    userName->setText(user);

    QHBoxLayout *nameLayout = new QHBoxLayout;
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(userName);

    QVBoxLayout *userLayout = new QVBoxLayout;
    userLayout->addLayout(nameLayout);
    userGroup->setLayout(userLayout);
    return userGroup;
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *PatternPage::GraphOutputGroup()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName());

    QGroupBox *graphOutputGroup = new QGroupBox(tr("Graphical output"));

    graphOutputCheck = new QCheckBox(tr("Use antialiasing"));
    bool graphOutputValue = settings.value("pattern/graphicalOutput", 1).toBool();
    graphOutputCheck->setChecked(graphOutputValue);

    QHBoxLayout *graphLayout = new QHBoxLayout;
    graphLayout->addWidget(graphOutputCheck);

    QVBoxLayout *graphOutputLayout = new QVBoxLayout;
    graphOutputLayout->addLayout(graphLayout);
    graphOutputGroup->setLayout(graphOutputLayout);
    return graphOutputGroup;
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *PatternPage::UndoGroup()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName());

    QGroupBox *undoGroup = new QGroupBox(tr("Undo"));
    QLabel *undoLabel = new QLabel(tr("Count steps (0 - no limit)"));
    undoCount = new QSpinBox;
    undoCount->setMinimum(0);
    bool ok = true;
    qint32 count = settings.value("pattern/undo", 0).toInt(&ok);
    if (ok == false)
    {
        count = 0;
    }
    undoCount->setValue(count);

    QHBoxLayout *countLayout = new QHBoxLayout;
    countLayout->addWidget(undoLabel);
    countLayout->addWidget(undoCount);

    QVBoxLayout *undoLayout = new QVBoxLayout;
    undoLayout->addLayout(countLayout);
    undoGroup->setLayout(undoLayout);
    return undoGroup;
}


//---------------------------------------------------------------------------------------------------------------------
//---------------------- Community Class
CommunityPage::CommunityPage(QWidget *parent):
    QWidget(parent), server(nullptr), secureComm(nullptr), useProxy(nullptr), proxyAddress(nullptr),
    proxyPort(nullptr), proxyUser(nullptr), proxyPass(nullptr), username(nullptr), savePassword(nullptr),
    userpassword(nullptr)
{
    QGroupBox *serverGroup = ServerGroup();
    QGroupBox *proxyGroup = ProxyGroup();
    QGroupBox *userGroup = UserGroup();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(serverGroup);
    mainLayout->addWidget(userGroup);
    mainLayout->addWidget(proxyGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

//---------------------------------------------------------------------------------------------------------------------
void CommunityPage::Apply()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName());
    settings.setValue("community/server", this->server->text());
    settings.setValue("community/serverSecure", this->secureComm->isChecked());
    settings.setValue("community/useProxy",this->useProxy->isChecked());
    settings.setValue("community/proxyAddress",this->proxyAddress->text());
    settings.setValue("community/proxyPort",this->proxyPort->text());
    settings.setValue("community/proxyUser",this->proxyUser->text());
    settings.setValue("community/proxyPass",this->proxyPass->text());

    settings.setValue("community/username",this->username->text());
    settings.setValue("community/savePassword",this->savePassword->isChecked());
    settings.setValue("community/userpassword",this->userpassword->text());

}

//---------------------------------------------------------------------------------------------------------------------
void CommunityPage::ProxyCheckChanged()
{
    if (this->useProxy->isChecked() == false)
    {
        this->proxyAddress->setEnabled(false);
        this->proxyPort->setEnabled(false);
        this->proxyUser->setEnabled(false);
        this->proxyPass->setEnabled(false);
    }
    else
    {
        this->proxyAddress->setEnabled(true);
        this->proxyPort->setEnabled(true);
        this->proxyUser->setEnabled(true);
        this->proxyPass->setEnabled(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void CommunityPage::PasswordCheckChanged()
{
    this->userpassword->setEnabled(this->savePassword->isChecked());
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *CommunityPage::ServerGroup()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName());

    QGroupBox *ServerGroup = new QGroupBox(tr("Server"));
    QFormLayout *serverLayout = new QFormLayout;

    CommunityPage::add_lineedit(&this->server,serverLayout,
           settings.value("community/server", "community.valentina-project.org").toString(), tr("Server name/IP"));

    CommunityPage::add_checkbox(&this->secureComm,serverLayout,
           settings.value("community/serverSecure", 0).toBool(), tr("Secure connection"));

    ServerGroup->setLayout(serverLayout);
    return ServerGroup;
}

//---------------------------------------------------------------------------------------------------------------------
void CommunityPage::add_checkbox(QCheckBox** thebox, QFormLayout *layout, bool checked, QString label)
{
    QLabel *labelbox = new QLabel(label);
    (*thebox)= new QCheckBox;
    (*thebox)->setChecked(checked);
    layout->addRow(labelbox, *thebox);
}

//---------------------------------------------------------------------------------------------------------------------
void CommunityPage::add_lineedit(QLineEdit** theline, QFormLayout *layout, QString value, QString label)
{
    QLabel *labelbox = new QLabel(label);
    (*theline)= new QLineEdit;
    (*theline)->setText(value);
    layout->addRow(labelbox, *theline);
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *CommunityPage::ProxyGroup()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName());
    QGroupBox *proxyGroup = new QGroupBox(tr("Proxy settings"));

    QFormLayout *proxyLayout = new QFormLayout;

    CommunityPage::add_checkbox(&this->useProxy, proxyLayout,
         settings.value("community/useProxy", 0).toBool(), tr("Use Proxy"));

    CommunityPage::add_lineedit(&this->proxyAddress, proxyLayout,
         settings.value("community/proxyAddress", "").toString(), tr("Proxy address"));

    CommunityPage::add_lineedit(&this->proxyPort, proxyLayout,
         settings.value("community/proxyPort", "").toString(), tr("Proxy port"));

    CommunityPage::add_lineedit(&this->proxyUser, proxyLayout,
         settings.value("community/proxyUser", "").toString(), tr("Proxy user"));

    CommunityPage::add_lineedit(&this->proxyPass, proxyLayout,
         settings.value("community/proxyPass", "").toString(), tr("Proxy pass"));

    connect(this->useProxy, &QCheckBox::stateChanged, this, &CommunityPage::ProxyCheckChanged);
    this->ProxyCheckChanged();

    proxyGroup->setLayout(proxyLayout);

    return proxyGroup;
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *CommunityPage::UserGroup()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName());
    QGroupBox *userGroup = new QGroupBox(tr("User settings"));
    QFormLayout *userLayout = new QFormLayout;

    CommunityPage::add_lineedit(&this->username, userLayout,
         settings.value("community/username", "").toString(), tr("User Name"));

    CommunityPage::add_checkbox(&this->savePassword, userLayout,
         settings.value("community/savePassword", 0).toBool(), tr("Save password"));

    CommunityPage::add_lineedit(&this->userpassword, userLayout,
         settings.value("community/userpassword", "").toString(), tr("Password"));

    connect(this->savePassword, &QCheckBox::stateChanged, this, &CommunityPage::PasswordCheckChanged);
    this->PasswordCheckChanged();

    userGroup->setLayout(userLayout);

    return userGroup;
}
