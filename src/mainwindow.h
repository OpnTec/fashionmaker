/************************************************************************
 **
 **  @file   mainwindow.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#include <QMainWindow>
#include "widgets/vmaingraphicsscene.h"
#include "widgets/vmaingraphicsview.h"
#include "widgets/vitem.h"
#include "dialogs/dialogs.h"
#include "tools/vtooldetail.h"
#include "tools/vtooluniondetails.h"
#include "tools/drawTools/drawtools.h"
#include "xml/vdomdocument.h"

namespace Ui
{
    class MainWindow;
}

/**
 * @brief The MainWindow class main windows.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
              /**
               * @brief MainWindow constructor.
               * @param parent parent widget.
               */
              explicit MainWindow(QWidget *parent = nullptr);
                       ~MainWindow();
    /**
     * @brief LoadPattern open pattern file.
     * @param fileName name of file.
     */
    void               LoadPattern(const QString &curFile);
public slots:
    /**
     * @brief mouseMove save mouse position and show user.
     * @param scenePos position mouse.
     */
    void               mouseMove(const QPointF &scenePos);
    /**
     * @brief ActionAroowTool set arrow tool. Cansel tool what was before.
     */
    void               ActionAroowTool();
    /**
     * @brief ActionDraw show draw scene.
     * @param checked true - button checked.
     */
    void               ActionDraw(bool checked);
    /**
     * @brief ActionDetails show details scene.
     * @param checked true - button checked.
     */
    void               ActionDetails(bool checked);
    /**
     * @brief ActionNewDraw add to scene new pattern peace.
     */
    void               ActionNewDraw();
    /**
     * @brief SaveAs save as pattern file.
     * @return true for successes saving.
     */
    bool               SaveAs();
    /**
     * @brief Save save pattern file.
     * @return true for successes saving.
     */
    bool               Save();
    /**
     * @brief Open ask user select pattern file.
     */
    void               Open();
    /**
     * @brief Options config dialog.
     */
    void               Options();
    /**
     * @brief NewPattern create new empty pattern.
     */
    void               NewPattern();
    /**
     * @brief ActionTable show table with variables.
     * @param checked true - button checked.
     */
    void               ActionTable(bool checked);
    /**
     * @brief ActionHistory show tool history.
     * @param checked true - button checked.
     */
    void               ActionHistory(bool checked);
    /**
     * @brief ActionLayout begin creation layout.
     * @param checked true - button checked.
     */
    void               ActionLayout(bool checked);
    /**
     * @brief currentDrawChanged change active pattern peace.
     * @param index index in combobox.
     */
    void               currentDrawChanged( int index );
    /**
     * @brief OptionDraw help change name of pattern peace.
     */
    void               OptionDraw();
    /**
     * @brief haveChange enable action save if we have unsaved change.
     */
    void               PatternWasModified();
    /**
     * @brief ChangedSize change new size value.
     * @param text value size.
     */
    void               ChangedSize(const QString &text);
    /**
     * @brief ChangedGrowth change new height value.
     * @param text value height.
     */
    void               ChangedHeight(const QString & text);
    /**
     * @brief ClosedActionTable actions after closing table with variables.
     */
    void               ClosedActionTable();
    /**
     * @brief ClosedActionHistory actions after closing history window with variables.
     */
    void               ClosedActionHistory();
    /**
     * @brief ToolEndLine handler tool endLine.
     * @param checked true - button checked.
     */
    void               ToolEndLine(bool checked);
    /**
     * @brief ToolLine handler tool line.
     * @param checked true - button checked.
     */
    void               ToolLine(bool checked);
    /**
     * @brief ToolAlongLine handler tool alongLine.
     * @param checked true - button checked.
     */
    void               ToolAlongLine(bool checked);
    /**
     * @brief ToolShoulderPoint handler tool shoulderPoint.
     * @param checked true - button checked.
     */
    void               ToolShoulderPoint(bool checked);
    /**
     * @brief ToolNormal handler tool normal.
     * @param checked true - button checked.
     */
    void               ToolNormal(bool checked);
    /**
     * @brief ToolBisector handler tool bisector.
     * @param checked true - button checked.
     */
    void               ToolBisector(bool checked);
    /**
     * @brief ToolLineIntersect handler tool lineIntersect.
     * @param checked true - button checked.
     */
    void               ToolLineIntersect(bool checked);
    /**
     * @brief ToolSpline handler tool spline.
     * @param checked true - button checked.
     */
    void               ToolSpline(bool checked);
    /**
     * @brief ToolCutSpline handler tool CutSpline.
     * @param checked true - button is checked
     */
    void               ToolCutSpline(bool checked);
    /**
     * @brief ToolArc handler tool arc.
     * @param checked true - button checked.
     */
    void               ToolArc(bool checked);
    /**
     * @brief ToolSplinePath handler tool splinePath.
     * @param checked true - button checked.
     */
    void               ToolSplinePath(bool checked);
    /**
     * @brief ToolCutSplinePath handler tool CutSplinePath.
     * @param checked true - button is checked
     */
    void               ToolCutSplinePath(bool checked);
    /**
     * @brief ToolPointOfContact handler tool pointOfContact.
     * @param checked true - button checked.
     */
    void               ToolPointOfContact(bool checked);
    /**
     * @brief ToolDetail handler tool detail.
     * @param checked true - button checked.
     */
    void               ToolDetail(bool checked);
    /**
     * @brief ToolHeight handler tool height.
     * @param checked true - button checked.
     */
    void               ToolHeight(bool checked);
    /**
     * @brief ToolTriangle handler tool triangle.
     * @param checked true - button checked.
     */
    void               ToolTriangle(bool checked);
    /**
     * @brief ToolPointOfIntersection handler tool pointOfIntersection.
     * @param checked true - button checked.
     */
    void               ToolPointOfIntersection(bool checked);
    /**
     * @brief ToolUnionDetails handler tool unionDetails.
     * @param checked true - button checked.
     */
    void               ToolUnionDetails(bool checked);
    /**
     * @brief ToolCutArc handler tool cutArc.
     * @param checked true - button checked.
     */
    void               ToolCutArc(bool checked);
    /**
     * @brief ClosedDialogEndLine actions after closing DialogEndLine.
     * @param result result of dialog working.
     */
    void               ClosedDialogEndLine(int result);
    /**
     * @brief ClosedDialogLine actions after closing DialogLine.
     * @param result result of dialog working.
     */
    void               ClosedDialogLine(int result);
    /**
     * @brief ClosedDialogAlongLine actions after closing DialogAlongLine.
     * @param result result of dialog working.
     */
    void               ClosedDialogAlongLine(int result);
    /**
     * @brief ClosedDialogShoulderPoint actions after closing DialogShoulderPoint.
     * @param result result of dialog working.
     */
    void               ClosedDialogShoulderPoint(int result);
    /**
     * @brief ClosedDialogNormal actions after closing DialogNormal.
     * @param result result of dialog working.
     */
    void               ClosedDialogNormal(int result);
    /**
     * @brief ClosedDialogBisector actions after closing DialogBisector.
     * @param result result of dialog working.
     */
    void               ClosedDialogBisector(int result);
    /**
     * @brief ClosedDialogLineIntersect actions after closing DialogLineIntersect.
     * @param result result of dialog working.
     */
    void               ClosedDialogLineIntersect(int result);
    /**
     * @brief ClosedDialogSpline actions after closing DialogSpline.
     * @param result result of dialog working.
     */
    void               ClosedDialogSpline(int result);
    /**
     * @brief ClosedDialogArc actions after closing DialogArc.
     * @param result result of dialog working..
     */
    void               ClosedDialogArc(int result);
    /**
     * @brief ClosedDialogSplinePath actions after closing DialogSplinePath.
     * @param result result of dialog working.
     */
    void               ClosedDialogSplinePath(int result);
    /**
     * @brief ClosedDialogCutSplinePath actions after closing DialogCutSplinePath.
     * @param result result of dialog working.
     */
    void               ClosedDialogCutSplinePath(int result);
    /**
     * @brief ClosedDialogPointOfContact actions after closing DialogPointOfContact.
     * @param result result of dialog working.
     */
    void               ClosedDialogPointOfContact(int result);
    /**
     * @brief ClosedDialogDetail actions after closing DialogDetail.
     * @param result result of dialog working.
     */
    void               ClosedDialogDetail(int result);
    /**
     * @brief ClosedDialogHeight actions after closing DialogHeight.
     * @param result result of dialog working.
     */
    void               ClosedDialogHeight(int result);
    /**
     * @brief ClosedDialogTriangle actions after closing DialogTriangle.
     * @param result result of dialog working.
     */
    void               ClosedDialogTriangle(int result);
    /**
     * @brief ClosedDialogPointOfIntersection actions after closing DialogPointOfIntersection.
     * @param result result of dialog working.
     */
    void               ClosedDialogPointOfIntersection(int result);
    /**
     * @brief ClosedDialogUnionDetails actions after closing DialogUnionDetails.
     * @param result result of dialog working.
     */
    void               ClosedDialogUnionDetails(int result);
    /**
     * @brief ClosedDialogCutSpline actions after closing DialogCutSpline.
     * @param result result of dialog working.
     */
    void               ClosedDialogCutSpline(int result);
    /**
     * @brief ClosedDialogCutArc actions after closing DialogCutArc.
     * @param result result of dialog working.
     */
    void               ClosedDialogCutArc(int result);
    /**
     * @brief About show widows about.
     */
    void               About();
    /**
     * @brief AboutQt show widows aboutQt.
     */
    void               AboutQt();
    /**
     * @brief ShowTool  highlight tool.Tip show tools tooltip.
     * @param toolTip tooltip text.
     */
    void               ShowToolTip(const QString &toolTip);
    /**
     * @brief tableClosed handle after close layout window.
     */
    void               tableClosed();
    void               OpenRecentFile();
    void               PatternProperties();
