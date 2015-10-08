/************************************************************************
 **
 **  @file   vsettings.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 11, 2014
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

#ifndef VSETTINGS_H
#define VSETTINGS_H

#include "vcommonsettings.h"
#include "../vlayout/vbank.h"

class VSettings : public VCommonSettings
{
    Q_OBJECT
public:
    VSettings(Format format, Scope scope, const QString &organization, const QString &application = QString(),
              QObject *parent = 0);

    virtual QString StandardTablesPath() const Q_DECL_OVERRIDE;

    QString GetLabelLanguage() const;
    void SetLabelLanguage(const QString &value);

    QString GetPathPattern() const;
    void SetPathPattern(const QString &value);

    QString GetPathLayout() const;
    void SetPathLayout(const QString &value);

    bool GetGraphicalOutput() const;
    void SetGraphicalOutput(const bool &value);

    QString GetServer() const;
    void SetServer(const QString &value);

    bool GetServerSecure() const;
    void SetServerSecure(const bool &value);

    bool GetProxy() const;
    void SetProxy(const bool &value);

    QString GetProxyAddress() const;
    void SetProxyAddress(const QString &value);

    QString GetProxyPort() const;
    void SetProxyPort(const QString &value);

    QString GetProxyUser() const;
    void SetProxyUser(const QString &value);

    QString GetProxyPass() const;
    void SetProxyPass(const QString &value);

    QString GetUsername() const;
    void SetUsername(const QString &value);

    bool GetSavePassword() const;
    void SetSavePassword(const bool &value);

    QString GetUserPassword() const;
    void SetUserPassword(const QString &value);

    int GetLayoutPaperHeight() const;
    void SetLayoutPaperHeight(int value);

    int GetLayoutPaperWidth() const;
    void SetLayoutPaperWidth(int value);

    unsigned int GetLayoutShift() const;
    static unsigned int GetDefLayoutShift();
    void SetLayoutShift(unsigned int value);

    unsigned int GetLayoutWidth() const;
    static unsigned int GetDefLayoutWidth();
    void SetLayoutWidth(unsigned int value);

    Cases GetLayoutGroup() const;
    static Cases GetDefLayoutGroup();
    void SetLayoutGroup(const Cases &value);

    bool GetLayoutRotate() const;
    static bool GetDefLayoutRotate();
    void SetLayoutRotate(bool value);

    int GetLayoutRotationIncrease() const;
    static int GetDefLayoutRotationIncrease();
    void SetLayoutRotationIncrease(int value);

    bool GetLayoutAutoCrop() const;
    void SetLayoutAutoCrop(bool value);

    bool GetLayoutSaveLength() const;
    void SetLayoutSaveLength(bool value);

    bool GetLayoutUnitePages() const;
    void SetLayoutUnitePages(bool value);

private:
    Q_DISABLE_COPY(VSettings)
    static const QString SettingConfigurationLabelLanguage;

    static const QString SettingPathsPattern;
    static const QString SettingPathsLayout;

    static const QString SettingPatternGraphicalOutput;

    static const QString SettingCommunityServer;
    static const QString SettingCommunityServerSecure;
    static const QString SettingCommunityUseProxy;
    static const QString SettingCommunityProxyAddress;
    static const QString SettingCommunityProxyPort;
    static const QString SettingCommunityProxyUser;
    static const QString SettingCommunityProxyPass;
    static const QString SettingCommunityUsername;
    static const QString SettingCommunitySavePassword;
    static const QString SettingCommunityUserPassword;

    static const QString SettingLayoutWidth;
    static const QString SettingLayoutSorting;
    static const QString SettingLayoutPaperHeight;
    static const QString SettingLayoutPaperWidth;
    static const QString SettingLayoutShift;
    static const QString SettingLayoutRotate;
    static const QString SettingLayoutRotationIncrease;
    static const QString SettingLayoutAutoCrop;
    static const QString SettingLayoutSaveLength;
    static const QString SettingLayoutUnitePages;
};

#endif // VSETTINGS_H
