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

#include <QApplication>
#include <QDate>
#include <QDir>
#include <QLocale>
#include <QMessageLogger>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QVariant>
#include <QtDebug>

#include "../vmisc/def.h"
#include "../vmisc/vmath.h"

const QString settingPathsIndividualMeasurements = QStringLiteral("paths/individual_measurements");
const QString settingPathsStandardMeasurements   = QStringLiteral("paths/standard_measurements");
const QString settingPathsTemplates              = QStringLiteral("paths/templates");

const QString settingConfigurationOsSeparator            = QStringLiteral("configuration/osSeparator");
const QString settingConfigurationAutosaveState          = QStringLiteral("configuration/autosave/state");
const QString settingConfigurationAutosaveTime           = QStringLiteral("configuration/autosave/time");
const QString settingConfigurationSendReportState        = QStringLiteral("configuration/send_report/state");
const QString settingConfigurationLocale                 = QStringLiteral("configuration/locale");
const QString settingPMSystemCode                        = QStringLiteral("configuration/pmscode");
const QString settingConfigurationUnit                   = QStringLiteral("configuration/unit");
const QString settingConfigurationConfirmItemDeletion    = QStringLiteral("configuration/confirm_item_deletion");
const QString settingConfigurationConfirmFormatRewriting = QStringLiteral("configuration/confirm_format_rewriting");
const QString settingConfigurationToolBarStyle           = QStringLiteral("configuration/tool_bar_style");

const QString settingPatternUser           = QStringLiteral("pattern/user");
const QString settingPatternUndo           = QStringLiteral("pattern/undo");
const QString settingPatternForbidFlipping = QStringLiteral("pattern/forbidFlipping");
const QString settingDoublePassmark        = QStringLiteral("pattern/doublePassmark");

const QString settingGeneralRecentFileList  = QStringLiteral("recentFileList");
const QString settingGeneralRestoreFileList = QStringLiteral("restoreFileList");
const QString settingGeneralGeometry        = QStringLiteral("geometry");
const QString settingGeneralWindowState     = QStringLiteral("windowState");
const QString settingGeneralToolbarsState   = QStringLiteral("toolbarsState");
const QString settingPreferenceDialogSize   = QStringLiteral("preferenceDialogSize");
const QString settingLatestSkippedVersion   = QStringLiteral("lastestSkippedVersion");
const QString settingDateOfLastRemind       = QStringLiteral("dateOfLastRemind");
const QString settingUserDefinedMaterials   = QStringLiteral("configuration/userDefinedMaterials");

static const QString commonIniFilename = QStringLiteral("common");

#if !defined(Q_OS_WIN)
const QString VCommonSettings::unixStandardSharePath = QStringLiteral("/usr/share/valentina");
#endif

namespace
{
//---------------------------------------------------------------------------------------------------------------------
bool SymlinkCopyDirRecursive(const QString &fromDir, const QString &toDir, bool replaceOnConflit)
{
    QDir dir;
    dir.setPath(fromDir);

    foreach (QString copyFile, dir.entryList(QDir::Files))
    {
        const QString from = fromDir + QDir::separator() + copyFile;
        const QString to = toDir + QDir::separator() + copyFile;

        if (QFile::exists(to))
        {
            if (replaceOnConflit)
            {
                if (QFile::remove(to) == false)
                {
                    return false;
                }
            }
            else
            {
                continue;
            }
        }

        if (QFile::link(from, to) == false)
        {
            return false;
        }
    }

    foreach (QString copyDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        const QString from = fromDir + QDir::separator() + copyDir;
        const QString to = toDir + QDir::separator() + copyDir;

        if (dir.mkpath(to) == false)
        {
            return false;
        }

        if (SymlinkCopyDirRecursive(from, to, replaceOnConflit) == false)
        {
            return false;
        }
    }

    return true;
}
}

