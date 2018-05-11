/************************************************************************
 **
 **  @file   vsettings.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 11, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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
#include <QPrinter>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QVariant>
#include <QPrinterInfo>
#include <QtDebug>
#include <QGlobalStatic>

#include "../vmisc/def.h"
#include "../vmisc/vmath.h"

Q_DECLARE_METATYPE(QMarginsF)

const int VSettings::defaultScrollingDuration = 300;
const int VSettings::scrollingDurationMin = 100;
const int VSettings::scrollingDurationMax = 1000;

const int VSettings::defaultScrollingUpdateInterval = 30;
const int VSettings::scrollingUpdateIntervalMin = 10;
const int VSettings::scrollingUpdateIntervalMax = 100;

const qreal VSettings::defaultSensorMouseScale = 2.0;
const qreal VSettings::sensorMouseScaleMin = 1.0;
const qreal VSettings::sensorMouseScaleMax = 10.0;

const qreal VSettings::defaultWheelMouseScale = 45.0;
const qreal VSettings::wheelMouseScaleMin = 1.0;
const qreal VSettings::wheelMouseScaleMax = 100.0;

const qreal VSettings::defaultScrollingAcceleration = 1.3;
const qreal VSettings::scrollingAccelerationMin = 1.0;
const qreal VSettings::scrollingAccelerationMax = 10.0;

namespace
{
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingConfigurationLabelLanguage,
                          (QLatin1String("configuration/label_language")))

Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingPathsPattern, (QLatin1String("paths/pattern")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingPathsLayout, (QLatin1String("paths/layout")))

Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingPatternGraphicalOutput, (QLatin1String("pattern/graphicalOutput")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingPatternUseOpenGLRender, (QLatin1String("pattern/useOpenGLRender")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingPatternKnownMaterials, (QLatin1String("pattern/knownMaterials")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingPatternRememberMaterials, (QLatin1String("pattern/rememberMaterials")))

Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutWidth, (QLatin1String("layout/width")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutSorting, (QLatin1String("layout/sorting")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutPaperHeight, (QLatin1String("layout/paperHeight")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutPaperWidth, (QLatin1String("layout/paperWidth")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutShift, (QLatin1String("layout/shift")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutRotate, (QLatin1String("layout/Rotate")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutRotationIncrease, (QLatin1String("layout/rotationIncrease")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutAutoCrop, (QLatin1String("layout/autoCrop")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutSaveLength, (QLatin1String("layout/saveLength")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutUnitePages, (QLatin1String("layout/unitePages")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingFields, (QLatin1String("layout/fields")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingIgnoreFields, (QLatin1String("layout/ignoreFields")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingStripOptimization, (QLatin1String("layout/stripOptimization")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingMultiplier, (QLatin1String("layout/multiplier")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingTextAsPaths, (QLatin1String("layout/textAsPaths")))

Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingTiledPDFMargins, (QLatin1String("tiledPDF/margins")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingTiledPDFPaperHeight, (QLatin1String("tiledPDF/paperHeight")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingTiledPDFPaperWidth, (QLatin1String("tiledPDF/paperWidth")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingTiledPDFOrientation, (QLatin1String("tiledPDF/orientation")))

Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingScrollingDuration, (QLatin1String("scrolling/duration")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingScrollingUpdateInterval, (QLatin1String("scrolling/updateInterval")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingScrollingSensorMouseScale, (QLatin1String("scrolling/sensorMouseScale")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingScrollingWheelMouseScale, (QLatin1String("scrolling/wheelMouseScale")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingScrollingAcceleration, (QLatin1String("scrolling/acceleration")))

// Reading settings file is very expensive, cache values to speed up getting a value
int scrollingDurationCached = -1;
int scrollingUpdateIntervalCached = -1;
qreal scrollingSensorMouseScaleCached = -1;
qreal scrollingWheelMouseScaleCached = -1;
qreal scrollingAccelerationCached = -1;
}

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
    return value(*settingConfigurationLabelLanguage, QLocale().bcp47Name()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLabelLanguage(const QString &value)
{
    setValue(*settingConfigurationLabelLanguage, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString VSettings::GetDefPathPattern()
{
    return QDir::homePath() + QLatin1String("/valentina/") + tr("patterns");
}

//---------------------------------------------------------------------------------------------------------------------
QString VSettings::GetPathPattern() const
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), this->applicationName());
    return settings.value(*settingPathsPattern, GetDefPathPattern()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetPathPattern(const QString &value)
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), this->applicationName());
    settings.setValue(*settingPathsPattern, value);
    settings.sync();
}

//---------------------------------------------------------------------------------------------------------------------
QString VSettings::GetDefPathLayout()
{
    return QDir::homePath() + QLatin1String("/valentina/") + tr("layouts");
}

//---------------------------------------------------------------------------------------------------------------------
QString VSettings::GetPathLayout() const
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), this->applicationName());
    return settings.value(*settingPathsLayout, GetDefPathLayout()).toString();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetPathLayout(const QString &value)
{
    QSettings settings(this->format(), this->scope(), this->organizationName(), this->applicationName());
    settings.setValue(*settingPathsLayout, value);
    settings.sync();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetGraphicalOutput() const
{
    return value(*settingPatternGraphicalOutput, 1).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetGraphicalOutput(const bool &value)
{
    setValue(*settingPatternGraphicalOutput, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::IsOpenGLRender() const
{
    return value(*settingPatternUseOpenGLRender, 0).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetOpenGLRender(bool value)
{
    setValue(*settingPatternUseOpenGLRender, value);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSettings::GetLayoutPaperHeight() const
{
    const qreal def = UnitConvertor(1189/*A0*/, Unit::Mm, Unit::Px);
    bool ok = false;
    const qreal height = value(*settingLayoutPaperHeight, def).toDouble(&ok);
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
    setValue(*settingLayoutPaperHeight, value);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSettings::GetLayoutPaperWidth() const
{
    const qreal def = UnitConvertor(841/*A0*/, Unit::Mm, Unit::Px);
    bool ok = false;
    const qreal width = value(*settingLayoutPaperWidth, def).toDouble(&ok);
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
    setValue(*settingLayoutPaperWidth, value);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSettings::GetLayoutShift() const
{
    const qreal def = GetDefLayoutShift();
    bool ok = false;
    const qreal shift = value(*settingLayoutShift, def).toDouble(&ok);
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
    setValue(*settingLayoutShift, value);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSettings::GetLayoutWidth() const
{
    const qreal def = GetDefLayoutWidth();
    bool ok = false;
    const qreal lWidth = value(*settingLayoutWidth, def).toDouble(&ok);
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
    setValue(*settingLayoutWidth, value);
}

//---------------------------------------------------------------------------------------------------------------------
QMarginsF VSettings::GetFields(const QMarginsF &def) const
{
    const QVariant val = value(*settingFields, QVariant::fromValue(def));
    if (val.canConvert<QMarginsF>())
    {
        return val.value<QMarginsF>();
    }
    return def;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetFields(const QMarginsF &value)
{
    setValue(*settingFields, QVariant::fromValue(value));
}

//---------------------------------------------------------------------------------------------------------------------
Cases VSettings::GetLayoutGroup() const
{
    const Cases def = GetDefLayoutGroup();
    bool ok = false;
    const int g = value(*settingLayoutSorting, static_cast<int>(def)).toInt(&ok);
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
    setValue(*settingLayoutSorting, static_cast<int>(value));
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetLayoutRotate() const
{
    return value(*settingLayoutRotate, GetDefLayoutRotate()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefLayoutRotate()
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutRotate(bool value)
{
    setValue(*settingLayoutRotate, value);
}

//---------------------------------------------------------------------------------------------------------------------
int VSettings::GetLayoutRotationIncrease() const
{
    const int def = GetDefLayoutRotationIncrease();
    bool ok = false;
    const int r = value(*settingLayoutRotationIncrease, def).toInt(&ok);
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
    setValue(*settingLayoutRotationIncrease, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetLayoutAutoCrop() const
{
    return value(*settingLayoutAutoCrop, GetDefLayoutAutoCrop()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefLayoutAutoCrop()
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutAutoCrop(bool value)
{
    setValue(*settingLayoutAutoCrop, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetLayoutSaveLength() const
{
    return value(*settingLayoutSaveLength, GetDefLayoutSaveLength()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefLayoutSaveLength()
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutSaveLength(bool value)
{
    setValue(*settingLayoutSaveLength, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetLayoutUnitePages() const
{
    return value(*settingLayoutUnitePages, GetDefLayoutUnitePages()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefLayoutUnitePages()
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutUnitePages(bool value)
{
    setValue(*settingLayoutUnitePages, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetIgnoreAllFields() const
{
    return value(*settingIgnoreFields, GetDefIgnoreAllFields()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefIgnoreAllFields()
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetIgnoreAllFields(bool value)
{
    setValue(*settingIgnoreFields, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetStripOptimization() const
{
    return value(*settingStripOptimization, GetDefStripOptimization()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefStripOptimization()
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetStripOptimization(bool value)
{
    setValue(*settingStripOptimization, value);
}

//---------------------------------------------------------------------------------------------------------------------
quint8 VSettings::GetMultiplier() const
{
    return static_cast<quint8>(value(*settingMultiplier, GetDefMultiplier()).toUInt());
}

//---------------------------------------------------------------------------------------------------------------------
quint8 VSettings::GetDefMultiplier()
{
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetMultiplier(quint8 value)
{
    setValue(*settingMultiplier, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetTextAsPaths() const
{
    return value(*settingTextAsPaths, GetDefTextAsPaths()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefTextAsPaths()
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetTextAsPaths(bool value)
{
    setValue(*settingTextAsPaths, value);
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VSettings::GetKnownMaterials() const
{
    return value(*settingPatternKnownMaterials, QStringList()).toStringList();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetKnownMaterials(const QStringList &list)
{
    setValue(*settingPatternKnownMaterials, list);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::IsRememberPatternMaterials() const
{
    return value(*settingPatternRememberMaterials, true).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetRememberPatternMaterials(bool value)
{
    setValue(*settingPatternRememberMaterials, value);
}

// settings for the tiled PDFs
//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetTiledPDFMargins returns the tiled pdf margins in the given unit. When the setting is
 * called for the first time, the 4 default margins are 10mm.
 * @param unit the unit in which are the value. Necessary because we save the values
 * internaly as mm so there is conversion beeing made.
 * @return tiled pdf margins
 */
QMarginsF VSettings::GetTiledPDFMargins(const Unit &unit) const
{
    // default value is 10mm. We save the margins in mm in the setting.
    const QMarginsF def = QMarginsF(10, 10, 10, 10);

    const QVariant val = value(*settingTiledPDFMargins, QVariant::fromValue(def));

    if (val.canConvert<QMarginsF>())
    {
        return UnitConvertor(val.value<QMarginsF>(), Unit::Mm, unit);
    }
    return UnitConvertor(def, Unit::Mm, unit);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetTiledPDFMargins sets the setting tiled pdf margins to the given value.
 * @param value the margins to save
 * @param unit the unit in which are the value. Necessary because we save the values
 * internaly as mm so there is conversion beeing made.
 */
void VSettings::SetTiledPDFMargins(const QMarginsF &value, const Unit &unit)
{
    setValue(*settingTiledPDFMargins, QVariant::fromValue(UnitConvertor(value, unit, Unit::Mm)));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetTiledPDFPaperHeight returns the paper height of tiled pdf in the desired unit.
 * @param unit the unit in which are the value. Necessary because we save the values
 * internaly as mm so there is conversion beeing made.
 * @return tiled pdf paper height
 */
qreal VSettings::GetTiledPDFPaperHeight(const Unit &unit) const
{
    const qreal def = 297 /*A4*/;
    bool ok = false;
    const qreal height = value(*settingTiledPDFPaperHeight, def).toDouble(&ok);
    if (ok)
    {
        return UnitConvertor(height, Unit::Mm, unit);
    }
    else
    {
        return UnitConvertor(def, Unit::Mm, unit);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetTiledPDFPaperHeight sets the tiled pdf paper height
 * @param value in mm
 * @param unit unit of the given value
 */
void VSettings::SetTiledPDFPaperHeight(qreal value, const Unit &unit)
{
    setValue(*settingTiledPDFPaperHeight, UnitConvertor(value, unit, Unit::Mm));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetTiledPDFPaperWidth returns the paper height of tiled pdf in the desired unit.
 * @param unit the unit in which are the value. Necessary because we save the values
 * internaly as mm so there is conversion beeing made.
 * @return tiled pdf paper width
 */
qreal VSettings::GetTiledPDFPaperWidth(const Unit &unit) const
{

    const qreal def = 210 /*A4*/;
    bool ok = false;
    const qreal width = value(*settingTiledPDFPaperWidth, def).toDouble(&ok);
    if (ok)
    {
        return UnitConvertor(width, Unit::Mm, unit);
    }
    else
    {
        return UnitConvertor(def, Unit::Mm, unit);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetTiledPDFPaperWidth sets the tiled pdf paper width
 * @param unit unit of the given value
 * @param value in mm
 */
void VSettings::SetTiledPDFPaperWidth(qreal value, const Unit &unit)
{
    setValue(*settingTiledPDFPaperWidth, UnitConvertor(value,unit, Unit::Mm));
}

//---------------------------------------------------------------------------------------------------------------------
PageOrientation VSettings::GetTiledPDFOrientation() const
{
    bool defaultValue = static_cast<bool>(PageOrientation::Portrait);
    bool result = value(*settingTiledPDFOrientation, defaultValue).toBool();
    return static_cast<PageOrientation>(result);
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetTiledPDFOrientation(PageOrientation value)
{
    setValue(*settingTiledPDFOrientation, static_cast<bool> (value));
}

//---------------------------------------------------------------------------------------------------------------------
int VSettings::GetScrollingDuration() const
{
    return GetCachedValue(scrollingDurationCached, *settingScrollingDuration, defaultScrollingDuration,
                          scrollingDurationMin, scrollingDurationMax);
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetScrollingDuration(int duration)
{
    scrollingDurationCached = qBound(scrollingDurationMin, duration, scrollingDurationMax);
    setValue(*settingScrollingDuration, scrollingDurationCached);
}

//---------------------------------------------------------------------------------------------------------------------
int VSettings::GetScrollingUpdateInterval() const
{
    return GetCachedValue(scrollingUpdateIntervalCached, *settingScrollingUpdateInterval,
                          defaultScrollingUpdateInterval, scrollingUpdateIntervalMin, scrollingUpdateIntervalMax);
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetScrollingUpdateInterval(int updateInterval)
{
    scrollingUpdateIntervalCached = qBound(scrollingUpdateIntervalMin, updateInterval, scrollingUpdateIntervalMax);
    setValue(*settingScrollingUpdateInterval, scrollingUpdateIntervalCached);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSettings::GetSensorMouseScale() const
{
    return GetCachedValue(scrollingSensorMouseScaleCached, *settingScrollingSensorMouseScale, defaultSensorMouseScale,
                          sensorMouseScaleMin, sensorMouseScaleMax);
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetSensorMouseScale(qreal scale)
{
    scrollingSensorMouseScaleCached = qBound(sensorMouseScaleMin, scale, sensorMouseScaleMax);
    setValue(*settingScrollingSensorMouseScale, scrollingSensorMouseScaleCached);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSettings::GetWheelMouseScale() const
{
    return GetCachedValue(scrollingWheelMouseScaleCached, *settingScrollingWheelMouseScale, defaultWheelMouseScale,
                          wheelMouseScaleMin, wheelMouseScaleMax);
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetWheelMouseScale(qreal scale)
{
    scrollingWheelMouseScaleCached = qBound(wheelMouseScaleMin, scale, wheelMouseScaleMax);
    setValue(*settingScrollingWheelMouseScale, scrollingWheelMouseScaleCached);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSettings::GetScrollingAcceleration() const
{
    return GetCachedValue(scrollingAccelerationCached, *settingScrollingAcceleration, defaultScrollingAcceleration,
                          scrollingAccelerationMin, scrollingAccelerationMax);
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetScrollingAcceleration(qreal acceleration)
{
    scrollingAccelerationCached = qBound(scrollingAccelerationMin, acceleration, scrollingAccelerationMax);
    setValue(*settingScrollingAcceleration, scrollingAccelerationCached);
}

//---------------------------------------------------------------------------------------------------------------------
template<typename T>
T VSettings::GetCachedValue(T &cache, const QString &setting, T defValue, T valueMin, T valueMax) const
{
    if (cache < 0)
    {
        const QVariant val = value(setting, defValue);
        cache = val.canConvert<T>() ? qBound(valueMin, val.value<T>(), valueMax) : defValue;
    }

    return cache;
}
