/************************************************************************
 **
 **  @file   mainwindow.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainwindowsnogui.h"
#include "widgets/vmaingraphicsview.h"
#include "dialogs/dialogs.h"
#include "tools/vtooldetail.h"
#include "tools/vtooluniondetails.h"
#include "tools/drawTools/drawtools.h"

namespace Ui
{
    class MainWindow;
}

class VToolOptionsPropertyBrowser;

/**
 * @brief The MainWindow class main windows.
 */
class MainWindow : public MainWindowsNoGUI
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();
    void               LoadPattern(const QString &curFile);
    void               ReopenFilesAfterCrash(QStringList &args);
public slots:
    void               mouseMove(const QPointF &scenePos);
    void               ArrowTool();

    void               ActionDraw(bool checked);
    void               ActionDetails(bool checked);
    void               ActionLayout(bool checked);

    void               ActionTable(bool checked);
    void               ActionHistory(bool checked);
    void               ActionCurveDetailsMode(bool checked);
    void               DrawOption();

    void               ClosedActionTable();
    void               ClosedActionHistory();

    void               New();
    bool               SaveAs();
    bool               Save();
    void               Open();
    void               NewPP();

    void               Preferences();
    void               RepotBug();
    void               OnlineHelp();
    void               About();
    void               AboutQt();
    void               PatternProperties();
    void               ShowToolTip(const QString &toolTip);
    void               OpenRecentFile();
    void               Clear();
    void               ResetWindow();

    void               currentPPChanged(int index);

    void               ChangedSize(const QString &text);
    void               ChangedHeight(const QString & text);

    void               PatternWasModified(bool saved);

    void               ToolEndLine(bool checked);
    void               ToolLine(bool checked);
    void               ToolAlongLine(bool checked);
    void               ToolShoulderPoint(bool checked);
    void               ToolNormal(bool checked);
    void               ToolBisector(bool checked);
    void               ToolLineIntersect(bool checked);
    void               ToolSpline(bool checked);
    void               ToolCutSpline(bool checked);
    void               ToolArc(bool checked);
    void               ToolSplinePath(bool checked);
    void               ToolCutSplinePath(bool checked);
    void               ToolPointOfContact(bool checked);
    void               ToolDetail(bool checked);
    void               ToolHeight(bool checked);
    void               ToolTriangle(bool checked);
    void               ToolPointOfIntersection(bool checked);
    void               ToolUnionDetails(bool checked);
    void               ToolCutArc(bool checked);
    void               ToolLineIntersectAxis(bool checked);
    void               ToolCurveIntersectAxis(bool checked);
    void               ToolPointOfIntersectionArcs(bool checked);
    void               ToolPointOfIntersectionCircles(bool checked);
    void               ToolPointFromCircleAndTangent(bool checked);

    void               ClosedDialogDetail(int result);
    void               ClosedDialogUnionDetails(int result);

//tmp
    void               LastUsedTool();

    /**
     * @brief Edit XML code of pattern
     */
    void               EditPatternCode();
    void               FullParseFile();

    void               SetEnabledGUI(bool enabled);

    void               ClickEndVisualization();
    void               Layout();
    void               UpdateGradation();
    void               GlobalChangePP(const QString &patternPiece);
    void               WindowsLocale();
    void               ToolBarStyles();

    void               ShowPaper(int index);
signals:
    void               RefreshHistory();
    void               EnableItemMove(bool move);
protected:
    virtual void       keyPressEvent(QKeyEvent *event);
    virtual void       showEvent(QShowEvent *event);
    virtual void       closeEvent(QCloseEvent *event);
    virtual void       customEvent(QEvent * event);

    virtual void       CleanLayout();
    virtual void       PrepareSceneList();
