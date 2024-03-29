/************************************************************************
 **
 **  @file   vcommonsettings.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 7, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

class VCommonSettings : public QSettings
{
    Q_OBJECT
public:
    VCommonSettings(Format format, Scope scope, const QString &organization, const QString &application = QString(),
              QObject *parent = nullptr);
    VCommonSettings(const QString &fileName, Format format, QObject *parent = nullptr);

    static QString SharePath(const QString &shareItem);
    static QString MultisizeTablesPath();
    static QString StandardTemplatesPath();
    static QString LabelTemplatesPath();

    static QString PrepareStandardTemplates(const QString &currentPath);
    static QString PrepareMultisizeTables(const QString &currentPath);
    static QString PrepareLabelTemplates(const QString &currentPath);

    static QString GetDefPathIndividualMeasurements();
    QString GetPathIndividualMeasurements() const;
    void SetPathIndividualMeasurements(const QString &value);

    static QString GetDefPathMultisizeMeasurements();
    QString GetPathMultisizeMeasurements() const;
    void SetPathMultisizeMeasurements(const QString &value);

    static QString GetDefPathPattern();
    QString GetPathPattern() const;
    void SetPathPattern(const QString &value);

    static QString GetDefPathTemplate();
    QString GetPathTemplate() const;
    void SetPathTemplate(const QString &value);

    static QString GetDefPathLabelTemplate();
    QString GetPathLabelTemplate() const;
    void SetPathLabelTemplate(const QString &value);

    bool GetOsSeparator() const;
    void SetOsSeparator(const bool &value);

    bool GetAutosaveState() const;
    void SetAutosaveState(const bool &value);

    int  GetAutosaveTime() const;
    void SetAutosaveTime(const int &value);

    QString GetLocale() const;
    void SetLocale(const QString &value);

    QString GetPMSystemCode() const;
    void SetPMSystemCode(const QString &value);

    QString GetUnit() const;
    void SetUnit(const QString &value);

    bool GetConfirmItemDelete() const;
    void SetConfirmItemDelete(const bool &value);

    bool GetConfirmFormatRewriting() const;
    void SetConfirmFormatRewriting(const bool &value);

    bool GetToolBarStyle() const;
    void SetToolBarStyle(const bool &value);

    bool GetDarkMode() const;
    void SetDarkMode(const bool &value);

    bool IsFreeCurveMode() const;
    void SetFreeCurveMode(bool value);

    bool IsDoubleClickZoomFitBestCurrentPP() const;
    void SetDoubleClickZoomFitBestCurrentPP(bool value);

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

    QSize GetToolSeamAllowanceDialogSize() const;
    void  SetToolSeamAllowanceDialogSize(const QSize& sz);

    QSize GetFormulaWizardDialogSize() const;
    void  SetFormulaWizardDialogSize(const QSize& sz);

    QSize GetIncrementsDialogSize() const;
    void  SetIncrementsDialogSize(const QSize& sz);

    QSize GetFinalMeasurementsDialogSize() const;
    void  SetFinalMeasurementsDialogSize(const QSize& sz);

    int  GetLatestSkippedVersion() const;
    void SetLatestSkippedVersion(int value);

    QDate GetDateOfLastRemind() const;
    void  SetDateOfLastRemind(const QDate &date);

    bool GetForbidWorkpieceFlipping() const;
    void SetForbidWorkpieceFlipping(bool value);

    bool GetForceWorkpieceFlipping() const;
    void SetForceWorkpieceFlipping(bool value);

    bool IsHideMainPath() const;
    void SetHideMainPath(bool value);

    bool IsDoublePassmark() const;
    void SetDoublePassmark(bool value);

    void SetCSVWithHeader(bool withHeader);
    bool GetCSVWithHeader() const;
    static bool GetDefCSVWithHeader();

    void SetCSVCodec(int mib);
    int  GetCSVCodec() const;
    static int GetDefCSVCodec();

    void  SetCSVSeparator(const QChar &separator);
    QChar GetCSVSeparator() const;
    static QChar GetDefCSVSeparator();

    void SetDefaultSeamAllowance(double value);
    double GetDefaultSeamAllowance();

    QFont GetLabelFont() const;
    void  SetLabelFont(const QFont &f);

    int  GetLabelFontSize() const;
    void SetLabelFontSize(int size);
    static int GetDefLabelFontSize();

    bool GetHideLabels() const;
    void SetHideLabels(bool value);

#if !defined(Q_OS_WIN)
    static const QString unixStandardSharePath;
#endif

    QString GetLabelDateFormat() const;
    void    SetLabelDateFormat(const QString &format);

    static QStringList PredefinedDateFormats();
    QStringList GetUserDefinedDateFormats() const;
    void        SetUserDefinedDateFormats(const QStringList &formats);

    QString GetLabelTimeFormat() const;
    void    SetLabelTimeFormat(const QString &format);

    static QStringList PredefinedTimeFormats();
    QStringList GetUserDefinedTimeFormats() const;
    void        SetUserDefinedTimeFormats(const QStringList &formats);

    qreal GetCurveApproximationScale() const;
    void  SetCurveApproximationScale(qreal value);

    bool IsShowCurveDetails() const;
    void SetShowCurveDetails(bool value);

    bool IsPieceShowMainPath() const;
    void SetPieceShowMainPath(bool value);

    static qreal DefaultLineWidth();
    static qreal MinimalLineWidth();
    static qreal MaximalLineWidth();
    qreal GetLineWidth() const;
    void  SetLineWidth(qreal width);
    qreal WidthMainLine() const;
    qreal WidthHairLine() const;
private:
    Q_DISABLE_COPY(VCommonSettings)
};

//---------------------------------------------------------------------------------------------------------------------
inline qreal VCommonSettings::DefaultLineWidth()
{
    return 1.2; // mm
}

//---------------------------------------------------------------------------------------------------------------------
inline qreal VCommonSettings::MinimalLineWidth()
{
    return 0.5; // mm
}

//---------------------------------------------------------------------------------------------------------------------
inline qreal VCommonSettings::MaximalLineWidth()
{
    return 5.0; // mm
}

#endif // VCOMMONSETTINGS_H
