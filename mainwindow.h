#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QtXml>
#include <QComboBox>
#include <QSharedPointer>

#include "widgets/vmaingraphicsscene.h"
#include "dialogs/dialogsinglepoint.h"
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
#include "tools/vtoolsinglepoint.h"
#include "xml/vdomdocument.h"
#include "container/vcontainer.h"

namespace Ui {
class MainWindow;
}

namespace Tools{
    enum Enum
    {
        ArrowTool,
        SinglePointTool,
        EndLineTool,
        LineTool,
        AlongLineTool,
        ShoulderPointTool,
        NormalTool,
        BisectorTool,
        LineIntersectTool,
        SplineTool,
        ArcTool,
        SplinePathTool
    };
}

class MainWindow : public QMainWindow
{
    Q_OBJECT    
public:
    explicit            MainWindow(QWidget *parent = 0);
                        ~MainWindow();
public slots:
    void                mouseMove(QPointF scenePos);
    void                ToolSinglePoint(bool checked);
    void                ActionAroowTool();
    void                ActionDraw(bool checked);
    void                ActionDetails(bool checked);
    void                ClosedDialogSinglePoint(int result);
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
    bool                isInitialized;
    DialogSinglePoint   *dialogSinglePoint;
    DialogIncrements    *dialogTable;
    DialogEndLine       *dialogEndLine;
    DialogLine          *dialogLine;
    DialogAlongLine     *dialogAlongLine;
    DialogShoulderPoint *dialogShoulderPoint;
    DialogNormal        *dialogNormal;
    DialogBisector      *dialogBisector;
    DialogLineIntersect *dialogLineIntersect;
    DialogSpline        *dialogSpline;
    DialogArc           *dialogArc;
    DialogSplinePath    *dialogSplinePath;
    VDomDocument        *doc;
    VContainer          *data;
    QComboBox           *comboBoxDraws;
    QString             fileName;
    bool                changeInFile;
    void                ToolBarOption();
    void                ToolBarDraws();
    void                CanselTool();
    void                ArrowTool();
    void                CreateManTableIGroup () const;
    void                SetEnableWidgets(bool enable);
    void                SetEnableTool(bool enable);
};

#endif // MAINWINDOW_H
