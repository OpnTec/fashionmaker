#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include <QMainWindow>
#include <QLabel>
#include <QtXml>
#include <QComboBox>
#include <QSharedPointer>
#include <QToolButton>
#include <QSharedPointer>
#include "widgets/vmaingraphicsscene.h"
#include "widgets/vmaingraphicsview.h"
#include "dialogs/dialogincrements.h"
#include "dialogs/dialogline.h"
#include "dialogs/dialogalongline.h"
#include "dialogs/dialogshoulderpoint.h"
#include "dialogs/dialogendline.h"
#include "dialogs/dialognormal.h"
#include "dialogs/dialogbisector.h"
#include "dialogs/dialoglineintersect.h"
#include "dialogs/dialogspline.h"
#include "dialogs/dialogarc.h"
#include "dialogs/dialogsplinepath.h"
#include "dialogs/dialoghistory.h"
#include "tools/vtoolsinglepoint.h"
#include "xml/vdomdocument.h"
#pragma GCC diagnostic pop
#include "container/vcontainer.h"
#include "options.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT    
public:
    explicit            MainWindow(QWidget *parent = 0);
                        ~MainWindow();
public slots:
    void                mouseMove(QPointF scenePos);
    void                ActionAroowTool();
    void                ActionDraw(bool checked);
    void                ActionDetails(bool checked);
    void                ActionNewDraw();
    void                currentDrawChanged( int index );
    void                OptionDraw();
    void                ActionSaveAs();
    void                ActionSave();
    void                ActionOpen();
    void                ActionNew();
    void                haveChange();
    void                ChangedSize(const QString &text);
    void                ChangedGrowth(const QString & text);
    void                ActionTable(bool checked);
    void                ClosedActionTable();
    void                ActionHistory(bool checked);
    void                ClosedActionHistory();
    void                ToolEndLine(bool checked);
    void                ClosedDialogEndLine(int result);
    void                ToolLine(bool checked);
    void                ClosedDialogLine(int result);
    void                ToolAlongLine(bool checked);
    void                ClosedDialogAlongLine(int result);
    void                ToolShoulderPoint(bool checked);
    void                ClosedDialogShoulderPoint(int result);
    void                ToolNormal(bool checked);
    void                ClosedDialogNormal(int result);
    void                ToolBisector(bool checked);
    void                ClosedDialogBisector(int result);
    void                ToolLineIntersect(bool checked);
    void                ClosedDialogLineIntersect(int result);
    void                ToolSpline(bool checked);
    void                ClosedDialogSpline(int result);
    void                ToolArc(bool checked);
    void                ClosedDialogArc(int result);
    void                ToolSplinePath(bool checked);
    void                ClosedDialogSplinePath(int result);
protected:
    virtual void        keyPressEvent ( QKeyEvent * event );
    virtual void        showEvent( QShowEvent *event );
private:
    Ui::MainWindow      *ui;
    Tools::Enum         tool;
    VMainGraphicsScene  *scene;
    QLabel              *mouseCoordinate;
    QLabel              *helpLabel;
    VMainGraphicsView   *view;
    bool                isInitialized;
    DialogIncrements    *dialogTable;
    QSharedPointer<DialogEndLine>       dialogEndLine;
    QSharedPointer<DialogLine>          dialogLine;
    QSharedPointer<DialogAlongLine>     dialogAlongLine;
    QSharedPointer<DialogShoulderPoint> dialogShoulderPoint;
    QSharedPointer<DialogNormal>        dialogNormal;
    QSharedPointer<DialogBisector>      dialogBisector;
    QSharedPointer<DialogLineIntersect> dialogLineIntersect;
    QSharedPointer<DialogSpline>        dialogSpline;
    QSharedPointer<DialogArc>           dialogArc;
    QSharedPointer<DialogSplinePath>    dialogSplinePath;
    DialogHistory       *dialogHistory;
    VDomDocument        *doc;
    VContainer          *data;
    QComboBox           *comboBoxDraws;
    QString             fileName;
    bool                changeInFile;
    MainWindow(const MainWindow &window);
    const MainWindow &operator=(const MainWindow &window);
    void                ToolBarOption();
    void                ToolBarDraws();
    void                CanselTool();
    void                ArrowTool();
    void                SetEnableWidgets(bool enable);
    void                SetEnableTool(bool enable);
    template <typename Dialog, typename Func>
    void SetToolButton(bool checked, Tools::Enum t, const QString &cursor, QSharedPointer<Dialog> &dialog,
                                   Func closeDialogSlot);
};

#endif // MAINWINDOW_H
