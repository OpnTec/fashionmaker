/************************************************************************
 **
 **  @file   vsettings.h
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

#ifndef VSETTINGS_H
#define VSETTINGS_H

#include <QMetaObject>
#include <QObject>
#include <QSettings>
#include <QString>
#include <QtGlobal>
#include <QMargins>

#include "../vmisc/def.h"
#include "../vlayout/vbank.h"
#include "vcommonsettings.h"

template <class T> class QSharedPointer;

class VSettings : public VCommonSettings
{
    Q_OBJECT
public:
    VSettings(Format format, Scope scope, const QString &organization, const QString &application = QString(),
              QObject *parent = nullptr);

    QString GetLabelLanguage() const;
    void SetLabelLanguage(const QString &value);

    static QString GetDefPathLayout();
    QString GetPathLayout() const;
    void SetPathLayout(const QString &value);

    bool GetGraphicalOutput() const;
    void SetGraphicalOutput(const bool &value);

    bool IsOpenGLRender() const;
    void SetOpenGLRender(bool value);

    // Layout settings
    qreal GetLayoutPaperHeight() const;
    void SetLayoutPaperHeight(qreal value);

    qreal GetLayoutPaperWidth() const;
    void SetLayoutPaperWidth(qreal value);

    qreal GetLayoutWidth() const;
    static qreal GetDefLayoutWidth();
    void SetLayoutWidth(qreal value);

    int  GetNestingTime() const;
    static int GetDefNestingTime(){return 1;}
    void SetNestingTime(int value);

    qreal GetEfficiencyCoefficient() const;
    static qreal GetDefEfficiencyCoefficient(){return 0.0;}
    void SetEfficiencyCoefficient(qreal value);

    QMarginsF GetFields(const QMarginsF &def = QMarginsF()) const;
    void SetFields(const QMarginsF &value);

    Cases GetLayoutGroup() const;
    static Cases GetDefLayoutGroup();
    void SetLayoutGroup(const Cases &value);

    bool GetLayoutFollowGrainline() const;
    static bool GetDefLayoutFollowGrainline();
    void SetLayoutFollowGrainline(bool value);

    bool GetLayoutAutoCrop() const;
    static bool GetDefLayoutAutoCrop();
    void SetLayoutAutoCrop(bool value);

    bool GetLayoutSaveLength() const;
    static bool GetDefLayoutSaveLength();
    void SetLayoutSaveLength(bool value);

    bool GetLayoutUnitePages() const;
    static bool GetDefLayoutUnitePages();
    void SetLayoutUnitePages(bool value);

    bool GetIgnoreAllFields() const;
    static bool GetDefIgnoreAllFields();
    void SetIgnoreAllFields(bool value);

    bool GetStripOptimization() const;
    static bool GetDefStripOptimization();
    void SetStripOptimization(bool value);

    quint8 GetMultiplier() const;
    static quint8 GetDefMultiplier();
    void SetMultiplier(quint8 value);

    bool GetTextAsPaths() const;
    static bool GetDefTextAsPaths();
    void SetTextAsPaths(bool value);

    QStringList GetKnownMaterials() const;
    void        SetKnownMaterials(const QStringList &list);

    bool IsRememberPatternMaterials() const;
    void SetRememberPatternMaterials(bool value);

    // settings for the tiled PDFs
    QMarginsF GetTiledPDFMargins(const Unit &unit) const;
    void      SetTiledPDFMargins(const QMarginsF &value, const Unit &unit);

    qreal GetTiledPDFPaperHeight(const Unit &unit) const;
    void  SetTiledPDFPaperHeight(qreal value, const Unit &unit);

    qreal GetTiledPDFPaperWidth(const Unit &unit) const;
    void  SetTiledPDFPaperWidth(qreal value, const Unit &unit);

    PageOrientation GetTiledPDFOrientation() const;
    void            SetTiledPDFOrientation(PageOrientation value);

    static const int defaultScrollingDuration;
    static const int scrollingDurationMin;
    static const int scrollingDurationMax;
    int GetScrollingDuration() const;
    void SetScrollingDuration(int duration);

    static const int defaultScrollingUpdateInterval;
    static const int scrollingUpdateIntervalMin;
    static const int scrollingUpdateIntervalMax;
    int GetScrollingUpdateInterval() const;
    void SetScrollingUpdateInterval(int updateInterval);

    static const qreal defaultSensorMouseScale;
    static const qreal sensorMouseScaleMin;
    static const qreal sensorMouseScaleMax;
    qreal GetSensorMouseScale() const;
    void SetSensorMouseScale(qreal scale);

    static const qreal defaultWheelMouseScale;
    static const qreal wheelMouseScaleMin;
    static const qreal wheelMouseScaleMax;
    qreal GetWheelMouseScale() const;
    void SetWheelMouseScale(qreal scale);

    static const qreal defaultScrollingAcceleration;
    static const qreal scrollingAccelerationMin;
    static const qreal scrollingAccelerationMax;
    qreal GetScrollingAcceleration() const;
    void SetScrollingAcceleration(qreal acceleration);

private:
    Q_DISABLE_COPY(VSettings)

    template <typename T>
    T GetCachedValue(T &cache, const QString &setting, T defValue, T valueMin, T valueMax) const;

    template <class T>
    T ValueOrDef(const QString &setting, const T &defValue) const;
};

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

#endif // VSETTINGS_H