//---------------------------------------------------------------------------------------------------------------------
VCommonSettings::VCommonSettings(Format format, Scope scope, const QString &organization,
                            const QString &application, QObject *parent)
    :QSettings(format, scope, organization, application, parent)
{}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::SharePath(const QString &shareItem)
{
#ifdef Q_OS_WIN
    return QApplication::applicationDirPath() + shareItem;
#elif defined(Q_OS_MAC)
    QDir dirBundle(QApplication::applicationDirPath() + QStringLiteral("/../Resources") + shareItem);
    if (dirBundle.exists())
    {
        return dirBundle.absolutePath();
    }
    else
    {
        QDir appDir = QDir(qApp->applicationDirPath());
        appDir.cdUp();
        appDir.cdUp();
        appDir.cdUp();
        QDir dir(appDir.absolutePath() + shareItem);
        if (dir.exists())
        {
            return dir.absolutePath();
        }
        else
        {
            return VCommonSettings::unixStandardSharePath + shareItem;
        }
    }
#else // Unix
#ifdef QT_DEBUG
    return QApplication::applicationDirPath() + shareItem;
#else
    QDir dir(QApplication::applicationDirPath() + shareItem);
    if (dir.exists())
    {
        return dir.absolutePath();
    }
    else
    {
        return VCommonSettings::unixStandardSharePath + shareItem;
    }
#endif
#endif
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::StandardTablesPath()
{
    return SharePath(QStringLiteral("/tables/standard"));
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::StandardTemplatesPath()
{
    return SharePath(QStringLiteral("/tables/templates"));
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::PrepareStandardTemplates(const QString & currentPath)
{
    QDir standardPath(VCommonSettings::StandardTemplatesPath());
    const QDir localdata (VCommonSettings::GetDefPathTemplate());
    if (currentPath == VCommonSettings::GetDefPathTemplate() && standardPath.exists() && not localdata.exists())
    {
        if (localdata.mkpath("."))
        {
            SymlinkCopyDirRecursive(VCommonSettings::StandardTemplatesPath(), VCommonSettings::GetDefPathTemplate(),
                                    false);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::PrepareStandardTables(const QString &currentPath)
{
    QDir standardPath(VCommonSettings::StandardTablesPath());
    const QDir localdata (VCommonSettings::GetDefPathStandardMeasurements());
    if (currentPath == VCommonSettings::GetDefPathStandardMeasurements()
            && standardPath.exists()
            && not localdata.exists())
    {
        if (localdata.mkpath("."))
        {
            SymlinkCopyDirRecursive(VCommonSettings::StandardTablesPath(),
                                    VCommonSettings::GetDefPathStandardMeasurements(), false);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::GetDefPathIndividualMeasurements()
{
    return QDir::homePath() + QLatin1String("/valentina/") + tr("measurements") + QLatin1String("/") + tr("individual");
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::GetPathIndividualMeasurements() const
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), commonIniFilename);
    return settings.value(settingPathsIndividualMeasurements, GetDefPathIndividualMeasurements()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetPathIndividualMeasurements(const QString &value)
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), commonIniFilename);
    settings.setValue(settingPathsIndividualMeasurements, value);
    settings.sync();
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::GetDefPathStandardMeasurements()
{
    return QDir::homePath() + QLatin1String("/valentina/") + tr("measurements") + QLatin1String("/") + tr("multisize");
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::GetPathStandardMeasurements() const
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), commonIniFilename);
    return settings.value(settingPathsStandardMeasurements, GetDefPathStandardMeasurements()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetPathStandardMeasurements(const QString &value)
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), commonIniFilename);
    settings.setValue(settingPathsStandardMeasurements, value);
    settings.sync();
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::GetDefPathTemplate()
{
    return QDir::homePath() + QLatin1String("/valentina/") + tr("templates");
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::GetPathTemplate() const
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), commonIniFilename);
    return settings.value(settingPathsTemplates, GetDefPathTemplate()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetPathTemplate(const QString &value)
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), commonIniFilename);
    settings.setValue(settingPathsTemplates, value);
    settings.sync();
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommonSettings::GetOsSeparator() const
{
    return value(settingConfigurationOsSeparator, 1).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetOsSeparator(const bool &value)
{
    setValue(settingConfigurationOsSeparator, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommonSettings::GetAutosaveState() const
{
    return value(settingConfigurationAutosaveState, 1).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetAutosaveState(const bool &value)
{
    setValue(settingConfigurationAutosaveState, value);
}

//---------------------------------------------------------------------------------------------------------------------
int VCommonSettings::GetAutosaveTime() const
{
    bool ok = false;
    int val = value(settingConfigurationAutosaveTime, 1).toInt(&ok);
    if (ok == false)
    {
        qDebug()<<"Could not convert value"<<value(settingConfigurationAutosaveTime, 1)
               <<"to int. Return default value for autosave time"<<1<<"minutes.";
        val = 1;
    }
    return val;
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetAutosaveTime(const int &value)
{
    setValue(settingConfigurationAutosaveTime, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommonSettings::GetSendReportState() const
{
    return value(settingConfigurationSendReportState, 1).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetSendReportState(const bool &value)
{
    setValue(settingConfigurationSendReportState, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::GetLocale() const
{
    return value(settingConfigurationLocale, QLocale().name()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetLocale(const QString &value)
{
    setValue(settingConfigurationLocale, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::GetPMSystemCode() const
{
    return value(settingPMSystemCode, p0_S).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetPMSystemCode(const QString &value)
{
    setValue(settingPMSystemCode, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::GetUnit() const
{
    return value(settingConfigurationUnit, "cm").toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetUnit(const QString &value)
{
    setValue(settingConfigurationUnit, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommonSettings::GetConfirmItemDelete() const
{
    return value(settingConfigurationConfirmItemDeletion, 1).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetConfirmItemDelete(const bool &value)
{
    setValue(settingConfigurationConfirmItemDeletion, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommonSettings::GetConfirmFormatRewriting() const
{
    return value(settingConfigurationConfirmFormatRewriting, 1).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetConfirmFormatRewriting(const bool &value)
{
    setValue(settingConfigurationConfirmFormatRewriting, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommonSettings::GetToolBarStyle() const
{
    return value(settingConfigurationToolBarStyle, 1).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetToolBarStyle(const bool &value)
{
    setValue(settingConfigurationToolBarStyle, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::GetUser() const
{
    QString user;
#ifdef Q_OS_WIN
    user = value(SettingPatternUser, QString::fromLocal8Bit(qgetenv("USERNAME").constData())).toString();
#else
    user = value(settingPatternUser, QString::fromLocal8Bit(qgetenv("USER").constData())).toString();
#endif
    return user;
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetUser(const QString &value)
{
    setValue(settingPatternUser, value);
}

//---------------------------------------------------------------------------------------------------------------------
int VCommonSettings::GetUndoCount() const
{
    bool ok = false;
    int val = value(settingPatternUndo, 0).toInt(&ok);
    if (ok == false)
    {
        qDebug()<<"Could not convert value"<<value(settingPatternUndo, 0)
               <<"to int. Return default value for undo counts 0 (no limit).";
        val = 0;
    }
    return val;
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetUndoCount(const int &value)
{
    setValue(settingPatternUndo, value);
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VCommonSettings::GetRecentFileList() const
{
    const QStringList files = value(settingGeneralRecentFileList).toStringList();
    QStringList cleared;

    for (int i = 0; i < files.size(); ++i)
    {
        if (QFileInfo(files.at(i)).exists())
        {
            cleared.append(files.at(i));
        }
    }

    return cleared;
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetRecentFileList(const QStringList &value)
{
    setValue(settingGeneralRecentFileList, value);
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VCommonSettings::GetRestoreFileList() const
{
    return value(settingGeneralRestoreFileList).toStringList();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetRestoreFileList(const QStringList &value)
{
    setValue(settingGeneralRestoreFileList, value);
}

//---------------------------------------------------------------------------------------------------------------------
QByteArray VCommonSettings::GetGeometry() const
{
    return value(settingGeneralGeometry).toByteArray();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetGeometry(const QByteArray &value)
{
    setValue(settingGeneralGeometry, value);
}

//---------------------------------------------------------------------------------------------------------------------
QByteArray VCommonSettings::GetWindowState() const
{
    return value(settingGeneralWindowState).toByteArray();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetWindowState(const QByteArray &value)
{
    setValue(settingGeneralWindowState, value);
}

//---------------------------------------------------------------------------------------------------------------------
QByteArray VCommonSettings::GetToolbarsState() const
{
    return value(settingGeneralToolbarsState).toByteArray();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetToolbarsState(const QByteArray &value)
{
    setValue(settingGeneralToolbarsState, value);
}

//---------------------------------------------------------------------------------------------------------------------
QSize VCommonSettings::GetPreferenceDialogSize() const
{
    return value(settingPreferenceDialogSize, QSize(0, 0)).toSize();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetPreferenceDialogSize(const QSize& sz)
{
    setValue(settingPreferenceDialogSize, sz);
}

//---------------------------------------------------------------------------------------------------------------------
int VCommonSettings::GetLatestSkippedVersion() const
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), commonIniFilename);
    return settings.value(settingLatestSkippedVersion, 0x0).toInt();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetLatestSkippedVersion(int value)
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), commonIniFilename);
    settings.setValue(settingLatestSkippedVersion, value);
    settings.sync();
}

//---------------------------------------------------------------------------------------------------------------------
QDate VCommonSettings::GetDateOfLastRemind() const
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), commonIniFilename);
    return settings.value(settingDateOfLastRemind, QDate(1900, 1, 1)).toDate();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetDateOfLastRemind(const QDate &date)
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), commonIniFilename);
    settings.setValue(settingDateOfLastRemind, date);
    settings.sync();
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VCommonSettings::GetUserDefinedMaterials() const
{
    return value(settingUserDefinedMaterials).toStringList();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::AddUserDefinedMaterial(QString qsMaterial)
{
    QStringList qsl = GetUserDefinedMaterials();
    qsl << qsMaterial;
    setValue(settingUserDefinedMaterials, qsl);
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::ClearUserDefinedMaterial()
{
    QStringList qsl;
    setValue(settingUserDefinedMaterials, qsl);
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommonSettings::GetForbidWorkpieceFlipping() const
{
    return value(settingPatternForbidFlipping, false).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetForbidWorkpieceFlipping(bool value)
{
    setValue(settingPatternForbidFlipping, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommonSettings::IsDoublePassmark() const
{
    return value(settingDoublePassmark, false).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetDoublePassmark(bool value)
{
    setValue(settingDoublePassmark, value);
}
