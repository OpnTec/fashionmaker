/************************************************************************
 **
 **  @file   vcommonsettings.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 7, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef VCOMMONSETTINGS_H
#define VCOMMONSETTINGS_H

#include <QByteArray>
#include <QMetaObject>
#include <QObject>
#include <QSettings>
#include <QSize>
#include <QString>
#include <QStringList>
#include <QtGlobal>

#include "../vlayout/vbank.h"

class QDate;

class VCommonSettings : public QSettings
{
    Q_OBJECT
public:
    VCommonSettings(Format format, Scope scope, const QString &organization, const QString &application = QString(),
              QObject *parent = 0);

    QString StandardTablesPath() const;
    QString TemplatesPath() const;

    QString GetPathIndividualMeasurements() const;
    void SetPathIndividualMeasurements(const QString &value);

    QString GetPathStandardMeasurements() const;
    void SetPathStandardMeasurements(const QString &value);

    QString GetPathTemplate() const;
    void SetPathTemplate(const QString &value);

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

    QString GetPMSystemCode() const;
    void SetPMSystemCode(const QString &value);

    QString GetUnit() const;
    void SetUnit(const QString &value);

    bool GetConfirmItemDelete() const;
    void SetConfirmItemDelete(const bool &value);

    bool GetToolBarStyle() const;
    void SetToolBarStyle(const bool &value);

    QString GetUser() const;
    void SetUser(const QString &value);

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

    QByteArray GetToolbarsState() const;
    void SetToolbarsState(const QByteArray &value);

    QSize GetPreferenceDialogSize() const;
    void SetPreferenceDialogSize(const QSize& sz);

    int  GetLatestSkippedVersion() const;
    void SetLatestSkippedVersion(int value);

    QDate GetDateOfLastRemind() const;
    void  SetDateOfLastRemind(const QDate &date);

    QStringList GetUserDefinedMaterials() const;
    void AddUserDefinedMaterial(QString qsMaterial);
    void ClearUserDefinedMaterial();

private:
    Q_DISABLE_COPY(VCommonSettings)

    static const QString SettingPathsIndividualMeasurements;
    static const QString SettingPathsStandardMeasurements;
    static const QString SettingPathsTemplates;

    static const QString SettingConfigurationOsSeparator;
    static const QString SettingConfigurationAutosaveState;
    static const QString SettingConfigurationAutosaveTime;
    static const QString SettingConfigurationSendReportState;
    static const QString SettingConfigurationLocale;
    static const QString SettingPMSystemCode;
    static const QString SettingConfigurationUnit;
    static const QString SettingConfigurationConfirmItemDeletion;
    static const QString SettingConfigurationToolBarStyle;

    static const QString SettingPatternUser;
    static const QString SettingPatternUndo;

    static const QString SettingGeneralRecentFileList;
    static const QString SettingGeneralRestoreFileList;
    static const QString SettingGeneralGeometry;
    static const QString SettingGeneralWindowState;
    static const QString SettingGeneralToolbarsState;
    static const QString SettingPreferenceDialogSize;
    static const QString SettingLatestSkippedVersion;
    static const QString SettingDateOfLastRemind;
    static const QString SettingUserDefinedMaterials;
};

#endif // VCOMMONSETTINGS_H
