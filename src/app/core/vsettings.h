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

    bool GetOsSeparator();
    void SetOsSeparator(const bool &value);

    bool GetAutosaveState();
    void SetAutosaveState(const bool &value);

    int  GetAutosaveTime();
    void SetAutosaveTime(const int &value);

    bool GetSendReportState();
    void SetSendReportState(const bool &value);

    QString GetLocale();
    void SetLocale(const QString &value);

    QString GetUnit();
    void SetUnit(const QString &value);

    QString GetLabelLanguage();
    void SetLabelLanguage(const QString &value);

    QString GetPathIndividualMeasurements();
    void SetPathIndividualMeasurements(const QString &value);

    QString GetPathPattern();
    void SetPathPattern(const QString &value);

    QString GetUser();
    void SetUser(const QString &value);

    bool GetGraphicalOutput();
    void SetGraphicalOutput(const bool &value);

    int  GetUndoCount();
    void SetUndoCount(const int &value);

    QStringList GetRecentFileList();
    void SetRecentFileList(const QStringList &value);

    QStringList GetRestoreFileList();
    void SetRestoreFileList(const QStringList &value);

    QByteArray GetGeometry();
    void SetGeometry(const QByteArray &value);

    QByteArray GetWindowState();
    void SetWindowState(const QByteArray &value);

    QString GetServer();
    void SetServer(const QString &value);

    bool GetServerSecure();
    void SetServerSecure(const bool &value);

    bool GetProxy();
    void SetProxy(const bool &value);

    QString GetProxyAddress();
    void SetProxyAddress(const QString &value);

    QString GetProxyPort();
    void SetProxyPort(const QString &value);

    QString GetProxyUser();
    void SetProxyUser(const QString &value);

    QString GetProxyPass();
    void SetProxyPass(const QString &value);

    QString GetUsername();
    void SetUsername(const QString &value);

    bool GetSavePassword();
    void SetSavePassword(const bool &value);

    QString GetUserPassword();
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
