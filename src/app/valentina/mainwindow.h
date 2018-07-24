/************************************************************************
 **
 **  @file   mainwindow.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainwindowsnogui.h"
#include "core/vcmdexport.h"
#include "../vmisc/vlockguard.h"

#include <QPointer>

namespace Ui
{
    class MainWindow;
}

class VToolOptionsPropertyBrowser;
class VMeasurements;
class QFileSystemWatcher;
class QLabel;
class DialogIncrements;
class DialogTool;
class DialogHistory;
class DialogFinalMeasurements;
class VWidgetGroups;
class VWidgetDetails;
class QToolButton;
class QDoubleSpinBox;
class QProgressBar;

/**
 * @brief The MainWindow class main windows.
 */
class MainWindow : public MainWindowsNoGUI
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow() override;

    bool LoadPattern(QString fileName, const QString &customMeasureFile = QString());

public slots:
    void ProcessCMD();
    virtual void ShowToolTip(const QString &toolTip) override;
    virtual void UpdateVisibilityGroups() override;
    virtual void UpdateDetailsList() override;
    virtual void ZoomFitBestCurrent() override;

signals:
    void RefreshHistory();
    void EnableItemMove(bool move);
    void ItemsSelection(SelectionType type);

    void EnableLabelSelection(bool enable);
    void EnablePointSelection(bool enable);
    void EnableLineSelection(bool enable);
    void EnableArcSelection(bool enable);
    void EnableElArcSelection(bool enable);
    void EnableSplineSelection(bool enable);
    void EnableSplinePathSelection(bool enable);
    void EnableNodeLabelSelection(bool enable);
    void EnableNodePointSelection(bool enable);
    void EnableDetailSelection(bool enable);

    void EnableLabelHover(bool enable);
    void EnablePointHover(bool enable);
    void EnableLineHover(bool enable);
    void EnableArcHover(bool enable);
    void EnableElArcHover(bool enable);
    void EnableSplineHover(bool enable);
    void EnableSplinePathHover(bool enable);
    void EnableNodeLabelHover(bool enable);
    void EnableNodePointHover(bool enable);
    void EnableDetailHover(bool enable);
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual void changeEvent(QEvent* event) override;
    virtual void closeEvent(QCloseEvent *event) override;
    virtual void customEvent(QEvent * event) override;
    virtual void CleanLayout() override;
    virtual void PrepareSceneList() override;
    virtual void ExportToCSVData(const QString &fileName, bool withHeader, int mib,
                                 const QChar &separator) final;
private slots:
    void ScaleChanged(qreal scale);
    void MouseMove(const QPointF &scenePos);
    void Clear();
    void PatternChangesWereSaved(bool saved);
    void LastUsedTool();
    void FullParseFile();
    void SetEnabledGUI(bool enabled);
    void GlobalChangePP(const QString &patternPiece);
    void PreviousPatternPiece();
    void NextPatternPiece();
    void ToolBarStyles();
    void ShowPaper(int index);
    void Preferences();
#if defined(Q_OS_MAC)
    void CreateMeasurements();
