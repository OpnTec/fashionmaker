#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QtXml>
#include <QComboBox>

#include "widgets/vmaingraphicsscene.h"
#include "dialogs/dialogsinglepoint.h"
#include "dialogs/dialogincrements.h"
#include "dialogs/dialogline.h"
#include "tools/vtoolsimplepoint.h"
#include "xml/vdomdocument.h"
#include "container/vcontainer.h"
#include "dialogs/dialogendline.h"

namespace Ui {
class MainWindow;
}

namespace Tools{
    enum Enum
    {
        ArrowTool,
        SinglePointTool,
        EndLineTool,
        LineTool
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
    void                ToolCanseled();
    void                SinglePointCreated(const QString name, const QPointF point);
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
protected:
    virtual void        keyPressEvent ( QKeyEvent * event );
    virtual void        showEvent( QShowEvent *event );
    virtual void        closeEvent ( QCloseEvent * event );
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
