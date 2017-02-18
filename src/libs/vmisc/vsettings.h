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

#include "../vlayout/vbank.h"
#include "vcommonsettings.h"

template <class T> class QSharedPointer;

#if QT_VERSION < QT_VERSION_CHECK(5, 3, 0)
#   include "../vmisc/backport/qmarginsf.h"
#else
#   include <QMargins>
#endif

class VSettings : public VCommonSettings
{
    Q_OBJECT
public:
    VSettings(Format format, Scope scope, const QString &organization, const QString &application = QString(),
              QObject *parent = 0);

    QString GetLabelLanguage() const;
    void SetLabelLanguage(const QString &value);

    static QString GetDefPathPattern();
    QString GetPathPattern() const;
    void SetPathPattern(const QString &value);

    static QString GetDefPathLayout();
    QString GetPathLayout() const;
    void SetPathLayout(const QString &value);

    bool GetGraphicalOutput() const;
    void SetGraphicalOutput(const bool &value);

    QString GetServer() const;
    void SetServer(const QString &value);

    bool GetServerSecure() const;
    void SetServerSecure(const bool &value);

    bool GetProxy() const;
    void SetProxy(const bool &value);

    QString GetProxyAddress() const;
    void SetProxyAddress(const QString &value);

    QString GetProxyPort() const;
    void SetProxyPort(const QString &value);

    QString GetProxyUser() const;
    void SetProxyUser(const QString &value);

    QString GetProxyPass() const;
    void SetProxyPass(const QString &value);

    QString GetUsername() const;
    void SetUsername(const QString &value);

    bool GetSavePassword() const;
    void SetSavePassword(const bool &value);

    QString GetUserPassword() const;
    void SetUserPassword(const QString &value);

    // Layout settings
    qreal GetLayoutPaperHeight() const;
    void SetLayoutPaperHeight(qreal value);

    qreal GetLayoutPaperWidth() const;
    void SetLayoutPaperWidth(qreal value);

    qreal GetLayoutShift() const;
    static qreal GetDefLayoutShift();
    void SetLayoutShift(qreal value);

    qreal GetLayoutWidth() const;
    static qreal GetDefLayoutWidth();
    void SetLayoutWidth(qreal value);

    QMarginsF GetFields(const QMarginsF &def = QMarginsF()) const;
    void SetFields(const QMarginsF &value);

    Cases GetLayoutGroup() const;
    static Cases GetDefLayoutGroup();
    void SetLayoutGroup(const Cases &value);

    bool GetLayoutRotate() const;
    static bool GetDefLayoutRotate();
    void SetLayoutRotate(bool value);

    int GetLayoutRotationIncrease() const;
    static int GetDefLayoutRotationIncrease();
    void SetLayoutRotationIncrease(int value);

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

private:
    Q_DISABLE_COPY(VSettings)
    static const QString SettingConfigurationLabelLanguage;

    static const QString SettingPathsPattern;
    static const QString SettingPathsLayout;

    static const QString SettingPatternGraphicalOutput;

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

    static const QString SettingLayoutWidth;
    static const QString SettingLayoutSorting;
    static const QString SettingLayoutPaperHeight;
    static const QString SettingLayoutPaperWidth;
    static const QString SettingLayoutShift;
    static const QString SettingLayoutRotate;
    static const QString SettingLayoutRotationIncrease;
    static const QString SettingLayoutAutoCrop;
    static const QString SettingLayoutSaveLength;
    static const QString SettingLayoutUnitePages;
    static const QString SettingFields;
    static const QString SettingIgnoreFields;
    static const QString SettingStripOptimization;
    static const QString SettingMultiplier;
};

#endif // VSETTINGS_H