#endif
    void ExportLayoutAs();
    void ExportDetailsAs();

    void ArrowTool(bool checked);
    void ToolEndLine(bool checked);
    void ToolLine(bool checked);
    void ToolAlongLine(bool checked);
    void ToolMidpoint(bool checked);
    void ToolShoulderPoint(bool checked);
    void ToolNormal(bool checked);
    void ToolBisector(bool checked);
    void ToolLineIntersect(bool checked);
    void ToolSpline(bool checked);
    void ToolCubicBezier(bool checked);
    void ToolCutSpline(bool checked);
    void ToolArc(bool checked);
    void ToolEllipticalArc(bool checked);
    void ToolSplinePath(bool checked);
    void ToolCubicBezierPath(bool checked);
    void ToolCutSplinePath(bool checked);
    void ToolPointOfContact(bool checked);
    void ToolDetail(bool checked);
    void ToolPiecePath(bool checked);
    void ToolPin(bool checked);
    void ToolPlaceLabel(bool checked);
    void ToolHeight(bool checked);
    void ToolTriangle(bool checked);
    void ToolPointOfIntersection(bool checked);
    void ToolUnionDetails(bool checked);
    void ToolDuplicateDetail(bool checked);
    void ToolGroup(bool checked);
    void ToolRotation(bool checked);
    void ToolFlippingByLine(bool checked);
    void ToolFlippingByAxis(bool checked);
    void ToolMove(bool checked);
    void ToolCutArc(bool checked);
    void ToolLineIntersectAxis(bool checked);
    void ToolCurveIntersectAxis(bool checked);
    void ToolArcIntersectAxis(bool checked);
    void ToolPointOfIntersectionArcs(bool checked);
    void ToolPointOfIntersectionCircles(bool checked);
    void ToolPointOfIntersectionCurves(bool checked);
    void ToolPointFromCircleAndTangent(bool checked);
    void ToolPointFromArcAndTangent(bool checked);
    void ToolArcWithLength(bool checked);
    void ToolTrueDarts(bool checked);
    void ToolInsertNode(bool checked);

    void ActionDraw(bool checked);
    void ActionDetails(bool checked);
    void ActionLayout(bool checked);

    void New();
    bool SaveAs();
    bool Save();
    void Open();

    void ClosedDialogUnionDetails(int result);
    void ClosedDialogDuplicateDetail(int result);
    void ClosedDialogGroup(int result);
    void ClosedDialogPiecePath(int result);
    void ClosedDialogPin(int result);
    void ClosedDialogPlaceLabel(int result);
    void ClosedDialogInsertNode(int result);

    void LoadIndividual();
    void LoadMultisize();
    void UnloadMeasurements();
    void ShowMeasurements();
    void MeasurementsChanged(const QString &path);
    void SyncMeasurements();
#if defined(Q_OS_MAC)
    void OpenAt(QAction *where);
#endif //defined(Q_OS_MAC)

    void ChangedSize(const QString &text);
    void ChangedHeight(const QString &text);

    void ShowProgress();

