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
#include <QTextCodec>
#include <QFont>

#include "../vmisc/def.h"
#include "../vmisc/vmath.h"
#include "../vpatterndb/pmsystems.h"

const QString settingPathsIndividualMeasurements = QStringLiteral("paths/individual_measurements");
const QString settingPathsMultisizeMeasurements  = QStringLiteral("paths/standard_measurements");
const QString settingPathsTemplates              = QStringLiteral("paths/templates");
const QString settingPathsLabelTemplate          = QStringLiteral("paths/labelTemplate");

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

const QString settingPatternUser                    = QStringLiteral("pattern/user");
const QString settingPatternUndo                    = QStringLiteral("pattern/undo");
const QString settingPatternForbidFlipping          = QStringLiteral("pattern/forbidFlipping");
const QString settingPatternHideMainPath            = QStringLiteral("pattern/hideMainPath");
const QString settingDoublePassmark                 = QStringLiteral("pattern/doublePassmark");
const QString settingPatternDefaultSeamAllowance    = QStringLiteral("pattern/defaultSeamAllowance");
const QString settingLabelFont                      = QStringLiteral("pattern/labelFont");

const QString settingGeneralRecentFileList       = QStringLiteral("recentFileList");
const QString settingGeneralRestoreFileList      = QStringLiteral("restoreFileList");
const QString settingGeneralGeometry             = QStringLiteral("geometry");
const QString settingGeneralWindowState          = QStringLiteral("windowState");
const QString settingGeneralToolbarsState        = QStringLiteral("toolbarsState");
const QString settingPreferenceDialogSize        = QStringLiteral("preferenceDialogSize");
const QString settingToolSeamAllowanceDialogSize = QStringLiteral("toolSeamAllowanceDialogSize");
const QString settingIncrementsDialogSize        = QStringLiteral("toolIncrementsDialogSize");
const QString settingFormulaWizardDialogSize     = QStringLiteral("formulaWizardDialogSize");
const QString settingLatestSkippedVersion        = QStringLiteral("lastestSkippedVersion");
const QString settingDateOfLastRemind            = QStringLiteral("dateOfLastRemind");
const QString settingUserDefinedMaterials        = QStringLiteral("configuration/userDefinedMaterials");

const QString settingCSVWithHeader = QStringLiteral("csv/withHeader");
const QString settingCSVCodec      = QStringLiteral("csv/withCodec");
const QString settingCSVSeparator  = QStringLiteral("csv/withSeparator");

static const QString commonIniFilename = QStringLiteral("common");

#if !defined(Q_OS_WIN)
const QString VCommonSettings::unixStandardSharePath = QStringLiteral("/usr/share/valentina");
#endif

