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
#include <QTableWidget>

#include "../vmisc/def.h"
#include "../vmisc/vlockguard.h"
#include "../vformat/vmeasurements.h"
#include "vtablesearch.h"

namespace Ui
{
    class TMainWindow;
}

class QComboBox;
class QTableWidgetItem;
class QLabel;

class TMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TMainWindow(QWidget *parent = 0);
    virtual ~TMainWindow() Q_DECL_OVERRIDE;

    QString CurrentFile() const;

    void RetranslateTable();

    void SetBaseMHeight(int height);
    void SetBaseMSize(int size);
    void SetPUnit(Unit unit);

public slots:
    bool LoadFile(const QString &path);
    void FileNew();
    void OpenIndividual();
    void OpenStandard();
    void OpenTemplate();
    void CreateFromExisting();

protected:
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    virtual void changeEvent(QEvent* event) Q_DECL_OVERRIDE;
    virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

private slots:
    void FileSave();
    void FileSaveAs();
    void AboutToShowWindowMenu();
    void ShowWindow();
    void AboutApplication();
    void AboutQt();

#if defined(Q_OS_MAC)
    void AboutToShowDockMenu();
    void OpenAt(QAction *where);
#endif //defined(Q_OS_MAC)

    void SaveGivenName();
    void SaveFamilyName();
    void SaveEmail();
    void SaveGender(int index);
    void SaveBirthDate(const QDate & date);
    void SaveNotes();
    void SavePMSystem(int index);
    void ReadOnly(bool ro);

    void Remove();
    void MoveTop();
    void MoveUp();
    void MoveDown();
    void MoveBottom();
    void Fx();

    void AddCustom();
    void AddKnown();
    void ImportFromPattern();

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
    void PatternUnitChanged(int index);
    void Find(const QString &term);
    void FindPrevious();
    void FindNext();

private:
    Q_DISABLE_COPY(TMainWindow)
    Ui::TMainWindow *ui;
    VMeasurements   *m;
    VContainer      *data;
    Unit             mUnit;
    Unit             pUnit;
    MeasurementsType mType;
    QString          curFile;
    QComboBox       *gradationHeights;
    QComboBox       *gradationSizes;
    QComboBox       *comboBoxUnits;
    int              formulaBaseHeight;
    std::shared_ptr<VLockGuard<char>> lock;
    QSharedPointer<VTableSearch> search;
    QLabel *labelGradationHeights;
    QLabel *labelGradationSizes;
    QLabel *labelPatternUnit;
    QAction *actionDockDiagram;
    bool dockDiagramVisible;
    bool isInitialized;
    QVector<QObject *> hackedWidgets;

    void SetupMenu();
    void InitWindow();
    void InitTable();
    void SetDecimals();
    void InitUnits();
    void InitComboBoxUnits();
    void InitGender(QComboBox *gender);

    void ShowUnits();
    void ShowHeaderUnits(QTableWidget *table, int column, const QString &unit);

    void MeasurementsWasSaved(bool saved);
    void SetCurrentFile(const QString &fileName);
    bool SaveMeasurements(const QString &fileName, QString &error);

    bool MaybeSave();

    QTableWidgetItem *AddCell(const QString &text, int row, int column, int aligment, bool ok = true);

    QComboBox *SetGradationList(QLabel *label, const QStringList &list);
    void       SetDefaultHeight(int value);
    void       SetDefaultSize(int value);

    void RefreshData();
    void RefreshTable();

    QString ClearCustomName(const QString &name) const;

    bool EvalFormula(const QString &formula, bool fromUser, VContainer *data, QLabel *label);
    void ShowMDiagram(const QString &name);

    void Open(const QString &pathTo, const QString &filter);
    void GUIReadOnly(bool ro);
    void MeasurementReadOnly(bool ro);
    void Controls();
    void MFields(bool enabled);

    void ReadSettings();
    void WriteSettings();

    QStringList FilterMeasurements(const QStringList &mNew, const QStringList &mFilter);

    void UpdatePatternUnit();

    bool LoadFromExistingFile(const QString &path);

    void CreateWindowMenu(QMenu *menu);

    bool IgnoreLocking(int error, const QString &path);

    template <class T>
    void HackWidget(T **widget);
};

#endif // TMAINWINDOW_H
