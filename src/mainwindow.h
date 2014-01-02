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
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
              /**
               * @brief MainWindow
               * @param parent
               */
              explicit MainWindow(QWidget *parent = 0);
                       ~MainWindow();
    /**
     * @brief OpenPattern
     * @param fileName
     */
    void               OpenPattern(const QString &fileName);
public slots:
    /**
     * @brief mouseMove
     * @param scenePos
     */
    void               mouseMove(const QPointF &scenePos);
    /**
     * @brief ActionAroowTool
     */
    void               ActionAroowTool();
    /**
     * @brief ActionDraw
     * @param checked
     */
    void               ActionDraw(bool checked);
    /**
     * @brief ActionDetails
     * @param checked
     */
    void               ActionDetails(bool checked);
    /**
     * @brief ActionNewDraw
     */
    void               ActionNewDraw();
    /**
     * @brief ActionSaveAs
     */
    void               ActionSaveAs();
    /**
     * @brief ActionSave
     */
    void               ActionSave();
    /**
     * @brief ActionOpen
     */
    void               ActionOpen();
    /**
     * @brief ActionNew
     */
    void               ActionNew();
    /**
     * @brief ActionTable
     * @param checked
     */
    void               ActionTable(bool checked);
    /**
     * @brief ActionHistory
     * @param checked
     */
    void               ActionHistory(bool checked);
    /**
     * @brief ActionLayout
     * @param checked
     */
    void               ActionLayout(bool checked);
    /**
     * @brief currentDrawChanged
     * @param index
     */
    void               currentDrawChanged( int index );
    /**
     * @brief OptionDraw
     */
    void               OptionDraw();
    /**
     * @brief haveChange
     */
    void               haveChange();
    /**
     * @brief ChangedSize
     * @param text
     */
    void               ChangedSize(const QString &text);
    /**
     * @brief ChangedGrowth
     * @param text
     */
    void               ChangedGrowth(const QString & text);
    /**
     * @brief ClosedActionTable
     */
    void               ClosedActionTable();
    /**
     * @brief ClosedActionHistory
     */
    void               ClosedActionHistory();
    /**
     * @brief ToolEndLine
     * @param checked
     */
    void               ToolEndLine(bool checked);
    /**
     * @brief ToolLine
     * @param checked
     */
    void               ToolLine(bool checked);
    /**
     * @brief ToolAlongLine
     * @param checked
     */
    void               ToolAlongLine(bool checked);
    /**
     * @brief ToolShoulderPoint
     * @param checked
     */
    void               ToolShoulderPoint(bool checked);
    /**
     * @brief ToolNormal
     * @param checked
     */
    void               ToolNormal(bool checked);
    /**
     * @brief ToolBisector
     * @param checked
     */
    void               ToolBisector(bool checked);
    /**
     * @brief ToolLineIntersect
     * @param checked
     */
    void               ToolLineIntersect(bool checked);
    /**
     * @brief ToolSpline
     * @param checked
     */
    void               ToolSpline(bool checked);
    /**
     * @brief ToolCutSpline handler tool CutSpline
     * @param checked true - button is checked
     */
    void               ToolCutSpline(bool checked);
    /**
     * @brief ToolArc
     * @param checked
     */
    void               ToolArc(bool checked);
    /**
     * @brief ToolSplinePath
     * @param checked
     */
    void               ToolSplinePath(bool checked);
    /**
     * @brief ToolCutSplinePath handler tool CutSplinePath
     * @param checked true - button is checked
     */
    void               ToolCutSplinePath(bool checked);
    /**
     * @brief ToolPointOfContact
     * @param checked
     */
    void               ToolPointOfContact(bool checked);
    /**
     * @brief ToolDetail
     * @param checked
     */
    void               ToolDetail(bool checked);
    /**
     * @brief ToolHeight
     * @param checked
     */
    void               ToolHeight(bool checked);
    /**
     * @brief ToolTriangle
     * @param checked
     */
    void               ToolTriangle(bool checked);
    /**
     * @brief ToolPointOfIntersection
     * @param checked
     */
    void               ToolPointOfIntersection(bool checked);
    void               ToolUnionDetails(bool checked);
    /**
     * @brief ClosedDialogEndLine
     * @param result
     */
    void               ClosedDialogEndLine(int result);
    /**
     * @brief ClosedDialogLine
     * @param result
     */
    void               ClosedDialogLine(int result);
    /**
     * @brief ClosedDialogAlongLine
     * @param result
     */
    void               ClosedDialogAlongLine(int result);
    /**
     * @brief ClosedDialogShoulderPoint
     * @param result
     */
    void               ClosedDialogShoulderPoint(int result);
    /**
     * @brief ClosedDialogNormal
     * @param result
     */
    void               ClosedDialogNormal(int result);
    /**
     * @brief ClosedDialogBisector
     * @param result
     */
    void               ClosedDialogBisector(int result);
    /**
     * @brief ClosedDialogLineIntersect
     * @param result
     */
    void               ClosedDialogLineIntersect(int result);
    /**
     * @brief ClosedDialogSpline
     * @param result
     */
    void               ClosedDialogSpline(int result);
    /**
     * @brief ClosedDialogArc
     * @param result
     */
    void               ClosedDialogArc(int result);
    /**
     * @brief ClosedDialogSplinePath
     * @param result
     */
    void               ClosedDialogSplinePath(int result);
    /**
     * @brief ClosedDialogCutSplinePath handler close event tool CutSplinePath
     * @param result result of working of dialog
     */
    void               ClosedDialogCutSplinePath(int result);
    /**
     * @brief ClosedDialogPointOfContact
     * @param result
     */
    void               ClosedDialogPointOfContact(int result);
    /**
     * @brief ClosedDialogDetail
     * @param result
     */
    void               ClosedDialogDetail(int result);
    /**
     * @brief ClosedDialogHeight
     * @param result
     */
    void               ClosedDialogHeight(int result);
    /**
     * @brief ClosedDialogTriangle
     * @param result
     */
    void               ClosedDialogTriangle(int result);
    /**
     * @brief ClosedDialogPointOfIntersection
     * @param result
     */
    void               ClosedDialogPointOfIntersection(int result);
    void               ClosedDialogUnionDetails(int result);
    /**
     * @brief ClosedDialogCutSpline handler close event tool CutSpline
     * @param result result of working of dialog
     */
    void               ClosedDialogCutSpline(int result);
    /**
     * @brief About
     */
    void               About();
    /**
     * @brief AboutQt
     */
    void               AboutQt();
    /**
     * @brief ShowToolTip
     * @param toolTip
     */
    void               ShowToolTip(const QString &toolTip);
    /**
     * @brief tableClosed Слот, що виконується при отриманні сигналу закриття вікна укладання
     *деталей моделі.
     */
    void               tableClosed();
