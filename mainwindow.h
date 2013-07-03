#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "widgets/vmaingraphicsscene.h"
#include "dialogs/dialogsinglepoint.h"

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
    void                showEvent( QShowEvent *event );
    void                clickedToolButtonSinglePoint();
    void                triggeredActionAroowTool();
private:
    Ui::MainWindow      *ui;
    Tools::Enum   tool;
    VMainGraphicsScene  *scene;
    QLabel              *mouseCoordinate;
    QLabel              *helpLabel;
    bool                isInitialized;
    DialogSinglePoint   *dialogSinglePoint;
    void                ToolBarOption();
    void                ToolBarDraws();
    void                CanselTool();
    void                ArrowTool();
};

#endif // MAINWINDOW_H
