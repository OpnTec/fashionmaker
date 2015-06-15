/************************************************************************
 **
 **  @file   vsettings.cpp
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

#include "vsettings.h"

#include <QDir>
#include <QDebug>
#include <QLocale>
#include <QApplication>

#include "../../libs/ifc/ifcdef.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
#   include "../../libs/vmisc/vmath.h"
#else
#   include <QtMath>
#endif

const QString VSettings::SettingConfigurationOsSeparator         = QStringLiteral("configuration/osSeparator");
const QString VSettings::SettingConfigurationAutosaveState       = QStringLiteral("configuration/autosave/state");
const QString VSettings::SettingConfigurationAutosaveTime        = QStringLiteral("configuration/autosave/time");
const QString VSettings::SettingConfigurationSendReportState     = QStringLiteral("configuration/send_report/state");
const QString VSettings::SettingConfigurationLocale              = QStringLiteral("configuration/locale");
const QString VSettings::SettingConfigurationUnit                = QStringLiteral("configuration/unit");
const QString VSettings::SettingConfigurationLabelLanguage       = QStringLiteral("configuration/label_language");
const QString VSettings::SettingConfigurationConfirmItemDeletion
                                                                = QStringLiteral("configuration/confirm_item_deletion");
const QString VSettings::SettingConfigurationToolBarStyle        = QStringLiteral("configuration/tool_bar_style");

const QString VSettings::SettingPathsIndividualMeasurements      = QStringLiteral("paths/individual_measurements");
const QString VSettings::SettingPathsStandardMeasurements        = QStringLiteral("paths/standard_measurements");
const QString VSettings::SettingPathsPattern                     = QStringLiteral("paths/pattern");
const QString VSettings::SettingPathsLayout                      = QStringLiteral("paths/layout");

const QString VSettings::SettingPatternUser                      = QStringLiteral("pattern/user");
const QString VSettings::SettingPatternGraphicalOutput           = QStringLiteral("pattern/graphicalOutput");
const QString VSettings::SettingPatternUndo                      = QStringLiteral("pattern/undo");

const QString VSettings::SettingGeneralRecentFileList            = QStringLiteral("recentFileList");
const QString VSettings::SettingGeneralRestoreFileList           = QStringLiteral("restoreFileList");
const QString VSettings::SettingGeneralGeometry                  = QStringLiteral("geometry");
const QString VSettings::SettingGeneralWindowState               = QStringLiteral("windowState");
const QString VSettings::SettingGeneralToolbarsState             = QStringLiteral("toolbarsState");

const QString VSettings::SettingCommunityServer                  = QStringLiteral("community/server");
const QString VSettings::SettingCommunityServerSecure            = QStringLiteral("community/serverSecure");
const QString VSettings::SettingCommunityUseProxy                = QStringLiteral("community/useProxy");
const QString VSettings::SettingCommunityProxyAddress            = QStringLiteral("community/proxyAddress");
const QString VSettings::SettingCommunityProxyPort               = QStringLiteral("community/proxyPort");
const QString VSettings::SettingCommunityProxyUser               = QStringLiteral("community/proxyUser");
const QString VSettings::SettingCommunityProxyPass               = QStringLiteral("community/proxyPass");
const QString VSettings::SettingCommunityUsername                = QStringLiteral("community/username");
const QString VSettings::SettingCommunitySavePassword            = QStringLiteral("community/savePassword");
const QString VSettings::SettingCommunityUserPassword            = QStringLiteral("community/userpassword");

const QString VSettings::SettingLayoutWidth                      = QStringLiteral("layout/width");
const QString VSettings::SettingLayoutSorting                    = QStringLiteral("layout/sorting");
const QString VSettings::SettingLayoutPaperHeight                = QStringLiteral("layout/paperHeight");
const QString VSettings::SettingLayoutPaperWidth                 = QStringLiteral("layout/paperWidth");
const QString VSettings::SettingLayoutShift                      = QStringLiteral("layout/shift");
const QString VSettings::SettingLayoutRotate                     = QStringLiteral("layout/Rotate");
const QString VSettings::SettingLayoutRotationIncrease           = QStringLiteral("layout/rotationIncrease");
const QString VSettings::SettingLayoutAutoCrop                   = QStringLiteral("layout/autoCrop");
const QString VSettings::SettingLayoutSaveLength                 = QStringLiteral("layout/saveLength");
const QString VSettings::SettingLayoutUnitePages                 = QStringLiteral("layout/unitePages");

//---------------------------------------------------------------------------------------------------------------------
VSettings::VSettings(Format format, Scope scope, const QString &organization, const QString &application,
                     QObject *parent)
    :QSettings(format, scope, organization, application, parent)
{}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetOsSeparator() const
{
    return value(SettingConfigurationOsSeparator, 1).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetOsSeparator(const bool &value)
{
    setValue(SettingConfigurationOsSeparator, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetAutosaveState() const
{
    return value(SettingConfigurationAutosaveState, 1).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetAutosaveState(const bool &value)
{
    setValue(SettingConfigurationAutosaveState, value);
}

//---------------------------------------------------------------------------------------------------------------------
int VSettings::GetAutosaveTime() const
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
void VSettings::SetAutosaveTime(const int &value)
{
    setValue(SettingConfigurationAutosaveTime, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetSendReportState() const
{
    return value(SettingConfigurationSendReportState, 1).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetSendReportState(const bool &value)
{
    setValue(SettingConfigurationSendReportState, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VSettings::GetLocale() const
{
    return value(SettingConfigurationLocale, QLocale::system().name()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLocale(const QString &value)
{
    setValue(SettingConfigurationLocale, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VSettings::GetUnit() const
{
    return value(SettingConfigurationUnit, "cm").toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetUnit(const QString &value)
{
    setValue(SettingConfigurationUnit, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VSettings::GetLabelLanguage() const
{
    return value(SettingConfigurationLabelLanguage, QLocale::system().bcp47Name()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLabelLanguage(const QString &value)
{
    setValue(SettingConfigurationLabelLanguage, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetConfirmItemDelete() const
{
    return value(SettingConfigurationConfirmItemDeletion, 1).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetConfirmItemDelete(const bool &value)
{
    setValue(SettingConfigurationConfirmItemDeletion, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetToolBarStyle() const
{
    return value(SettingConfigurationToolBarStyle, 1).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetToolBarStyle(const bool &value)
{
    setValue(SettingConfigurationToolBarStyle, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VSettings::GetPathIndividualMeasurements() const
{
    return value(SettingPathsIndividualMeasurements, QDir::homePath()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetPathIndividualMeasurements(const QString &value)
{
    setValue(SettingPathsIndividualMeasurements, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VSettings::GetPathStandardMeasurements() const
{
    return value(SettingPathsStandardMeasurements, StandardTablesPath()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetPathStandardMeasurements(const QString &value)
{
    setValue(SettingPathsStandardMeasurements, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VSettings::GetPathPattern() const
{
    return value(SettingPathsPattern, QDir::homePath()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetPathPattern(const QString &value)
{
    setValue(SettingPathsPattern, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VSettings::GetPathLayout() const
{
    return value(SettingPathsLayout, QDir::homePath()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetPathLayout(const QString &value)
{
    setValue(SettingPathsLayout, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VSettings::GetUser() const
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
void VSettings::SetUser(const QString &value)
{
    setValue(SettingPatternUser, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetGraphicalOutput() const
{
    return value(SettingPatternGraphicalOutput, 1).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetGraphicalOutput(const bool &value)
{
    setValue(SettingPatternGraphicalOutput, value);
}

//---------------------------------------------------------------------------------------------------------------------
int VSettings::GetUndoCount() const
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
void VSettings::SetUndoCount(const int &value)
{
    setValue(SettingPatternUndo, value);
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VSettings::GetRecentFileList() const
{
    return value(SettingGeneralRecentFileList).toStringList();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetRecentFileList(const QStringList &value)
{
    setValue(SettingGeneralRecentFileList, value);
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VSettings::GetRestoreFileList() const
{
    return value(SettingGeneralRestoreFileList).toStringList();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetRestoreFileList(const QStringList &value)
{
    setValue(SettingGeneralRestoreFileList, value);
}

//---------------------------------------------------------------------------------------------------------------------
QByteArray VSettings::GetGeometry() const
{
    return value(SettingGeneralGeometry).toByteArray();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetGeometry(const QByteArray &value)
{
    setValue(SettingGeneralGeometry, value);
}

//---------------------------------------------------------------------------------------------------------------------
QByteArray VSettings::GetWindowState() const
{
    return value(SettingGeneralWindowState).toByteArray();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetWindowState(const QByteArray &value)
{
    setValue(SettingGeneralWindowState, value);
}

//---------------------------------------------------------------------------------------------------------------------
QByteArray VSettings::GetToolbarsState() const
{
    return value(SettingGeneralToolbarsState).toByteArray();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetToolbarsState(const QByteArray &value)
{
    setValue(SettingGeneralToolbarsState, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VSettings::GetServer() const
{
    return value(SettingCommunityServer, "community.valentina-project.org").toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetServer(const QString &value)
{
    setValue(SettingCommunityServer, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetServerSecure() const
{
    return value(SettingCommunityServerSecure, 0).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetServerSecure(const bool &value)
{
    setValue(SettingCommunityServerSecure, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetProxy() const
{
    return value(SettingCommunityUseProxy, 0).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetProxy(const bool &value)
{
    setValue(SettingCommunityUseProxy, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VSettings::GetProxyAddress() const
{
    return value(SettingCommunityProxyAddress).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetProxyAddress(const QString &value)
{
    setValue(SettingCommunityProxyAddress, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VSettings::GetProxyPort() const
{
    return value(SettingCommunityProxyPort).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetProxyPort(const QString &value)
{
    setValue(SettingCommunityProxyPort, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VSettings::GetProxyUser() const
{
    return value(SettingCommunityProxyUser).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetProxyUser(const QString &value)
{
    setValue(SettingCommunityProxyUser, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VSettings::GetProxyPass() const
{
    return value(SettingCommunityProxyPass).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetProxyPass(const QString &value)
{
    setValue(SettingCommunityProxyPass, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VSettings::GetUsername() const
{
    return value(SettingCommunityUsername).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetUsername(const QString &value)
{
    setValue(SettingCommunityUsername, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetSavePassword() const
{
    return value(SettingCommunitySavePassword, 0).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetSavePassword(const bool &value)
{
    setValue(SettingCommunitySavePassword, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VSettings::GetUserPassword() const
{
    return value(SettingCommunityUserPassword).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetUserPassword(const QString &value)
{
    setValue(SettingCommunityUserPassword, value);
}

//---------------------------------------------------------------------------------------------------------------------
int VSettings::GetLayoutPaperHeight() const
{
    const int def = qFloor(UnitConvertor(1189/*A0*/, Unit::Mm, Unit::Px));
    bool ok = false;
    const int height = value(SettingLayoutPaperHeight, def).toInt(&ok);
    if (ok)
    {
        return height;
    }
    else
    {
        return def;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutPaperHeight(int value)
{
    setValue(SettingLayoutPaperHeight, value);
}

//---------------------------------------------------------------------------------------------------------------------
int VSettings::GetLayoutPaperWidth() const
{
    const int def = qFloor(UnitConvertor(841/*A0*/, Unit::Mm, Unit::Px));
    bool ok = false;
    const int width = value(SettingLayoutPaperWidth, def).toInt(&ok);
    if (ok)
    {
        return width;
    }
    else
    {
        return def;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutPaperWidth(int value)
{
    setValue(SettingLayoutPaperWidth, value);
}

//---------------------------------------------------------------------------------------------------------------------
unsigned int VSettings::GetLayoutShift() const
{
    const unsigned int def = GetDefLayoutShift();
    bool ok = false;
    const unsigned int shift = value(SettingLayoutShift, def).toUInt(&ok);
    if (ok)
    {
        return shift;
    }
    else
    {
        return def;
    }
}

//---------------------------------------------------------------------------------------------------------------------
unsigned int VSettings::GetDefLayoutShift()
{
    return static_cast<unsigned int>(UnitConvertor(50, Unit::Mm, Unit::Px));
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutShift(unsigned int value)
{
    setValue(SettingLayoutShift, value);
}

//---------------------------------------------------------------------------------------------------------------------
unsigned int VSettings::GetLayoutWidth() const
{
    const unsigned int def = GetDefLayoutWidth();
    bool ok = false;
    const unsigned int lWidth = value(SettingLayoutWidth, def).toUInt(&ok);
    if (ok)
    {
        return lWidth;
    }
    else
    {
        return def;
    }
}

//---------------------------------------------------------------------------------------------------------------------
unsigned int VSettings::GetDefLayoutWidth()
{
    return static_cast<unsigned int>(UnitConvertor(2.5, Unit::Mm, Unit::Px));
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutWidth(unsigned int value)
{
    setValue(SettingLayoutWidth, value);
}

//---------------------------------------------------------------------------------------------------------------------
Cases VSettings::GetLayoutGroup() const
{
    const Cases def = GetDefLayoutGroup();
    bool ok = false;
    const int g = value(SettingLayoutSorting, static_cast<int>(def)).toInt(&ok);
    if (ok)
    {
        if (g >= static_cast<int>(Cases::UnknownCase))
        {
            return def;
        }
        else
        {
            return static_cast<Cases>(g);
        }
    }
    else
    {
        return def;
    }
}

//---------------------------------------------------------------------------------------------------------------------
Cases VSettings::GetDefLayoutGroup()
{
    return Cases::CaseDesc;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutGroup(const Cases &value)
{
    setValue(SettingLayoutSorting, static_cast<int>(value));
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetLayoutRotate() const
{
    return value(SettingLayoutRotate, GetDefLayoutRotate()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefLayoutRotate()
{
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutRotate(bool value)
{
    setValue(SettingLayoutRotate, value);
}

//---------------------------------------------------------------------------------------------------------------------
int VSettings::GetLayoutRotationIncrease() const
{
    const int def = GetDefLayoutRotationIncrease();
    bool ok = false;
    const int r = value(SettingLayoutRotationIncrease, def).toInt(&ok);
    if (ok)
    {
        if (not (r >= 1 && r <= 180 && 360 % r == 0))
        {
            return def;
        }
        else
        {
            return r;
        }
    }
    else
    {
        return def;
    }
}

//---------------------------------------------------------------------------------------------------------------------
int VSettings::GetDefLayoutRotationIncrease()
{
    return 180;/*degree*/
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutRotationIncrease(int value)
{
    setValue(SettingLayoutRotationIncrease, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetLayoutAutoCrop() const
{
    return value(SettingLayoutAutoCrop, false).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutAutoCrop(bool value)
{
    setValue(SettingLayoutAutoCrop, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetLayoutSaveLength() const
{
    return value(SettingLayoutSaveLength, false).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutSaveLength(bool value)
{
    setValue(SettingLayoutSaveLength, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetLayoutUnitePages() const
{
    return value(SettingLayoutUnitePages, false).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutUnitePages(bool value)
{
    setValue(SettingLayoutUnitePages, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VSettings::StandardTablesPath()
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