signals:
    /**
     * @brief ModelChosen Сигнал, що висилається після розрахунку всіх деталей моделі.
     * @param listDetails Список детайле моделі.
     */
    void               ModelChosen(QVector<VItem*> listDetails);
protected:
    /**
     * @brief keyPressEvent
     * @param event
     */
    virtual void       keyPressEvent ( QKeyEvent * event );
    /**
     * @brief showEvent
     * @param event
     */
    virtual void       showEvent( QShowEvent *event );
    /**
     * @brief closeEvent
     * @param event
     */
    virtual void       closeEvent( QCloseEvent * event );
    /**
     * @brief Clear
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
    VDomDocument      *doc;
    /**
     * @brief tool
     */
    Tool::Tools        tool;
    /**
     * @brief currentScene
     */
    VMainGraphicsScene *currentScene;
    /**
     * @brief sceneDraw
     */
    VMainGraphicsScene *sceneDraw;
    /**
     * @brief sceneDetails
     */
    VMainGraphicsScene *sceneDetails;
    /**
     * @brief mouseCoordinate
     */
    QLabel             *mouseCoordinate;
    /**
     * @brief helpLabel
     */
    QLabel             *helpLabel;
    /**
     * @brief view
     */
    VMainGraphicsView  *view;
    /**
     * @brief isInitialized
     */
    bool               isInitialized;
    /**
     * @brief dialogTable
     */
    DialogIncrements   *dialogTable;
    /**
     * @brief dialogEndLine
     */
    QSharedPointer<DialogEndLine>             dialogEndLine;
    /**
     * @brief dialogLine
     */
    QSharedPointer<DialogLine>                dialogLine;
    /**
     * @brief dialogAlongLine
     */
    QSharedPointer<DialogAlongLine>           dialogAlongLine;
    /**
     * @brief dialogShoulderPoint
     */
    QSharedPointer<DialogShoulderPoint>       dialogShoulderPoint;
    /**
     * @brief dialogNormal
     */
    QSharedPointer<DialogNormal>              dialogNormal;
    /**
     * @brief dialogBisector
     */
    QSharedPointer<DialogBisector>            dialogBisector;
    /**
     * @brief dialogLineIntersect
     */
    QSharedPointer<DialogLineIntersect>       dialogLineIntersect;
    /**
     * @brief dialogSpline
     */
    QSharedPointer<DialogSpline>              dialogSpline;
    /**
     * @brief dialogArc
     */
    QSharedPointer<DialogArc>                 dialogArc;
    /**
     * @brief dialogSplinePath
     */
    QSharedPointer<DialogSplinePath>          dialogSplinePath;
    /**
     * @brief dialogPointOfContact
     */
    QSharedPointer<DialogPointOfContact>      dialogPointOfContact;
    /**
     * @brief dialogDetail
     */
    QSharedPointer<DialogDetail>              dialogDetail;
    /**
     * @brief dialogHeight
     */
    QSharedPointer<DialogHeight>              dialogHeight;
    /**
     * @brief dialogTriangle
     */
    QSharedPointer<DialogTriangle>            dialogTriangle;
    /**
     * @brief dialogPointOfIntersection
     */
    QSharedPointer<DialogPointOfIntersection> dialogPointOfIntersection;
    /**
     * @brief dialogCutSpline pointer to the dialog tool cut spline
     */
    QSharedPointer<DialogCutSpline>           dialogCutSpline;
    /**
     * @brief dialogCutSplinePath pointer to the dialog tool cut spline path
     */
    QSharedPointer<DialogCutSplinePath>       dialogCutSplinePath;
    /**
     * @brief dialogUnionDetails
     */
    QSharedPointer<DialogUnionDetails>        dialogUnionDetails;
    /**
     * @brief dialogHistory
     */
    DialogHistory      *dialogHistory;
    /**
     * @brief comboBoxDraws
     */
    QComboBox          *comboBoxDraws;
    /**
     * @brief fileName
     */
    QString            fileName;
    /**
     * @brief changeInFile
     */
    bool               changeInFile;
    /**
     * @brief mode
     */
    Draw::Draws        mode;
    qint32             currentDrawIndex;
    /**
     * @brief ToolBarOption
     */
    void               ToolBarOption();
    /**
     * @brief ToolBarDraws
     */
    void               ToolBarDraws();
    /**
     * @brief CanselTool
     */
    void               CanselTool();
    /**
     * @brief ArrowTool
     */
    void               ArrowTool();
    /**
     * @brief SetEnableWidgets
     * @param enable
     */
    void               SetEnableWidgets(bool enable);
    /**
     * @brief SetEnableTool
     * @param enable
     */
    void               SetEnableTool(bool enable);
    /**
     *
     */
    template <typename Dialog, typename Func>
    /**
     * @brief SetToolButton
     * @param checked
     * @param t
     * @param cursor
     * @param toolTip
     * @param dialog
     * @param closeDialogSlot
     */
    void               SetToolButton(bool checked, Tool::Tools t, const QString &cursor, const QString &toolTip,
                                     QSharedPointer<Dialog> &dialog, Func closeDialogSlot);
    /**
     * @brief MinimumScrollBar
     */
    void               MinimumScrollBar();
    template <typename T>
    /**
     * @brief AddToolToDetail
     * @param tool
     * @param id
     * @param typeTool
     * @param idDetail
     */
    void               AddToolToDetail(T *tool, const qint64 &id, Tool::Tools typeTool,
                                       const qint64 &idDetail);
    template <typename DrawTool, typename Dialog>
    /**
     * @brief ClosedDialog
     * @param dialog
     * @param result
     */
    void               ClosedDialog(QSharedPointer<Dialog> &dialog, int result);
    /**
     * @brief SafeSaveing
     * @param fileName
     * @return
     */
    bool               SafeSaveing(const QString &fileName)const;
    /**
     * @brief AutoSavePattern
     */
    void               AutoSavePattern();
};

#endif // MAINWINDOW_H