namespace
{
//---------------------------------------------------------------------------------------------------------------------
void SymlinkCopyDirRecursive(const QString &fromDir, const QString &toDir, bool replaceOnConflit)
{
    QDir dir;
    dir.setPath(fromDir);

    foreach (QString copyFile, dir.entryList(QDir::Files))
    {
        const QString from = fromDir + QDir::separator() + copyFile;
        QString to = toDir + QDir::separator() + copyFile;

#ifdef Q_OS_WIN
        {
            // To fix issue #702 check each not symlink if it is actually broken symlink.
            // Also trying to mimic Unix symlink. If a file eaxists do not create a symlink and remove it if exists.
            QFile fileTo(to);
            if (fileTo.exists())
            {
                if (not fileTo.rename(to + QLatin1String(".lnk")))
                {
                    QFile::remove(to + QLatin1String(".lnk"));
                    fileTo.rename(to + QLatin1String(".lnk"));
                }

                QFileInfo info(to + QLatin1String(".lnk"));
                if (info.symLinkTarget().isEmpty())
                {
                    fileTo.copy(to);
                    fileTo.remove();
                    continue; // The file already exists, skip creating shortcut
                }
            }
        }

        to = to + QLatin1String(".lnk");
#endif

        if (QFile::exists(to))
        {
            if (replaceOnConflit)
            {
                QFile::remove(to);
            }
            else
            {
                continue;
            }
        }

        QFile::link(from, to);
    }

    foreach (QString copyDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        const QString from = fromDir + QDir::separator() + copyDir;
        const QString to = toDir + QDir::separator() + copyDir;

        if (dir.mkpath(to) == false)
        {
            return;
        }

        SymlinkCopyDirRecursive(from, to, replaceOnConflit);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString PrepareStandardFiles(const QString &currentPath, const QString &standardPath, const QString &defPath)
{
    QDir standardPathDir(standardPath);
    QDir currentPathDir(currentPath);
    if ((currentPath == defPath || not currentPathDir.exists()) && standardPathDir.exists())
    {
        const QDir localdata (defPath);
        if (localdata.mkpath("."))
        {
            SymlinkCopyDirRecursive(standardPath, defPath, false);
        }
        return defPath;
    }
    return currentPath;
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
    return QCoreApplication::applicationDirPath() + shareItem;
#elif defined(Q_OS_MAC)
    QDir dirBundle(QCoreApplication::applicationDirPath() + QStringLiteral("/../Resources") + shareItem);
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
    return QCoreApplication::applicationDirPath() + shareItem;
#else
    QDir dir(QCoreApplication::applicationDirPath() + shareItem);
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
QString VCommonSettings::MultisizeTablesPath()
{
    return SharePath(QStringLiteral("/tables/multisize"));
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::StandardTemplatesPath()
{
    return SharePath(QStringLiteral("/tables/templates"));
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::PrepareStandardTemplates(const QString & currentPath)
{
    return PrepareStandardFiles(currentPath, StandardTemplatesPath(), GetDefPathTemplate());
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::PrepareMultisizeTables(const QString &currentPath)
{
    return PrepareStandardFiles(currentPath, MultisizeTablesPath(), GetDefPathMultisizeMeasurements());
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
QString VCommonSettings::GetDefPathMultisizeMeasurements()
{
    return QDir::homePath() + QLatin1String("/valentina/") + tr("measurements") + QLatin1String("/") + tr("multisize");
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::GetPathMultisizeMeasurements() const
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), commonIniFilename);
    return settings.value(settingPathsMultisizeMeasurements, GetDefPathMultisizeMeasurements()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetPathMultisizeMeasurements(const QString &value)
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), commonIniFilename);
    settings.setValue(settingPathsMultisizeMeasurements, value);
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
QString VCommonSettings::GetDefPathLabelTemplate()
{
    return QDir::homePath() + QLatin1String("/valentina/") + tr("label templates");
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::GetPathLabelTemplate() const
{
    return value(settingPathsLabelTemplate, GetDefPathLabelTemplate()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetPathLabelTemplate(const QString &value)
{
    setValue(settingPathsLabelTemplate, value);
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
    return value(settingPMSystemCode, "p998").toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetPMSystemCode(const QString &value)
{
    setValue(settingPMSystemCode, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VCommonSettings::GetUnit() const
{
    return value(settingConfigurationUnit,
                 QLocale().measurementSystem() == QLocale::MetricSystem ? unitCM : unitINCH).toString();
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
    user = value(settingPatternUser, QString::fromLocal8Bit(qgetenv("USERNAME").constData())).toString();
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
QSize VCommonSettings::GetToolSeamAllowanceDialogSize() const
{
    return value(settingToolSeamAllowanceDialogSize, QSize(0, 0)).toSize();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetToolSeamAllowanceDialogSize(const QSize &sz)
{
    setValue(settingToolSeamAllowanceDialogSize, sz);
}

//---------------------------------------------------------------------------------------------------------------------
QSize VCommonSettings::GetFormulaWizardDialogSize() const
{
    return value(settingFormulaWizardDialogSize, QSize(0, 0)).toSize();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetFormulaWizardDialogSize(const QSize &sz)
{
    setValue(settingFormulaWizardDialogSize, sz);
}

//---------------------------------------------------------------------------------------------------------------------
QSize VCommonSettings::GetIncrementsDialogSize() const
{
    return value(settingIncrementsDialogSize, QSize(0, 0)).toSize();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetIncrementsDialogSize(const QSize &sz)
{
    setValue(settingIncrementsDialogSize, sz);
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
bool VCommonSettings::IsHideMainPath() const
{
    return value(settingPatternHideMainPath, false).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetHideMainPath(bool value)
{
    setValue(settingPatternHideMainPath, value);
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

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetCSVWithHeader(bool withHeader)
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), commonIniFilename);
    settings.setValue(settingCSVWithHeader, withHeader);
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommonSettings::GetCSVWithHeader() const
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), commonIniFilename);
    return settings.value(settingCSVWithHeader, GetDefCSVWithHeader()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VCommonSettings::GetDefCSVWithHeader() const
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetCSVCodec(int mib)
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), commonIniFilename);
    settings.setValue(settingCSVCodec, mib);
}

//---------------------------------------------------------------------------------------------------------------------
int VCommonSettings::GetCSVCodec() const
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), commonIniFilename);
    return settings.value(settingCSVCodec, GetDefCSVCodec()).toInt();
}

//---------------------------------------------------------------------------------------------------------------------
int VCommonSettings::GetDefCSVCodec() const
{
    return QTextCodec::codecForLocale()->mibEnum();
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetCSVSeparator(const QChar &separator)
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), commonIniFilename);
    switch(separator.toLatin1())
    {
        case '\t':
            settings.setValue(settingCSVSeparator, 0);
            break;
        case ';':
            settings.setValue(settingCSVSeparator, 1);
            break;
        case ' ':
            settings.setValue(settingCSVSeparator, 2);
            break;
        default:
            settings.setValue(settingCSVSeparator, 3);
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QChar VCommonSettings::GetCSVSeparator() const
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), commonIniFilename);
    const quint8 separator = static_cast<quint8>(settings.value(settingCSVSeparator, 3).toUInt());
    switch(separator)
    {
        case 0:
            return QChar('\t');
        case 1:
            return QChar(';');
        case 2:
            return QChar(' ');
        default:
            return QChar(',');
    }
}

//---------------------------------------------------------------------------------------------------------------------
QChar VCommonSettings::GetDefCSVSeparator() const
{
    return QChar(',');
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetDefaultSeamAllowance(double value)
{
    setValue(settingPatternDefaultSeamAllowance, UnitConvertor(value, StrToUnits(GetUnit()), Unit::Cm));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief returns the default seam allowance. The corresponding unit is the default unit.
 * @return the default seam allowance
 */
double VCommonSettings::GetDefaultSeamAllowance()
{
    double defaultValue;

    const Unit globalUnit = StrToUnits(GetUnit());

    switch (globalUnit)
    {
        case Unit::Mm:
            defaultValue = 10;
            break;
        case Unit::Inch:
            defaultValue = 0.25;
            break;
        default:
        case Unit::Cm:
            defaultValue = 1;
            break;
    }

    bool ok = false;
    double val = value(settingPatternDefaultSeamAllowance, -1).toDouble(&ok);
    if (ok == false)
    {
        qDebug()<< "Could not convert value"<<value(settingPatternDefaultSeamAllowance, 0)
                << "to real. Return default value for default seam allowance is "
                << defaultValue << ".";
        val = defaultValue;
    }

    if (val < 0)
    {
        val = defaultValue;
    }
    else
    {
        val = UnitConvertor(val, Unit::Cm, globalUnit);
    }

    return val;
}

//---------------------------------------------------------------------------------------------------------------------
QFont VCommonSettings::GetLabelFont() const
{
    return qvariant_cast<QFont>(value(settingLabelFont, QApplication::font()));
}

//---------------------------------------------------------------------------------------------------------------------
void VCommonSettings::SetLabelFont(const QFont &f)
{
    setValue(settingLabelFont, f);
}
