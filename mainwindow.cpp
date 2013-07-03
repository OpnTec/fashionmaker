#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QScrollBar>
#include <QShowEvent>

#include "options.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tool = Tools::ArrayTool;
    isInitialized = false;
    ToolBarOption();
    ToolBarDraws();
    QRectF sceneRect = QRectF(0, 0, PaperSize*PrintDPI/25.4, PaperSize*PrintDPI/25.4);
    scene = new VMainGraphicsScene(sceneRect);
    ui->graphicsView->setScene(scene);

    connect(scene, &VMainGraphicsScene::mouseMove, this, &MainWindow::mouseMove);
    connect(ui->toolButtonSinglePoint, &QToolButton::clicked, this,
            &MainWindow::clickedToolButtonSinglePoint);
    helpLabel = new QLabel("Створіть новий файл для початку роботи.");
    ui->statusBar->addWidget(helpLabel);

    connect(ui->actionArrowTool, &QAction::triggered, this, &MainWindow::triggeredActionAroowTool);
}

/*
 * Інструмет базова точка креслення.
 */
void MainWindow::clickedToolButtonSinglePoint(){
    tool = Tools::SinglePointTool;
    QPixmap pixmap(":/cursor/spoint_cursor.png");
    QCursor cur(pixmap, 2, 3);
    ui->graphicsView->setCursor(cur);
    helpLabel->setText("Виберіть розташування для точки.");
    dialogSinglePoint = new DialogSinglePoint;
    connect(scene, &VMainGraphicsScene::mousePress, dialogSinglePoint, &DialogSinglePoint::mousePress);
}

void MainWindow::showEvent( QShowEvent *event ){
    QMainWindow::showEvent( event );
    if( event->spontaneous() ){
        return;
    }

    if(isInitialized){
        return;
    }

    // do your init stuff here
    QScrollBar *horScrollBar = ui->graphicsView->horizontalScrollBar();
    horScrollBar->setValue(horScrollBar->minimum());
    QScrollBar *verScrollBar = ui->graphicsView->verticalScrollBar();
    verScrollBar->setValue(verScrollBar->minimum());

    isInitialized = true;//перший показ вікна вже відбувся
}

void MainWindow::ToolBarOption(){
    QLabel * labelGrowth = new QLabel;
    labelGrowth->setText("Зріст: ");
    ui->toolBarOption->addWidget(labelGrowth);

    QStringList list;
    list << "104"<<"110"<<"116"<<"122"<<"128"<<"134"<<"140"<<"146"<<"152"<<"158"<<"164"<<"170"<<"176"
         << "182" << "188";
    QComboBox* comboBoxGrow = new QComboBox;
    comboBoxGrow->clear();
    comboBoxGrow->addItems(list);
    comboBoxGrow->setCurrentIndex(12);
    ui->toolBarOption->addWidget(comboBoxGrow);

    QLabel * labelSize = new QLabel;
    labelSize->setText(" Розмір: ");
    ui->toolBarOption->addWidget(labelSize);

    list.clear();
    list << "28"<<"30"<<"32"<<"34"<<"36"<<"38"<<"40"<<"42"<<"44"<<"46"<<"48"<<"50" << "52" << "54" << "56";
    QComboBox* comboBoxSize = new QComboBox;
    comboBoxSize->clear();
    comboBoxSize->addItems(list);
    comboBoxSize->setCurrentIndex(11);
    ui->toolBarOption->addWidget(comboBoxSize);

    ui->toolBarOption->addSeparator();

    mouseCoordinate = new QLabel;
    mouseCoordinate ->setText("0, 0");
    ui->toolBarOption->addWidget(mouseCoordinate);

}

void MainWindow::ToolBarDraws(){
    QLabel * labelNameDraw = new QLabel;
    labelNameDraw ->setText("Креслення: ");
    ui->toolBarDraws->addWidget(labelNameDraw);

    QComboBox* comboBoxDraws = new QComboBox;
    ui->toolBarDraws->addWidget(comboBoxDraws);

    ui->toolBarDraws->addSeparator();

    QLabel* labelTranslateX = new QLabel;
    labelTranslateX ->setText(" Зміщення по Х: ");
    ui->toolBarDraws->addWidget(labelTranslateX);

    QSpinBox* spinBoxTranslateX = new QSpinBox;
    spinBoxTranslateX->setRange(0,(qint32)(PaperSize*PrintDPI/25.4));
    spinBoxTranslateX->setFixedSize(80,25);
    ui->toolBarDraws->addWidget(spinBoxTranslateX);

    QLabel* labelTranslateY = new QLabel;
    labelTranslateY ->setText(" Зміщення по Y: ");
    ui->toolBarDraws->addWidget(labelTranslateY);

    QSpinBox* spinBoxTranslateY = new QSpinBox;
    spinBoxTranslateY->setRange(0,(qint32)(PaperSize*PrintDPI/25.4));
    spinBoxTranslateY->setFixedSize(80,25);
    ui->toolBarDraws->addWidget(spinBoxTranslateY);

    QPushButton* pushButtonTranslate = new QPushButton;
    pushButtonTranslate->setText("Застосувати");
    ui->toolBarDraws->addWidget(pushButtonTranslate);
}

void MainWindow::mouseMove(QPointF scenePos){
    QString string = QString("%1, %2")
                            .arg((qint32)(scenePos.x()*PrintDPI/25.4))
                            .arg((qint32)(scenePos.y()*PrintDPI/25.4));
    mouseCoordinate->setText(string);
}

void MainWindow::CanselTool(){
    switch( tool )
    {
        case Tools::ArrowTool:
            //Покищо нічого тут не робимо.
            break;
        case Tools::SinglePointTool:
            //Знищимо діалогове вікно.
            delete dialogSinglePoint;
            break;
    }
}

void  MainWindow::ArrowTool(){
    CanselTool();
    tool = Tools::ArrowTool;
    QCursor cur(Qt::ArrowCursor);
    ui->graphicsView->setCursor(cur);
    helpLabel->setText("");
}

void MainWindow::triggeredActionAroowTool(){
    ArrowTool();
}

MainWindow::~MainWindow()
{
    CanselTool();
    delete ui;
}
