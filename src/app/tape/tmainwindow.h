/************************************************************************
 **
 **  @file   tmainwindow.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 7, 2015
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

#ifndef TMAINWINDOW_H
#define TMAINWINDOW_H

#include <QMainWindow>

#include "../vmisc/def.h"
#include "../vformat/vmeasurements.h"

namespace Ui
{
    class TMainWindow;
}

class QComboBox;
class QTableWidgetItem;
class QLabel;
#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
class QLockFile;
#endif

class TMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TMainWindow(QWidget *parent = 0);
    virtual ~TMainWindow() Q_DECL_OVERRIDE;

    QString CurrentFile() const;

public slots:
    void LoadFile(const QString &path);
    void FileNew();
    void OpenIndividual();
    void OpenStandard();
    void OpenTemplate();

protected:
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void FileSave();
    void FileSaveAs();
    void AboutToShowWindowMenu();
    void ShowWindow();
    void AboutApplication();

    void SaveGivenName();
    void SaveFamilyName();
    void SaveEmail();
    void SaveSex(int index);
    void SaveBirthDate(const QDate & date);
    void SaveNotes();
    void ReadOnly(bool ro);

    void Remove();
    void MoveUp();
    void MoveDown();
    void Fx();

    void AddCustom();
    void AddKnown();

    void ChangedSize(const QString &text);
    void ChangedHeight(const QString & text);

    void ShowMData();

    void DeployFormula();
    void ShowInGraphicalShell();

    void SaveMName();
    void SaveMValue();
    void SaveMBaseValue(double value);
    void SaveMSizeIncrease(double value);
    void SaveMHeightIncrease(double value);
    void SaveMDescription();
    void SaveMFullName();

    void NewWindow();

    void Preferences();

private:
    Q_DISABLE_COPY(TMainWindow)
    Ui::TMainWindow *ui;
    VMeasurements   *m;
    VContainer      *data;
    Unit             mUnit;
    MeasurementsType mType;
    QString          curFile;
    QComboBox       *gradationHeights;
    QComboBox       *gradationSizes;
    int              formulaBaseHeight;

#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
    QLockFile          *lock;
#endif

    void SetupMenu();
    void InitWindow();
    void InitTable();
    void SetDecimals();

    void ShowUnits();
    void ShowHeaderUnits(QTableWidget *table, int column, const QString &unit);

    void MeasurementsWasSaved(bool saved);
    void SetCurrentFile(const QString &fileName);
    bool SaveMeasurements(const QString &fileName, QString &error);

    bool MaybeSave();

    void AddCell(const QString &text, int row, int column, int aligment, bool ok = true);

    QComboBox *SetGradationList(const QString &label, const QStringList &list);
    void       SetDefaultHeight(int value);
    void       SetDefaultSize(int value);

    void RefreshData();
    void RefreshTable();

    void Controls();
    void MFields(bool enabled);

    QString ClearCustomName(const QString &name) const;

    void EvalFormula(const QString &formula, VContainer *data, QLabel *label);

    void Open(const QString &pathTo, const QString &filter);
    void GUIReadOnly(bool ro);

    void ReadSettings();
    void WriteSettings();
};

#endif // TMAINWINDOW_H
