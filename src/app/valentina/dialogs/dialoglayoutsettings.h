/************************************************************************
 **
 **  @file   dialoglayoutsettings.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   13 1, 2015
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

#ifndef DIALOGLAYOUTSETTINGS_H
#define DIALOGLAYOUTSETTINGS_H

#include <QCloseEvent>
#include <QDialog>

#include "../vlayout/vbank.h"
#include "../ifc/ifcdef.h"
#include "../vlayout/vlayoutgenerator.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 3, 0)
#   include "../vmisc/backport/qmarginsf.h"
#else
#   include <QMargins>
#endif

namespace Ui
{
    class DialogLayoutSettings;
}

class VLayoutGenerator;

class DialogLayoutSettings : public QDialog
{
    Q_OBJECT
public:
    enum class PaperSizeTemplate : char { A0 = 0,
                                          A1,
                                          A2,
                                          A3,
                                          A4,
                                          Letter,
                                          Legal,
                                          Roll24in = 7, // Be carefull when change order roll type
                                          Roll30in,     // Used also for showing icon
                                          Roll36in,
                                          Roll42in,
                                          Roll44in = 11,
                                          Custom = 12};
    explicit DialogLayoutSettings(VLayoutGenerator *generator, QWidget *parent = nullptr, bool disableSettings = false);
    ~DialogLayoutSettings();

    qreal GetPaperHeight() const;
    void SetPaperHeight(qreal value);

    qreal GetPaperWidth() const;
    void SetPaperWidth(qreal value);

    qreal GetShift() const;
    void SetShift(qreal value);

    qreal GetLayoutWidth() const;
    void SetLayoutWidth(qreal value);

    QMarginsF GetFields() const;
    void SetFields(const QMarginsF &value);

    Cases GetGroup() const;
    void SetGroup(const Cases &value);

    bool GetRotate() const;
    void SetRotate(bool state);

    int GetIncrease() const;
    bool SetIncrease(int increase);

    bool GetAutoCrop() const;
    void SetAutoCrop(bool crop);

    bool IsSaveLength() const;
    void SetSaveLength(bool save);

    bool IsUnitePages() const;
    void SetUnitePages(bool save);

    bool IsStripOptimization() const;
    void SetStripOptimization(bool save);

    quint8 GetMultiplier() const;
    void   SetMultiplier(const quint8 &value);

    bool IsIgnoreAllFields() const;
    void SetIgnoreAllFields(bool value);

    QString SelectedPrinter() const;

    //support functions for the command line parser which uses invisible dialog to properly build layout generator
    bool SelectTemplate(const PaperSizeTemplate& id);
    static QString MakeHelpTemplateList();
    bool SelectPaperUnit(const QString& units);
    bool SelectLayoutUnit(const QString& units);
    qreal LayoutToPixels(qreal value) const;
    qreal PageToPixels(qreal value) const;
    static QString MakeGroupsHelp();
protected:
    virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
public slots:
    void DialogAccepted();
private slots:
    void ConvertPaperSize();
    void ConvertLayoutSize();
    void TemplateSelected();
    void FindTemplate();
    void PaperSizeChanged();
    void Swap(bool checked);
    void RestoreDefaults();
    void PrinterMargins();

    void CorrectMaxFileds();
    void IgnoreAllFields(int state);
private:
    Q_DISABLE_COPY(DialogLayoutSettings)
    typedef QStringList FormatsVector;
    typedef int VIndexType;
    const static  FormatsVector pageFormatNames;

    bool disableSettings;

    Ui::DialogLayoutSettings *ui;
    Unit oldPaperUnit;
    Unit oldLayoutUnit;
    VLayoutGenerator *generator;
    bool isInitialized;

    void InitPaperUnits();
    void InitLayoutUnits();
    void InitTemplates();
    void InitPrinter();
    QSizeF Template();
    QSizeF TemplateSize(const PaperSizeTemplate &tmpl) const;
    QSizeF RoundTemplateSize(qreal width, qreal height) const;
    QMarginsF RoundMargins(const QMarginsF &margins) const;
    QMarginsF MinPrinterFields() const;
    QMarginsF GetDefPrinterFields() const;

    Unit PaperUnit() const;
    Unit LayoutUnit() const;

    void CorrectPaperDecimals();
    void CorrectLayoutDecimals();

    void MinimumPaperSize();
    void MinimumLayoutSize();

    void ReadSettings();
    void WriteSettings() const;

    void SheetSize(const QSizeF &size);
    void SetAdditionalOptions(bool value);
};

#endif // DIALOGLAYOUTSETTINGS_H
