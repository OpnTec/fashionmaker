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
    qApp->Settings()->SetServer(this->server->text());
    qApp->Settings()->SetServerSecure(this->secureComm->isChecked());
    qApp->Settings()->SetProxy(this->useProxy->isChecked());
    qApp->Settings()->SetProxyAddress(this->proxyAddress->text());
    qApp->Settings()->SetProxyPort(this->proxyPort->text());
    qApp->Settings()->SetProxyUser(this->proxyUser->text());
    qApp->Settings()->SetProxyPass(this->proxyPass->text());

    qApp->Settings()->SetUsername(this->username->text());
    qApp->Settings()->SetSavePassword(this->savePassword->isChecked());
    qApp->Settings()->SetUserPassword(this->userpassword->text());

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
    QGroupBox *ServerGroup = new QGroupBox(tr("Server"));
    QFormLayout *serverLayout = new QFormLayout;

    CommunityPage::add_lineedit(&this->server, serverLayout, qApp->Settings()->GetServer(), tr("Server name/IP"));

    CommunityPage::add_checkbox(&this->secureComm, serverLayout, qApp->Settings()->GetServerSecure(),
                                tr("Secure connection"));

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
    QGroupBox *proxyGroup = new QGroupBox(tr("Proxy settings"));

    QFormLayout *proxyLayout = new QFormLayout;

    CommunityPage::add_checkbox(&this->useProxy, proxyLayout, qApp->Settings()->GetProxy(), tr("Use Proxy"));
    CommunityPage::add_lineedit(&this->proxyAddress, proxyLayout, qApp->Settings()->GetProxyAddress(),
                                tr("Proxy address"));
    CommunityPage::add_lineedit(&this->proxyPort, proxyLayout, qApp->Settings()->GetProxyPort(), tr("Proxy port"));
    CommunityPage::add_lineedit(&this->proxyUser, proxyLayout, qApp->Settings()->GetProxyUser(), tr("Proxy user"));
    CommunityPage::add_lineedit(&this->proxyPass, proxyLayout, qApp->Settings()->GetProxyPass(), tr("Proxy pass"));
    connect(this->useProxy, &QCheckBox::stateChanged, this, &CommunityPage::ProxyCheckChanged);
    this->ProxyCheckChanged();

    proxyGroup->setLayout(proxyLayout);

    return proxyGroup;
}

//---------------------------------------------------------------------------------------------------------------------
QGroupBox *CommunityPage::UserGroup()
{
    QGroupBox *userGroup = new QGroupBox(tr("User settings"));
    QFormLayout *userLayout = new QFormLayout;

    CommunityPage::add_lineedit(&this->username, userLayout, qApp->Settings()->GetUsername(), tr("User Name"));
    CommunityPage::add_checkbox(&this->savePassword, userLayout, qApp->Settings()->GetSavePassword(),
                                tr("Save password"));
    CommunityPage::add_lineedit(&this->userpassword, userLayout, qApp->Settings()->GetUserPassword(),
                                tr("Password"));

    connect(this->savePassword, &QCheckBox::stateChanged, this, &CommunityPage::PasswordCheckChanged);
    this->PasswordCheckChanged();

    userGroup->setLayout(userLayout);

    return userGroup;
}
