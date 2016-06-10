/************************************************************************
 **
 **  @file   vcommonsettings.cpp
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

#include "vcommonsettings.h"

#include <QDir>
#include <QDebug>
#include <QLocale>
#include <QApplication>

#include "../ifc/ifcdef.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
#   include "../vmisc/vmath.h"
#else
#   include <QtMath>
#endif

const QString VCommonSettings::SettingPathsIndividualMeasurements     = QStringLiteral("paths/individual_measurements");
const QString VCommonSettings::SettingPathsStandardMeasurements       = QStringLiteral("paths/standard_measurements");
const QString VCommonSettings::SettingPathsTemplates                  = QStringLiteral("paths/templates");

const QString VCommonSettings::SettingConfigurationOsSeparator         = QStringLiteral("configuration/osSeparator");
const QString VCommonSettings::SettingConfigurationAutosaveState       = QStringLiteral("configuration/autosave/state");
const QString VCommonSettings::SettingConfigurationAutosaveTime        = QStringLiteral("configuration/autosave/time");
const QString VCommonSettings::SettingConfigurationSendReportState  = QStringLiteral("configuration/send_report/state");
const QString VCommonSettings::SettingConfigurationLocale              = QStringLiteral("configuration/locale");
const QString VCommonSettings::SettingPMSystemCode                     = QStringLiteral("configuration/pmscode");
const QString VCommonSettings::SettingConfigurationUnit                = QStringLiteral("configuration/unit");
const QString VCommonSettings::SettingConfigurationConfirmItemDeletion
                                                                = QStringLiteral("configuration/confirm_item_deletion");
const QString VCommonSettings::SettingConfigurationToolBarStyle        = QStringLiteral("configuration/tool_bar_style");

const QString VCommonSettings::SettingPatternUser                      = QStringLiteral("pattern/user");
const QString VCommonSettings::SettingPatternUndo                      = QStringLiteral("pattern/undo");

const QString VCommonSettings::SettingGeneralRecentFileList            = QStringLiteral("recentFileList");
const QString VCommonSettings::SettingGeneralRestoreFileList           = QStringLiteral("restoreFileList");
const QString VCommonSettings::SettingGeneralGeometry                  = QStringLiteral("geometry");
const QString VCommonSettings::SettingGeneralWindowState               = QStringLiteral("windowState");
const QString VCommonSettings::SettingGeneralToolbarsState             = QStringLiteral("toolbarsState");

//---------------------------------------------------------------------------------------------------------------------
VCommonSettings::VCommonSettings(Format format, Scope scope, const QString &organization,
                            const QString &application, QObject *parent)
    :QSettings(format, scope, organization, application, parent)
{}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::StandardTablesPath() const
{
    const QString stPath = QStringLiteral("/tables/standard");
#ifdef Q_OS_WIN
    return QApplication::applicationDirPath() + stPath;
#elif defined(Q_OS_MAC)
    QDir dirBundle(QApplication::applicationDirPath() + QStringLiteral("/../Resources") + stPath);
    if (dirBundle.exists())
    {
        return dirBundle.absolutePath();
    }
    else
    {
        QDir dir(QApplication::applicationDirPath() + stPath);
        if (dir.exists())
        {
            return dir.absolutePath();
        }
        else
        {
            return QStringLiteral("/usr/share/valentina/tables/standard");
        }
    }
#else // Unix
    #ifdef QT_DEBUG
        return QApplication::applicationDirPath() + stPath;
    #else
        QDir dir(QApplication::applicationDirPath() + stPath);
        if (dir.exists())
        {
            return dir.absolutePath();
        }
        else
        {
            return QStringLiteral("/usr/share/valentina/tables/standard");
        }
    #endif
#endif
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::GetPathIndividualMeasurements() const
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), "common");
    return settings.value(SettingPathsIndividualMeasurements, QDir::homePath()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetPathIndividualMeasurements(const QString &value)
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), "common");
    settings.setValue(SettingPathsIndividualMeasurements, value);
    settings.sync();
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::GetPathStandardMeasurements() const
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), "common");
    return settings.value(SettingPathsStandardMeasurements, StandardTablesPath()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetPathStandardMeasurements(const QString &value)
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), "common");
    settings.setValue(SettingPathsStandardMeasurements, value);
    settings.sync();
}


//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::GetPathTemplate() const
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), "common");
    return settings.value(SettingPathsTemplates, TemplatesPath()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetPathTemplate(const QString &value)
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), "common");
    settings.setValue(SettingPathsTemplates, value);
    settings.sync();
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::TemplatesPath() const
{
    const QString stPath = QStringLiteral("/tables/templates");
    const QString unixFullPath = QStringLiteral("/usr/share/valentina/tables/templates");
#ifdef Q_OS_WIN
    return QApplication::applicationDirPath() + stPath;
#elif defined(Q_OS_MAC)
    QDir dirBundle(QApplication::applicationDirPath() + QStringLiteral("/../Resources") + stPath);
    if (dirBundle.exists())
    {
        return dirBundle.absolutePath();
    }
    else
    {
        QDir dir(QApplication::applicationDirPath() + stPath);
        if (dir.exists())
        {
            return dir.absolutePath();
        }
        else
        {
            return unixFullPath;
        }
    }
#else // Unix
    #ifdef QT_DEBUG
        Q_UNUSED(unixFullPath);
        return QApplication::applicationDirPath() + stPath;
    #else
        QDir dir(QApplication::applicationDirPath() + stPath);
        if (dir.exists())
        {
            return dir.absolutePath();
        }
        else
        {
            return unixFullPath;
        }
    #endif
#endif
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommonSettings::GetOsSeparator() const
{
    return value(SettingConfigurationOsSeparator, 1).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetOsSeparator(const bool &value)
{
    setValue(SettingConfigurationOsSeparator, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommonSettings::GetAutosaveState() const
{
    return value(SettingConfigurationAutosaveState, 1).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetAutosaveState(const bool &value)
{
    setValue(SettingConfigurationAutosaveState, value);
}

//---------------------------------------------------------------------------------------------------------------------
int VCommonSettings::GetAutosaveTime() const
{
    bool ok = false;
    int val = value(SettingConfigurationAutosaveTime, 1).toInt(&ok);
    if (ok == false)
    {
        qDebug()<<"Could not convert value"<<value(SettingConfigurationAutosaveTime, 1)
               <<"to int. Return default value for autosave time"<<1<<"minutes.";
        val = 1;
    }
    return val;
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetAutosaveTime(const int &value)
{
    setValue(SettingConfigurationAutosaveTime, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommonSettings::GetSendReportState() const
{
    return value(SettingConfigurationSendReportState, 1).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetSendReportState(const bool &value)
{
    setValue(SettingConfigurationSendReportState, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::GetLocale() const
{
    return value(SettingConfigurationLocale, QLocale::system().name()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetLocale(const QString &value)
{
    setValue(SettingConfigurationLocale, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::GetPMSystemCode() const
{
    return value(SettingPMSystemCode, p0_S).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetPMSystemCode(const QString &value)
{
    setValue(SettingPMSystemCode, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::GetUnit() const
{
    return value(SettingConfigurationUnit, "cm").toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetUnit(const QString &value)
{
    setValue(SettingConfigurationUnit, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommonSettings::GetConfirmItemDelete() const
{
    return value(SettingConfigurationConfirmItemDeletion, 1).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetConfirmItemDelete(const bool &value)
{
    setValue(SettingConfigurationConfirmItemDeletion, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommonSettings::GetToolBarStyle() const
{
    return value(SettingConfigurationToolBarStyle, 1).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetToolBarStyle(const bool &value)
{
    setValue(SettingConfigurationToolBarStyle, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::GetUser() const
{
    QString user;
#ifdef Q_OS_WIN
    user = value(SettingPatternUser, QString::fromLocal8Bit(qgetenv("USERNAME").constData())).toString();
#else
    user = value(SettingPatternUser, QString::fromLocal8Bit(qgetenv("USER").constData())).toString();
#endif
    return user;
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetUser(const QString &value)
{
    setValue(SettingPatternUser, value);
}

//---------------------------------------------------------------------------------------------------------------------
int VCommonSettings::GetUndoCount() const
{
    bool ok = false;
    int val = value(SettingPatternUndo, 0).toInt(&ok);
    if (ok == false)
    {
        qDebug()<<"Could not convert value"<<value(SettingPatternUndo, 0)
               <<"to int. Return default value for undo counts 0 (no limit).";
        val = 0;
    }
    return val;
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetUndoCount(const int &value)
{
    setValue(SettingPatternUndo, value);
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VCommonSettings::GetRecentFileList() const
{
    return value(SettingGeneralRecentFileList).toStringList();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetRecentFileList(const QStringList &value)
{
    setValue(SettingGeneralRecentFileList, value);
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VCommonSettings::GetRestoreFileList() const
{
    return value(SettingGeneralRestoreFileList).toStringList();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetRestoreFileList(const QStringList &value)
{
    setValue(SettingGeneralRestoreFileList, value);
}

//---------------------------------------------------------------------------------------------------------------------
QByteArray VCommonSettings::GetGeometry() const
{
    return value(SettingGeneralGeometry).toByteArray();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetGeometry(const QByteArray &value)
{
    setValue(SettingGeneralGeometry, value);
}

//---------------------------------------------------------------------------------------------------------------------
QByteArray VCommonSettings::GetWindowState() const
{
    return value(SettingGeneralWindowState).toByteArray();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetWindowState(const QByteArray &value)
{
    setValue(SettingGeneralWindowState, value);
}

//---------------------------------------------------------------------------------------------------------------------
QByteArray VCommonSettings::GetToolbarsState() const
{
    return value(SettingGeneralToolbarsState).toByteArray();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetToolbarsState(const QByteArray &value)
{
    setValue(SettingGeneralToolbarsState, value);
}
