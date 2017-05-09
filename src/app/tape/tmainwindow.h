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

#include <QTableWidget>

#include "../vmisc/def.h"
#include "../vmisc/vlockguard.h"
#include "../vformat/vmeasurements.h"
#include "../vmisc/vtablesearch.h"
#include "../vwidgets/vabstractmainwindow.h"

namespace Ui
{
    class TMainWindow;
}

class QLabel;

class TMainWindow : public VAbstractMainWindow
{
    Q_OBJECT

public:
    explicit TMainWindow(QWidget *parent = nullptr);
    virtual ~TMainWindow() Q_DECL_OVERRIDE;

    QString CurrentFile() const;

    void RetranslateTable();

    void SetBaseMHeight(int height);
    void SetBaseMSize(int size);
    void SetPUnit(Unit unit);

    bool LoadFile(const QString &path);

public slots:
    virtual void ShowToolTip(const QString &toolTip) Q_DECL_OVERRIDE;

protected:
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    virtual void changeEvent(QEvent* event) Q_DECL_OVERRIDE;
    virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    virtual bool eventFilter(QObject *object, QEvent *event) Q_DECL_OVERRIDE;

private slots:
    void FileNew();
    void OpenIndividual();
    void OpenStandard();
    void OpenTemplate();
    void CreateFromExisting();
    void Preferences();
    void ToolBarStyles();

    bool FileSave();
    bool FileSaveAs();
    void ExportToCSV();
    void AboutToShowWindowMenu();
    void ShowWindow() const;

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

    void SaveMName(const QString &text);
    void SaveMValue();
    void SaveMBaseValue(double value);
    void SaveMSizeIncrease(double value);
    void SaveMHeightIncrease(double value);
    void SaveMDescription();
    void SaveMFullName();

    void PatternUnitChanged(int index);

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
    bool mIsReadOnly;
    enum { MaxRecentFiles = 5 };
    QAction            *recentFileActs[MaxRecentFiles];
    QAction            *separatorAct;
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
    void UpdateRecentFileActions();

    void MeasurementsWasSaved(bool saved);
    void SetCurrentFile(const QString &fileName);
    bool SaveMeasurements(const QString &fileName, QString &error);

    bool MaybeSave();

    QTableWidgetItem *AddCell(const QString &text, int row, int column, int aligment, bool ok = true);

    QComboBox *SetGradationList(QLabel *label, const QStringList &list) Q_REQUIRED_RESULT;
    void       SetDefaultHeight(int value);
    void       SetDefaultSize(int value);

    void RefreshData(bool freshCall = false);
    void RefreshTable(bool freshCall = false);

    QString GetCustomName() const;
    QString ClearCustomName(const QString &name) const;

    bool EvalFormula(const QString &formula, bool fromUser, VContainer *data, QLabel *label);
    void ShowMDiagram(const QString &name);

    void Open(const QString &pathTo, const QString &filter);
    void UpdatePadlock(bool ro);
    void MeasurementGUI();
    void Controls();
    void MFields(bool enabled);
    void UpdateWindowTitle();

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
