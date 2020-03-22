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
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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
#include "../vlayout/vbank.h"

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
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingAutoRefreshPatternMessage,
                          (QLatin1String("configuration/autoRefreshPatternMessage")))

Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingPathsLayout, (QLatin1String("paths/layout")))

Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingPatternGraphicalOutput, (QLatin1String("pattern/graphicalOutput")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingPatternUseOpenGLRender, (QLatin1String("pattern/useOpenGLRender")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingPatternKnownMaterials, (QLatin1String("pattern/knownMaterials")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingPatternRememberMaterials, (QLatin1String("pattern/rememberMaterials")))

Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutWidth, (QLatin1String("layout/width")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutSorting, (QLatin1String("layout/sorting")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutPaperHeight, (QLatin1String("layout/paperHeight")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutPaperWidth, (QLatin1String("layout/paperWidth")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutFollowGrainline, (QLatin1String("layout/followGrainline")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutManualPriority, (QLatin1String("layout/manualPriority")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutNestQuantity, (QLatin1String("layout/nestQuantity")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutAutoCropLength, (QLatin1String("layout/autoCropLength")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutAutoCropWidth, (QLatin1String("layout/autoCropWidth")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutSaveLength, (QLatin1String("layout/saveLength")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutPreferOneSheetSolution,
                          (QLatin1String("layout/preferOneSheetSolution")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingLayoutUnitePages, (QLatin1String("layout/unitePages")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingFields, (QLatin1String("layout/fields")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingIgnoreFields, (QLatin1String("layout/ignoreFields")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingStripOptimization, (QLatin1String("layout/stripOptimization")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingMultiplier, (QLatin1String("layout/multiplier")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingTextAsPaths, (QLatin1String("layout/textAsPaths")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingNestingTime, (QLatin1String("layout/time")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingEfficiencyCoefficient, (QLatin1String("layout/efficiencyCoefficient")))

Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingTiledPDFMargins, (QLatin1String("tiledPDF/margins")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingTiledPDFPaperHeight, (QLatin1String("tiledPDF/paperHeight")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingTiledPDFPaperWidth, (QLatin1String("tiledPDF/paperWidth")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingTiledPDFOrientation, (QLatin1String("tiledPDF/orientation")))

Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingScrollingDuration, (QLatin1String("scrolling/duration")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingScrollingUpdateInterval, (QLatin1String("scrolling/updateInterval")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingScrollingSensorMouseScale,
                          (QLatin1String("scrolling/sensorMouseScale")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingScrollingWheelMouseScale, (QLatin1String("scrolling/wheelMouseScale")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingScrollingAcceleration, (QLatin1String("scrolling/acceleration")))

Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingdockWidgetGroupsActive, (QLatin1String("dockWidget/groupsActive")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingDockWidgetToolOptionsActive,
                          (QLatin1String("dockWidget/toolOptionsActive")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingDockWidgetPatternMessagesActive,
                          (QLatin1String("dockWidget/patternMessagesActive")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingPatternMessagesFontSize, (QLatin1String("font/patternMessagesSize")))

Q_GLOBAL_STATIC_WITH_ARGS(const QString, settingWatermarkEditorSize, (QLatin1String("watermarkEditorSize")))

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
VSettings::VSettings(const QString &fileName, QSettings::Format format, QObject *parent)
    :VCommonSettings(fileName, format, parent)
{
    qRegisterMetaTypeStreamOperators<QMarginsF>("QMarginsF");
}

//---------------------------------------------------------------------------------------------------------------------
template <class T>
inline T VSettings::ValueOrDef(const QString &setting, const T &defValue) const
{
    const QVariant val = value(setting, QVariant::fromValue(defValue));
    return val.canConvert<T>() ? val.value<T>() : defValue;
}

//---------------------------------------------------------------------------------------------------------------------
template <>
inline Cases VSettings::ValueOrDef<Cases>(const QString &setting, const Cases &defValue) const
{
    const QVariant val = value(setting, QVariant::fromValue(static_cast<int>(defValue)));
    const int g = val.canConvert<int>() ? val.value<int>() : static_cast<int>(defValue);
    if (g < static_cast<int>(Cases::CaseThreeGroup) || g >= static_cast<int>(Cases::UnknownCase))
    {
        return defValue;
    }
    else
    {
        return static_cast<Cases>(g);
    }
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
QString VSettings::GetDefPathLayout()
{
    return QDir::homePath() + QStringLiteral("/valentina/") + tr("layouts");
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
    return ValueOrDef<qreal>(*settingLayoutPaperHeight, UnitConvertor(1189/*A0*/, Unit::Mm, Unit::Px));
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutPaperHeight(qreal value)
{
    setValue(*settingLayoutPaperHeight, value);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSettings::GetLayoutPaperWidth() const
{
    return ValueOrDef<qreal>(*settingLayoutPaperWidth, UnitConvertor(841/*A0*/, Unit::Mm, Unit::Px));
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutPaperWidth(qreal value)
{
    setValue(*settingLayoutPaperWidth, value);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSettings::GetLayoutWidth() const
{
    return ValueOrDef<qreal>(*settingLayoutWidth, GetDefLayoutWidth());
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
int VSettings::GetNestingTime() const
{
    return ValueOrDef<int>(*settingNestingTime, GetDefNestingTime());
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetNestingTime(int value)
{
    setValue(*settingNestingTime, value);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSettings::GetEfficiencyCoefficient() const
{
    return ValueOrDef<qreal>(*settingEfficiencyCoefficient, GetDefEfficiencyCoefficient());
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetEfficiencyCoefficient(qreal value)
{
    setValue(*settingEfficiencyCoefficient, value);
}

//---------------------------------------------------------------------------------------------------------------------
QMarginsF VSettings::GetFields(const QMarginsF &def) const
{
    return ValueOrDef<QMarginsF>(*settingFields, def);
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetFields(const QMarginsF &value)
{
    setValue(*settingFields, QVariant::fromValue(value));
}

//---------------------------------------------------------------------------------------------------------------------
Cases VSettings::GetLayoutGroup() const
{
    return ValueOrDef<Cases>(*settingLayoutSorting, GetDefLayoutGroup());
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
bool VSettings::GetLayoutFollowGrainline() const
{
    return value(*settingLayoutFollowGrainline, GetDefLayoutFollowGrainline()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefLayoutFollowGrainline()
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutFollowGrainline(bool value)
{
    setValue(*settingLayoutFollowGrainline, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetLayoutManualPriority() const
{
    return value(*settingLayoutManualPriority, GetDefLayoutManualPriority()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefLayoutManualPriority()
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutManualPriority(bool value)
{
    setValue(*settingLayoutManualPriority, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetLayoutNestQuantity() const
{
    return value(*settingLayoutNestQuantity, GetDefLayoutNestQuantity()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefLayoutNestQuantity()
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutNestQuantity(bool value)
{
    setValue(*settingLayoutNestQuantity, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetLayoutAutoCropLength() const
{
    return value(*settingLayoutAutoCropLength, GetDefLayoutAutoCropLength()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefLayoutAutoCropLength()
{
    return false;
}
//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutAutoCropLength(bool value)
{
    setValue(*settingLayoutAutoCropLength, value);
}
//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetLayoutAutoCropWidth() const
{
    return value(*settingLayoutAutoCropWidth, GetDefLayoutAutoCropWidth()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefLayoutAutoCropWidth()
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutAutoCropWidth(bool value)
{
    setValue(*settingLayoutAutoCropWidth, value);
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
bool VSettings::GetLayoutPreferOneSheetSolution() const
{
    return value(*settingLayoutPreferOneSheetSolution, GetDefLayoutPreferOneSheetSolution()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefLayoutPreferOneSheetSolution()
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetLayoutPreferOneSheetSolution(bool value)
{
    setValue(*settingLayoutPreferOneSheetSolution, value);
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
    return UnitConvertor(ValueOrDef<QMarginsF>(*settingTiledPDFMargins, QMarginsF(10, 10, 10, 10)), Unit::Mm, unit);
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
    return UnitConvertor(ValueOrDef<qreal>(*settingTiledPDFPaperHeight, 297 /*A4*/), Unit::Mm, unit);
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
    return UnitConvertor(ValueOrDef<qreal>(*settingTiledPDFPaperWidth, 210 /*A4*/), Unit::Mm, unit);
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
bool VSettings::IsDockWidgetGroupsActive() const
{
    return value(*settingdockWidgetGroupsActive, GetDefDockWidgetGroupsActive()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefDockWidgetGroupsActive()
{
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetDockWidgetGroupsActive(bool value)
{
    setValue(*settingdockWidgetGroupsActive, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::IsDockWidgetToolOptionsActive() const
{
    return value(*settingDockWidgetToolOptionsActive, GetDefDockWidgetToolOptionsActive()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefDockWidgetToolOptionsActive()
{
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetDockWidgetToolOptionsActive(bool value)
{
    setValue(*settingDockWidgetToolOptionsActive, value);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::IsDockWidgetPatternMessagesActive() const
{
    return value(*settingDockWidgetPatternMessagesActive, GetDefDockWidgetPatternMessagesActive()).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetDefDockWidgetPatternMessagesActive()
{
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetDockWidgetPatternMessagesActive(bool value)
{
    setValue(*settingDockWidgetPatternMessagesActive, value);
}

//---------------------------------------------------------------------------------------------------------------------
int VSettings::GetPatternMessageFontSize(int fontSizeDef) const
{
    fontSizeDef = qBound(GetDefMinPatternMessageFontSize(), fontSizeDef, GetDefMaxPatternMessageFontSize());
    const int fontSize = value(*settingPatternMessagesFontSize, fontSizeDef).toInt();
    return qBound(GetDefMinPatternMessageFontSize(), fontSize, GetDefMaxPatternMessageFontSize());
}

//---------------------------------------------------------------------------------------------------------------------
int VSettings::GetDefMinPatternMessageFontSize()
{
    return 5;
}

//---------------------------------------------------------------------------------------------------------------------
int VSettings::GetDefMaxPatternMessageFontSize()
{
    return 40;
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetPatternMessageFontSize(int size)
{
    setValue(*settingPatternMessagesFontSize, qBound(GetDefMinPatternMessageFontSize(), size,
                                                     GetDefMaxPatternMessageFontSize()));
}

//---------------------------------------------------------------------------------------------------------------------
bool VSettings::GetAutoRefreshPatternMessage() const
{
    return value(*settingAutoRefreshPatternMessage, true).toBool();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetAutoRefreshPatternMessage(bool value)
{
    setValue(*settingAutoRefreshPatternMessage, value);
}

//---------------------------------------------------------------------------------------------------------------------
QSize VSettings::GetWatermarkEditorSize() const
{
    return value(*settingWatermarkEditorSize, QSize(0, 0)).toSize();
}

//---------------------------------------------------------------------------------------------------------------------
void VSettings::SetWatermarkEditorSize(const QSize &sz)
{
    setValue(*settingWatermarkEditorSize, sz);
}

//---------------------------------------------------------------------------------------------------------------------
template<typename T>
T VSettings::GetCachedValue(T &cache, const QString &setting, T defValue, T valueMin, T valueMax) const
{
    if (cache < 0)
    {
        cache = qBound(valueMin, ValueOrDef(setting, defValue), valueMax);
    }

    return cache;
}
