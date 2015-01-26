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

#ifndef VSETTINGS_H
#define VSETTINGS_H

#include <QSettings>

class VSettings : public QSettings
{
    Q_OBJECT
public:
    VSettings(Format format, Scope scope, const QString &organization, const QString &application = QString(),
              QObject *parent = 0);

    bool GetOsSeparator() const;
    void SetOsSeparator(const bool &value);

    bool GetAutosaveState() const;
    void SetAutosaveState(const bool &value);

    int  GetAutosaveTime() const;
    void SetAutosaveTime(const int &value);

    bool GetSendReportState() const;
    void SetSendReportState(const bool &value);

    QString GetLocale() const;
    void SetLocale(const QString &value);

    QString GetUnit() const;
    void SetUnit(const QString &value);

    QString GetLabelLanguage() const;
    void SetLabelLanguage(const QString &value);

    QString GetPathIndividualMeasurements() const;
    void SetPathIndividualMeasurements(const QString &value);

    QString GetPathPattern() const;
    void SetPathPattern(const QString &value);

    QString GetPathLayout() const;
    void SetPathLayout(const QString &value);

    QString GetUser() const;
    void SetUser(const QString &value);

    bool GetGraphicalOutput() const;
    void SetGraphicalOutput(const bool &value);

    int  GetUndoCount() const;
    void SetUndoCount(const int &value);

    QStringList GetRecentFileList() const;
    void SetRecentFileList(const QStringList &value);

    QStringList GetRestoreFileList() const;
    void SetRestoreFileList(const QStringList &value);

    QByteArray GetGeometry() const;
    void SetGeometry(const QByteArray &value);

    QByteArray GetWindowState() const;
    void SetWindowState(const QByteArray &value);

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

private:
    Q_DISABLE_COPY(VSettings)
    static const QString SettingConfigurationOsSeparator;
    static const QString SettingConfigurationAutosaveState;
    static const QString SettingConfigurationAutosaveTime;
    static const QString SettingConfigurationSendReportState;
    static const QString SettingConfigurationLocale;
    static const QString SettingConfigurationUnit;
    static const QString SettingConfigurationLabelLanguage;

    static const QString SettingPathsIndividualMeasurements;
    static const QString SettingPathsPattern;
    static const QString SettingPathsLayout;

    static const QString SettingPatternUser;
    static const QString SettingPatternGraphicalOutput;
    static const QString SettingPatternUndo;

    static const QString SettingGeneralRecentFileList;
    static const QString SettingGeneralRestoreFileList;
    static const QString SettingGeneralGeometry;
    static const QString SettingGeneralWindowState;

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
};

#endif // VSETTINGS_H
