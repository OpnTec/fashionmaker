#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QComboBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList list;
    list << "28"<<"30"<<"32"<<"34"<<"36"<<"38"<<"40"<<"42"<<"44"<<"46"<<"48"<<"50" << "52" << "54" << "56";
    QComboBox* comboBoxSize = new QComboBox;
    comboBoxSize ->clear();
    comboBoxSize->addItems(list);
    ui->toolBarOption->addWidget(comboBoxSize);
}

MainWindow::~MainWindow()
{
    delete ui;
}