private:
    Q_DISABLE_COPY(MainWindow)
    /** @brief ui keeps information about user interface */
    Ui::MainWindow     *ui;

    QFileSystemWatcher *watcher;

    /** @brief tool current tool */
    Tool               currentTool;

    /** @brief tool last used tool */
    Tool               lastUsedTool;

    /** @brief sceneDraw draw scene. */
    VMainGraphicsScene *sceneDraw;

    /** @brief sceneDetails details scene. */
    VMainGraphicsScene *sceneDetails;

    /** @brief mouseCoordinate pointer to label who show mouse coordinate. */
    QPointer<QLabel>    mouseCoordinate;

    /** @brief isInitialized true after first show window. */
    bool               isInitialized;

    /** @brief mChanges true if measurement file was changed. */
    bool               mChanges;
    bool               mChangesAsked;

    bool               patternReadOnly;

    QPointer<DialogIncrements>        dialogTable;
    QPointer<DialogTool>              dialogTool;
    QPointer<DialogHistory>           dialogHistory;
    QPointer<DialogFinalMeasurements> dialogFMeasurements;

    /** @brief comboBoxDraws comboc who show name of pattern peaces. */
    QComboBox          *comboBoxDraws;
    QLabel             *patternPieceLabel;

    /** @brief mode keep current draw mode. */
    Draw               mode;

    /** @brief currentDrawIndex save current selected pattern peace. */
    qint32             currentDrawIndex;

    /** @brief currentToolBoxIndex save current set of tools. */
    qint32             currentToolBoxIndex;

    bool               isDockToolOptionsVisible;
    bool               isDockGroupsVisible;

    /** @brief drawMode true if we current draw scene. */
    bool               drawMode;

    enum { MaxRecentFiles = 5 };
    QAction            *recentFileActs[MaxRecentFiles];
    QAction            *separatorAct;
    QLabel             *leftGoToStage;
    QLabel             *rightGoToStage;
    QTimer             *autoSaveTimer;
    bool               guiEnabled;
    QPointer<QComboBox> gradationHeights;
    QPointer<QComboBox> gradationSizes;
    QPointer<QLabel>   gradationHeightsLabel;
    QPointer<QLabel>   gradationSizesLabel;
    QPointer<QLabel>   zoomScale;
    QPointer<QDoubleSpinBox> doubleSpinBoxScale;
    VToolOptionsPropertyBrowser *toolOptions;
    VWidgetGroups *groupsWidget;
    VWidgetDetails *detailsWidget;
    std::shared_ptr<VLockGuard<char>> lock;

    QList<QToolButton*> toolButtonPointerList;

    QProgressBar *m_progressBar;
    QLabel       *m_statusLabel;

    void               SetDefaultHeight();
    void               SetDefaultSize();

    void               ToolBarOption();
    void               ToolBarStages();
    void               ToolBarDraws();
    void               ToolBarTools();
    void               InitToolButtons();
    void               CancelTool();

    void               SetEnableWidgets(bool enable);
    void               SetEnableTool(bool enable);
    void               SetLayoutModeActions();

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
    void ClosedDialogWithApply(int result, VMainGraphicsScene *scene);
    template <typename DrawTool>
    void ApplyDialog(VMainGraphicsScene *scene);
    template <typename DrawTool>
    void ClosedDrawDialogWithApply(int result);
    template <typename DrawTool>
    void ApplyDrawDialog();
    template <typename DrawTool>
    void ClosedDetailsDialogWithApply(int result);
    template <typename DrawTool>
    void ApplyDetailsDialog();

    bool               SavePattern(const QString &fileName, QString &error);
    void               AutoSavePattern();
    void               setCurrentFile(const QString &fileName);

    void               ReadSettings();
    void               WriteSettings();

    bool               MaybeSave();
    void               UpdateRecentFileActions();
    void               CreateMenus();
    void               CreateActions();
    void               InitAutoSave();
    QString            PatternPieceName(const QString &text);
    QString            CheckPathToMeasurements(const QString &patternPath, const QString &path);
    QComboBox          *SetGradationList(QLabel *label, const QStringList &list);
    void               ChangePP(int index, bool zoomBestFit = true);
    /**
     * @brief EndVisualization try show dialog after and working with tool visualization.
     */
    void               EndVisualization(bool click = false);
    void               ZoomFirstShow();
    void               UpdateHeightsList(const QStringList &list);
    void               UpdateSizesList(const QStringList &list);

    void               AddDocks();
    void               InitDocksContain();
    bool               OpenNewValentina(const QString &fileName = QString())const;
    void               FileClosedCorrect();
    QStringList        GetUnlokedRestoreFileList()const;

    void               AddPP(const QString &PPName);
    QPointF            StartPositionNewPP() const;

    void               InitScenes();

    QSharedPointer<VMeasurements> OpenMeasurementFile(const QString &path);
    bool               LoadMeasurements(const QString &path);
    bool               UpdateMeasurements(const QString &path, int size, int height);
    void               CheckRequiredMeasurements(const VMeasurements *m);

    void               ReopenFilesAfterCrash(QStringList &args);
    bool               DoExport(const VCommandLinePtr& expParams);
    bool               DoFMExport(const VCommandLinePtr& expParams);

    bool               SetSize(const QString &text);
    bool               SetHeight(const QString & text);

    QString            GetPatternFileName();
    QString            GetMeasurementFileName();

    void               UpdateWindowTitle();

    bool               IgnoreLocking(int error, const QString &path);

    void ToolSelectPoint();
    void ToolSelectPointByPress();
    void ToolSelectPointByRelease();
    void ToolSelectSpline();
    void ToolSelectSplinePath();
    void ToolSelectArc();
    void ToolSelectPointArc();
    void ToolSelectCurve();
    void ToolSelectAllDrawObjects();
    void ToolSelectOperationObjects();
    void ToolSelectGroupObjects();
    void ToolSelectDetail();
};

#endif // MAINWINDOW_H