signals:
    /**
     * @brief ModelChosen emit after calculation all details.
     * @param listDetails list of details.
     * @param description pattern description.
     */
    void               ModelChosen(QVector<VItem*> listDetails, const QString &curFile, const QString &description);
protected:
    /**
     * @brief keyPressEvent handle key press events.
     * @param event key event.
     */
    virtual void       keyPressEvent ( QKeyEvent * event );
    /**
     * @brief showEvent handle after show window.
     * @param event show event.
     */
    virtual void       showEvent( QShowEvent *event );
    /**
     * @brief closeEvent handle after close window.
     * @param event close event.
     */
    virtual void       closeEvent( QCloseEvent * event );
    /**
     * @brief Clear reset to default window.
     */
    void               Clear();
private:
    Q_DISABLE_COPY(MainWindow)
    /**
     * @brief ui keeps information about user interface
     */
    Ui::MainWindow     *ui;
    /**
     * @brief pattern container with data (points, arcs, splines, spline paths, variables)
     */
    VContainer        *pattern;
    /**
     * @brief doc dom document container
     */
    VPattern          *doc;
    /**
     * @brief tool current tool
     */
    Valentina::Tools        tool;
    /**
     * @brief currentScene pointer to current scene.
     */
    VMainGraphicsScene *currentScene;
    /**
     * @brief sceneDraw draw scene.
     */
    VMainGraphicsScene *sceneDraw;
    /**
     * @brief sceneDetails details scene.
     */
    VMainGraphicsScene *sceneDetails;
    /**
     * @brief mouseCoordinate pointer to label who show mouse coordinate.
     */
    QLabel             *mouseCoordinate;
    /**
     * @brief helpLabel help show tooltip.
     */
    QLabel             *helpLabel;
    /**
     * @brief view show current scene.
     */
    VMainGraphicsView  *view;
    /**
     * @brief isInitialized true after first show window.
     */
    bool               isInitialized;
    DialogIncrements   *dialogTable;
    DialogTool         *dialogTool;
    DialogHistory      *dialogHistory;
    /**
     * @brief comboBoxDraws comboc who show name of pattern peaces.
     */
    QComboBox          *comboBoxDraws;
    /**
     * @brief fileName name current pattern file.
     */
    QString            curFile;
    /**
     * @brief mode keep current draw mode.
     */
    Valentina::Draws        mode;
    /**
     * @brief currentDrawIndex save current selected pattern peace.
     */
    qint32             currentDrawIndex;
    /**
     * @brief currentToolBoxIndex save current set of tools.
     */
    qint32             currentToolBoxIndex;
    /**
     * @brief drawMode true if we current draw scene.
     */
    bool               drawMode;
    enum { MaxRecentFiles = 5 };
    QAction            *recentFileActs[MaxRecentFiles];
    QAction            *separatorAct;
    QTimer             *autoSaveTimer;
    /**
     * @brief ToolBarOption enable option toolbar.
     */
    void               ToolBarOption();
    /**
     * @brief ToolBarDraws enable draw toolbar.
     */
    void               ToolBarDraws();
    /**
     * @brief CanselTool cansel tool.
     */
    void               CancelTool();
    /**
     * @brief ArrowTool enable arrow tool.
     */
    void               ArrowTool();
    /**
     * @brief SetEnableWidgets enable action button.
     * @param enable enable value.
     */
    void               SetEnableWidgets(bool enable);
    /**
     * @brief SetEnableTool enable button.
     * @param enable enable value.
     */
    void               SetEnableTool(bool enable);
    /**
     * @brief SaveCurrentScene save scene options before set another.
     */
    void               SaveCurrentScene();
    /**
     * @brief RestoreCurrentScene restore scene options after change.
     */
    void               RestoreCurrentScene();
    template <typename Dialog, typename Func>
    /**
     * @brief SetToolButton set tool and show dialog.
     * @param checked true if tool button checked.
     * @param t tool type.
     * @param cursor path tool cursor icon.
     * @param toolTip first tooltipe.
     * @param closeDialogSlot function what handle after close dialog.
     */
    void               SetToolButton(bool checked, Valentina::Tools t, const QString &cursor, const QString &toolTip,
                                     Func closeDialogSlot);
    /**
     * @brief MinimumScrollBar set scroll bar to minimum.
     */
    void               MinimumScrollBar();
    template <typename DrawTool>
    /**
     * @brief ClosedDialog handle close dialog
     * @param result result working dialog.
     */
    void               ClosedDialog(int result);
    /**
     * @brief SavePattern save pattern file.
     * @param fileName pattern file name.
     * @return true if all is good.
     */
    bool               SavePattern(const QString &curFile);
    /**
     * @brief AutoSavePattern start safe saving.
     */
    void               AutoSavePattern();
    /**
     * @brief setCurrentFile the function is called to reset the state of a few variables when a file
     * is loaded or saved, or when the user starts editing a new file (in which case fileName is empty).
     * @param fileName file name.
     */
    void               setCurrentFile(const QString &fileName);
    /**
     * @brief strippedName the function call around curFile to exclude the path to the file.
     * @param fullFileName full path to the file.
     * @return file name.
     */
    QString            strippedName(const QString &fullFileName);
    /**
     * @brief ReadSettings read setting for app.
     */
    void               ReadSettings();
    /**
     * @brief WriteSettings save setting for app.
     */
    void               WriteSettings();
    /**
     * @brief MaybeSave The function is called to save pending changes.
     * @return returns true in all cases, except when the user clicks Cancel.
     */
    bool               MaybeSave();
    void               UpdateRecentFileActions();
    void               CreateMenus();
    void               CreateActions();
    void               InitAutoSave();
    QString            PatternPieceName(const QString &text);
};

#endif // MAINWINDOW_H
