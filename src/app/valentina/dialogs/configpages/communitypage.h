/************************************************************************
 **
 **  @file   communitypage.h
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

#ifndef COMMUNITYPAGE_H
#define COMMUNITYPAGE_H

#include <QObject>
#include <QWidget>
#include <QFormLayout>

class QCheckBox;
class QGroupBox;
class QLineEdit;
class QLabel;

class CommunityPage : public QWidget
{
    Q_OBJECT
public:
    explicit CommunityPage(QWidget *parent = nullptr);
    void      Apply();
protected:
    virtual void changeEvent(QEvent* event) Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(CommunityPage)
    // server name and https connection
    QGroupBox *serverGroup;
    QLineEdit *server;
    QCheckBox *secureComm;
    QLabel    *serverNameLabel;
    QLabel    *secureConnectionLabel;

    // proxy stuff
    QGroupBox *proxyGroup;
    QCheckBox *useProxy;
    QLineEdit *proxyAddress;
    QLineEdit *proxyPort;
    QLineEdit *proxyUser;
    QLineEdit *proxyPass;

    QLabel    *useProxyLabel;
    QLabel    *proxyAddressLabel;
    QLabel    *proxyPortLabel;
    QLabel    *proxyUserLabel;
    QLabel    *proxyPassLabel;

    // username and password
    QGroupBox *userGroup;
    QLineEdit *username;
    QCheckBox *savePassword;
    QLineEdit *userpassword;

    QLabel    *usernameLabel;
    QLabel    *savePasswordLabel;
    QLabel    *userpasswordLabel;

    static void AddCheckbox(QCheckBox** thebox, QFormLayout *layout, bool checked, QLabel* label);
    static void AddLineedit(QLineEdit** theline, QFormLayout *layout, QString value, QLabel *label);

    void ProxyCheckChanged();
    void PasswordCheckChanged();

    QGroupBox *ServerGroup();
    QGroupBox *ProxyGroup();
    QGroupBox *UserGroup();

    void RetranslateUi();
};

#endif // COMMUNITYPAGE_H