private:
    Q_DISABLE_COPY(MainWindow)
    /** @brief ui keeps information about user interface */
    Ui::MainWindow     *ui;



    /** @brief tool current tool */
    Tool               currentTool;

    /** @brief tool last used tool */
    Tool               lastUsedTool;

    /** @brief sceneDraw draw scene. */
    VMainGraphicsScene *sceneDraw;

    /** @brief sceneDetails details scene. */
    VMainGraphicsScene *sceneDetails;

    /** @brief mouseCoordinate pointer to label who show mouse coordinate. */
    QLabel             *mouseCoordinate;

    /** @brief helpLabel help show tooltip. */
    QLabel             *helpLabel;

    /** @brief isInitialized true after first show window. */
    bool               isInitialized;

    DialogIncrements   *dialogTable;
    DialogTool         *dialogTool;
    DialogHistory      *dialogHistory;

    /** @brief comboBoxDraws comboc who show name of pattern peaces. */
    QComboBox          *comboBoxDraws;

    /** @brief mode keep current draw mode. */
    Draw               mode;

    /** @brief currentDrawIndex save current selected pattern peace. */
    qint32             currentDrawIndex;

    /** @brief currentToolBoxIndex save current set of tools. */
    qint32             currentToolBoxIndex;

    /** @brief drawMode true if we current draw scene. */
    bool               drawMode;

    enum { MaxRecentFiles = 5 };
    QAction            *recentFileActs[MaxRecentFiles];
    QAction            *separatorAct;
    QTimer             *autoSaveTimer;
    bool               guiEnabled;
    QComboBox          *gradationHeights;
    QComboBox          *gradationSizes;
    VToolOptionsPropertyBrowser *toolOptions;
#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
    QLockFile          *lock;
#endif

    void               ToolBarOption();
    void               ToolBarDraws();
    void               ToolBarTools();
    void               InitToolButtons();
    void               CancelTool();

    void               SetEnableWidgets(bool enable);
    void               SetEnableTool(bool enable);
    void               SetLayoutModeActions(bool enable);

    void               SaveCurrentScene();
    void               RestoreCurrentScene();
    void               MinimumScrollBar();

    template <typename Dialog, typename Func>
    void               SetToolButton(bool checked, Tool t, const QString &cursor, const QString &toolTip,
                                     Func closeDialogSlot);
    template <typename Dialog, typename Func, typename Func2>
    void               SetToolButtonWithApply(bool checked, Tool t, const QString &cursor, const QString &toolTip,
                                              Func closeDialogSlot, Func2 applyDialogSlot);
    template <typename DrawTool>
    void               ClosedDialog(int result);
    template <typename DrawTool>
    void               ClosedDialogWithApply(int result);
    template <typename DrawTool>
    void               ApplyDialog();
    bool               SavePattern(const QString &curFile, QString &error);
    void               AutoSavePattern();
    void               setCurrentFile(const QString &fileName);
    QString            strippedName(const QString &fullFileName);

    void               ReadSettings();
    void               WriteSettings();

    bool               MaybeSave();
    void               UpdateRecentFileActions();
    void               CreateMenus();
    void               CreateActions();
    void               InitAutoSave();
    QString            PatternPieceName(const QString &text);
    QString            CheckPathToMeasurements(const QString &path, const MeasurementsType &patternType);
    QComboBox          *SetGradationList(const QString &label, const QStringList &list);
    void               ChangePP(int index, bool zoomBestFit = true);
    /**
     * @brief EndVisualization try show dialog after and working with tool visualization.
     */
    void               EndVisualization(bool click = false);
    void               ZoomFirstShow();
    void               UpdateHeightsList(const QStringList &list);
    void               UpdateSizesList(const QStringList &list);
    void               SetDefaultHeight(int value);
    void               SetDefaultSize(int value);

    void               AddDocks();
    void               PropertyBrowser();
    bool               OpenNewValentina(const QString &fileName = QString())const;
    void               FileClosedCorrect();
    QStringList        GetUnlokedRestoreFileList()const;
    void               ToolBarStyle(QToolBar *bar);

    void               AddPP(const QString &PPName, const QString &path);

    void               InitScenes();
};

#endif // MAINWINDOW_H
