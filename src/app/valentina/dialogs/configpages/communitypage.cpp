/************************************************************************
 **
 **  @file   communitypage.cpp
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

#include "communitypage.h"
#include "../../options.h"
#include "../../core/vapplication.h"
#include "../vmisc/vsettings.h"
#include <QGroupBox>
#include <QLabel>
#include <QSettings>
#include <QCheckBox>
#include <QLineEdit>

//---------------------------------------------------------------------------------------------------------------------
CommunityPage::CommunityPage(QWidget *parent):
    QWidget(parent),
    serverGroup(nullptr),
    server(nullptr),
    secureComm(nullptr),
    serverNameLabel(nullptr),
    secureConnectionLabel(nullptr),
    proxyGroup(nullptr),
    useProxy(nullptr),
    proxyAddress(nullptr),
    proxyPort(nullptr),
    proxyUser(nullptr),
    proxyPass(nullptr),
    useProxyLabel(nullptr),
    proxyAddressLabel(nullptr),
    proxyPortLabel(nullptr),
    proxyUserLabel(nullptr),
    proxyPassLabel(nullptr),
    userGroup(nullptr),
    username(nullptr),
    savePassword(nullptr),
    userpassword(nullptr),
    usernameLabel(nullptr),
    savePasswordLabel(nullptr),
    userpasswordLabel(nullptr)
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
    VSettings *settings = qApp->ValentinaSettings();
    settings->SetServer(this->server->text());
    settings->SetServerSecure(this->secureComm->isChecked());
    settings->SetProxy(this->useProxy->isChecked());
    settings->SetProxyAddress(this->proxyAddress->text());
    settings->SetProxyPort(this->proxyPort->text());
    settings->SetProxyUser(this->proxyUser->text());
    settings->SetProxyPass(this->proxyPass->text());

    settings->SetUsername(this->username->text());
    settings->SetSavePassword(this->savePassword->isChecked());
    settings->SetUserPassword(this->userpassword->text());
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
    serverGroup = new QGroupBox(tr("Server"));
    QFormLayout *serverLayout = new QFormLayout;
    serverLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    serverNameLabel = new QLabel(tr("Server name/IP:"));
    secureConnectionLabel = new QLabel(tr("Secure connection"));

    CommunityPage::AddLineedit(&this->server, serverLayout, qApp->ValentinaSettings()->GetServer(), serverNameLabel);

    CommunityPage::AddCheckbox(&this->secureComm, serverLayout, qApp->ValentinaSettings()->GetServerSecure(),
                                secureConnectionLabel);

    serverGroup->setLayout(serverLayout);
    return serverGroup;
}

//---------------------------------------------------------------------------------------------------------------------
void CommunityPage::AddCheckbox(QCheckBox** thebox, QFormLayout *layout, bool checked, QLabel *label)
{
    (*thebox)= new QCheckBox;
    (*thebox)->setChecked(checked);
    layout->addRow(label, *thebox);
}

//---------------------------------------------------------------------------------------------------------------------
void CommunityPage::AddLineedit(QLineEdit** theline, QFormLayout *layout, QString value, QLabel *label)
{
    (*theline)= new QLineEdit;
#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    (*theline)->setClearButtonEnabled(true);
#endif
    (*theline)->setText(value);
    layout->addRow(label, *theline);
}

//---------------------------------------------------------------------------------------------------------------------
void CommunityPage::changeEvent(QEvent *event)
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
QGroupBox *CommunityPage::ProxyGroup()
{
    proxyGroup = new QGroupBox(tr("Proxy settings"));

    QFormLayout *proxyLayout = new QFormLayout;
    proxyLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    useProxyLabel     = new QLabel(tr("Use Proxy"));
    proxyAddressLabel = new QLabel(tr("Proxy address:"));
    proxyPortLabel    = new QLabel(tr("Proxy port:"));
    proxyUserLabel    = new QLabel(tr("Proxy user:"));
    proxyPassLabel    = new QLabel(tr("Proxy pass:"));

    const VSettings *settings = qApp->ValentinaSettings();
    CommunityPage::AddCheckbox(&this->useProxy, proxyLayout, settings->GetProxy(), useProxyLabel);
    CommunityPage::AddLineedit(&this->proxyAddress, proxyLayout, settings->GetProxyAddress(),
                                proxyAddressLabel);
    CommunityPage::AddLineedit(&this->proxyPort, proxyLayout, settings->GetProxyPort(), proxyPortLabel);
    CommunityPage::AddLineedit(&this->proxyUser, proxyLayout, settings->GetProxyUser(), proxyUserLabel);
    CommunityPage::AddLineedit(&this->proxyPass, proxyLayout, settings->GetProxyPass(), proxyPassLabel);
    connect(this->useProxy, &QCheckBox::stateChanged, this, &CommunityPage::ProxyCheckChanged);
    this->ProxyCheckChanged();

    proxyGroup->setLayout(proxyLayout);

    return proxyGroup;
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *CommunityPage::UserGroup()
{
    userGroup = new QGroupBox(tr("User settings"));
    QFormLayout *userLayout = new QFormLayout;
    userLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    usernameLabel     = new QLabel(tr("User Name:"));
    savePasswordLabel = new QLabel(tr("Save password"));
    userpasswordLabel = new QLabel(tr("Password:"));

    const VSettings *settings = qApp->ValentinaSettings();
    CommunityPage::AddLineedit(&this->username, userLayout, settings->GetUsername(), usernameLabel);
    CommunityPage::AddCheckbox(&this->savePassword, userLayout, settings->GetSavePassword(), savePasswordLabel);
    CommunityPage::AddLineedit(&this->userpassword, userLayout, settings->GetUserPassword(), userpasswordLabel);

    connect(this->savePassword, &QCheckBox::stateChanged, this, &CommunityPage::PasswordCheckChanged);
    this->PasswordCheckChanged();

    userGroup->setLayout(userLayout);

    return userGroup;
}

//---------------------------------------------------------------------------------------------------------------------
void CommunityPage::RetranslateUi()
{
    serverGroup->setTitle(tr("Server"));
    serverNameLabel->setText(tr("Server name/IP:"));
    secureConnectionLabel->setText(tr("Secure connection"));

    proxyGroup->setTitle(tr("Proxy settings"));
    useProxyLabel->setText(tr("Use Proxy"));
    proxyAddressLabel->setText(tr("Proxy address:"));
    proxyPortLabel->setText(tr("Proxy port:"));
    proxyUserLabel->setText(tr("Proxy user:"));
    proxyPassLabel->setText(tr("Proxy pass:"));

    userGroup->setTitle(tr("User settings"));
    usernameLabel->setText(tr("User Name:"));
    savePasswordLabel->setText(tr("Save password"));
    userpasswordLabel->setText(tr("Password:"));
}
