#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QtXml>
#include <QComboBox>

#include "widgets/vmaingraphicsscene.h"
#include "dialogs/dialogsinglepoint.h"
#include "dialogs/dialogincrements.h"
#include "tools/vtoolsimplepoint.h"
#include "xml/vdomdocument.h"
#include "container/vcontainer.h"

namespace Ui {
class MainWindow;
}

namespace Tools{
    enum Enum
    {
        ArrowTool,
        SinglePointTool
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
    void                clickedToolButtonSinglePoint(bool checked);
    void                triggeredActionAroowTool();
    void                triggeredActionDraw(bool checked);
    void                triggeredActionDetails(bool checked);
    void                ToolCanseled();
    void                SinglePointCreated(const QString name, const QPointF point);
    void                triggeredActionNewDraw();
    void                currentDrawChanged( int index );
    void                triggeredOptionDraw();
    void                triggeredActionSaveAs();
    void                triggeredActionSave();
    void                triggeredActionOpen();
    void                triggeredActionNew();
    void                haveChange();
    void                ChangedSize(const QString &text);
    void                ChangedGrowth(const QString & text);
    void                triggeredActionTable(bool checked);
    void                closedActionTable();
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
};

#endif // MAINWINDOW_H
