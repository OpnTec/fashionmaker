/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QtXml>
#include <QComboBox>
#include <QSharedPointer>
#include <QToolButton>
#include "widgets/vmaingraphicsscene.h"
#include "widgets/vmaingraphicsview.h"
#include "widgets/vitem.h"
#include "dialogs/dialogs.h"
#include "tools/drawTools/drawtools.h"
#include "tools/modelingTools/modelingtools.h"
#include "xml/vdomdocument.h"
#include "container/vcontainer.h"
#include "options.h"
#include "tools/vtooldetail.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT    
public:
    explicit           MainWindow(QWidget *parent = 0);
                       ~MainWindow();
    void               OpenPattern(const QString &fileName);
public slots:
    void               mouseMove(QPointF scenePos);
    void               ActionAroowTool();
    void               ActionDraw(bool checked);
    void               ActionDetails(bool checked);
    void               ActionNewDraw();
    void               ActionSaveAs();
    void               ActionSave();
    void               ActionOpen();
    void               ActionNew();
    void               ActionTable(bool checked);
    void               ActionHistory(bool checked);
    void               ActionLayout(bool checked);
    void               currentDrawChanged( int index );
    void               OptionDraw();
    void               haveChange();
    void               ChangedSize(const QString &text);
    void               ChangedGrowth(const QString & text);
    void               ClosedActionTable();
    void               ClosedActionHistory();
    void               ToolEndLine(bool checked);
    void               ToolLine(bool checked);
    void               ToolAlongLine(bool checked);
    void               ToolShoulderPoint(bool checked);
    void               ToolNormal(bool checked);
    void               ToolBisector(bool checked);
    void               ToolLineIntersect(bool checked);
    void               ToolSpline(bool checked);
    void               ToolArc(bool checked);
    void               ToolSplinePath(bool checked);
    void               ToolPointOfContact(bool checked);
    void               ToolDetail(bool checked);
    void               ClosedDialogEndLine(int result);
    void               ClosedDialogLine(int result);
    void               ClosedDialogAlongLine(int result);
    void               ClosedDialogShoulderPoint(int result);
    void               ClosedDialogNormal(int result);
    void               ClosedDialogBisector(int result);
    void               ClosedDialogLineIntersect(int result);
    void               ClosedDialogSpline(int result);
    void               ClosedDialogArc(int result);
    void               ClosedDialogSplinePath(int result);
    void               ClosedDialogPointOfContact(int result);
    void               ClosedDialogDetail(int result);
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
    virtual void       keyPressEvent ( QKeyEvent * event );
    virtual void       showEvent( QShowEvent *event );
    virtual void       closeEvent( QCloseEvent * event );
    void               Clear();
private:
    Q_DISABLE_COPY(MainWindow)
    Ui::MainWindow     *ui;
    Tool::Tools        tool;
    VMainGraphicsScene *currentScene;
    VMainGraphicsScene *sceneDraw;
    VMainGraphicsScene *sceneDetails;
    QLabel             *mouseCoordinate;
    QLabel             *helpLabel;
    VMainGraphicsView  *view;
    bool               isInitialized;
    DialogIncrements   *dialogTable;
    QSharedPointer<DialogEndLine>        dialogEndLine;
    QSharedPointer<DialogLine>           dialogLine;
    QSharedPointer<DialogAlongLine>      dialogAlongLine;
    QSharedPointer<DialogShoulderPoint>  dialogShoulderPoint;
    QSharedPointer<DialogNormal>         dialogNormal;
    QSharedPointer<DialogBisector>       dialogBisector;
    QSharedPointer<DialogLineIntersect>  dialogLineIntersect;
    QSharedPointer<DialogSpline>         dialogSpline;
    QSharedPointer<DialogArc>            dialogArc;
    QSharedPointer<DialogSplinePath>     dialogSplinePath;
    QSharedPointer<DialogPointOfContact> dialogPointOfContact;
    QSharedPointer<DialogDetail>         dialogDetail;
    DialogHistory      *dialogHistory;
    VDomDocument       *doc;
    VContainer         *data;
    QComboBox          *comboBoxDraws;
    QString            fileName;
    bool               changeInFile;
    Draw::Draws         mode;
    void               ToolBarOption();
    void               ToolBarDraws();
    void               CanselTool();
    void               ArrowTool();
    void               SetEnableWidgets(bool enable);
    void               SetEnableTool(bool enable);
    template <typename Dialog, typename Func>
    void               SetToolButton(bool checked, Tool::Tools t, const QString &cursor,
                                     QSharedPointer<Dialog> &dialog,
                       Func closeDialogSlot);
    void               MinimumScrollBar();
    template <typename T>
    void               AddToolToDetail(T *tool, const qint64 &id, Tool::Tools typeTool,
                                       const qint64 &idDetail);
};

#endif // MAINWINDOW_H
