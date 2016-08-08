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
#include <QLocale>
#include <QMetaType>
#include <QMetaTypeId>
#include <QPrinter>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QVariant>

#include "../ifc/../vmisc/def.h"

class QMarginsF;

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
#   include "../vmisc/vmath.h"
#else
#endif

Q_DECLARE_METATYPE(QMarginsF)

const QString VSettings::SettingConfigurationLabelLanguage       = QStringLiteral("configuration/label_language");

const QString VSettings::SettingPathsPattern                     = QStringLiteral("paths/pattern");
const QString VSettings::SettingPathsLayout                      = QStringLiteral("paths/layout");

const QString VSettings::SettingPatternGraphicalOutput           = QStringLiteral("pattern/graphicalOutput");

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
const QString VSettings::SettingFields                           = QStringLiteral("layout/fields");
const QString VSettings::SettingIgnoreFields                     = QStringLiteral("layout/ignoreFields");
const QString VSettings::SettingStripOptimization                = QStringLiteral("layout/stripOptimization");
const QString VSettings::SettingMultiplier                       = QStringLiteral("layout/multiplier");

//---------------------------------------------------------------------------------------------------------------------
VSettings::VSettings(Format format, Scope scope, const QString &organization, const QString &application,
                     QObject *parent)
    :VCommonSettings(format, scope, organization, application, parent)
{
    qRegisterMetaTypeStreamOperators<QMarginsF>("QMarginsF");
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
QString VSettings::GetPathPattern() const
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), this->applicationName());
    return settings.value(SettingPathsPattern, QDir::homePath()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetPathPattern(const QString &value)
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), this->applicationName());
    settings.setValue(SettingPathsPattern, value);
    settings.sync();
}

//---------------------------------------------------------------------------------------------------------------------
QString VSettings::GetPathLayout() const
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), this->applicationName());
    return settings.value(SettingPathsLayout, QDir::homePath()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetPathLayout(const QString &value)
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), this->applicationName());
    settings.setValue(SettingPathsLayout, value);
    settings.sync();
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
qreal VSettings::GetLayoutPaperHeight() const
{
    const qreal def = UnitConvertor(1189/*A0*/, Unit::Mm, Unit::Px);
    bool ok = false;
    const qreal height = value(SettingLayoutPaperHeight, def).toDouble(&ok);
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
void VSettings::SetLayoutPaperHeight(qreal value)
{
    setValue(SettingLayoutPaperHeight, value);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSettings::GetLayoutPaperWidth() const
{
    const qreal def = UnitConvertor(841/*A0*/, Unit::Mm, Unit::Px);
    bool ok = false;
    const qreal width = value(SettingLayoutPaperWidth, def).toDouble(&ok);
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
void VSettings::SetLayoutPaperWidth(qreal value)
{
    setValue(SettingLayoutPaperWidth, value);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSettings::GetLayoutShift() const
{
    const qreal def = GetDefLayoutShift();
    bool ok = false;
    const qreal shift = value(SettingLayoutShift, def).toDouble(&ok);
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
qreal VSettings::GetDefLayoutShift()
{
    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutShift(qreal value)
{
    setValue(SettingLayoutShift, value);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSettings::GetLayoutWidth() const
{
    const qreal def = GetDefLayoutWidth();
    bool ok = false;
    const qreal lWidth = value(SettingLayoutWidth, def).toDouble(&ok);
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
qreal VSettings::GetDefLayoutWidth()
{
    return UnitConvertor(2.5, Unit::Mm, Unit::Px);
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutWidth(qreal value)
{
    setValue(SettingLayoutWidth, value);
}

//---------------------------------------------------------------------------------------------------------------------
QMarginsF VSettings::GetFields() const
{
    const QMarginsF def = GetDefFields();
    const QVariant val = value(SettingFields, QVariant::fromValue(def));
    if (val.canConvert<QMarginsF>())
    {
        return val.value<QMarginsF>();
    }
    return def;
}

//---------------------------------------------------------------------------------------------------------------------
QMarginsF VSettings::GetDefFields()
{
    QSharedPointer<QPrinter> printer = DefaultPrinter();
    if (printer.isNull())
    {
        return QMarginsF();
    }

    qreal left = 0;
    qreal top = 0;
    qreal right = 0;
    qreal bottom = 0;
    printer->getPageMargins(&left, &top, &right, &bottom, QPrinter::Millimeter);
    // We can't use Unit::Px because our dpi in most cases is different
    QMarginsF def;
    def.setLeft(UnitConvertor(left, Unit::Mm, Unit::Px));
    def.setRight(UnitConvertor(right, Unit::Mm, Unit::Px));
    def.setTop(UnitConvertor(top, Unit::Mm, Unit::Px));
    def.setBottom(UnitConvertor(bottom, Unit::Mm, Unit::Px));
    return def;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetFields(const QMarginsF &value)
{
    QVariant val = QVariant::fromValue(value);
    QString str = val.toString();
    setValue(SettingFields, val);
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
    return false;
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
    return value(SettingLayoutAutoCrop, GetDefLayoutAutoCrop()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefLayoutAutoCrop()
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutAutoCrop(bool value)
{
    setValue(SettingLayoutAutoCrop, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetLayoutSaveLength() const
{
    return value(SettingLayoutSaveLength, GetDefLayoutSaveLength()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefLayoutSaveLength()
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutSaveLength(bool value)
{
    setValue(SettingLayoutSaveLength, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetLayoutUnitePages() const
{
    return value(SettingLayoutUnitePages, GetDefLayoutUnitePages()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefLayoutUnitePages()
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutUnitePages(bool value)
{
    setValue(SettingLayoutUnitePages, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetIgnoreAllFields() const
{
    return value(SettingIgnoreFields, GetDefIgnoreAllFields()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefIgnoreAllFields()
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetIgnoreAllFields(bool value)
{
    setValue(SettingIgnoreFields, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetStripOptimization() const
{
    return value(SettingStripOptimization, GetDefStripOptimization()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefStripOptimization()
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetStripOptimization(bool value)
{
    setValue(SettingStripOptimization, value);
}

//---------------------------------------------------------------------------------------------------------------------
quint8 VSettings::GetMultiplier() const
{
    return static_cast<quint8>(value(SettingMultiplier, GetDefMultiplier()).toUInt());
}

//---------------------------------------------------------------------------------------------------------------------
quint8 VSettings::GetDefMultiplier()
{
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetMultiplier(quint8 value)
{
    setValue(SettingMultiplier, value);
}
