#include "dialogsinglepoint.h"
#include "ui_dialogsinglepoint.h"
#include <QShowEvent>
#include <QPushButton>
#include <QDebug>

#include "../options.h"

DialogSinglePoint::DialogSinglePoint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSinglePoint)
{
    ui->setupUi(this);
    isInitialized = false;
    ui->doubleSpinBoxX->setRange(0,PaperSize/PrintDPI*25.4);
    ui->doubleSpinBoxY->setRange(0,PaperSize/PrintDPI*25.4);
    QPushButton* pOkButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    pOkButton->setEnabled(false);
    connect(pOkButton, &QPushButton::clicked, this, &DialogSinglePoint::OkOperation);
    connect(ui->lineEditName, &QLineEdit::textChanged, this, &DialogSinglePoint::NameChanged);
    QPushButton* pCanselButton = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(pCanselButton, &QPushButton::clicked, this, &DialogSinglePoint::CanselOperation);
}

void DialogSinglePoint::mousePress(QPointF scenePos){
    if(isInitialized == false){
        ui->doubleSpinBoxX->setValue(scenePos.x()/PrintDPI*25.4);
        ui->doubleSpinBoxY->setValue(scenePos.y()/PrintDPI*25.4);
        this->show();
    } else {
        ui->doubleSpinBoxX->setValue(scenePos.x()/PrintDPI*25.4);
        ui->doubleSpinBoxY->setValue(scenePos.y()/PrintDPI*25.4);
    }
}

void DialogSinglePoint::showEvent( QShowEvent *event ){
    QDialog::showEvent( event );
    if( event->spontaneous() ){
        return;
    }

    if(isInitialized){
        return;
    }

    // do your init stuff here

    isInitialized = true;//перший показ вікна вже відбувся
}

void DialogSinglePoint::NameChanged(){
    QString name = ui->lineEditName->text();
    if(name.isEmpty() || name.contains(" ")){
        QPushButton* pOkButton = ui->buttonBox->button(QDialogButtonBox::Ok);
        pOkButton->setEnabled(false);
    } else {
        QPushButton* pOkButton = ui->buttonBox->button(QDialogButtonBox::Ok);
        pOkButton->setEnabled(true);
    }
}

void DialogSinglePoint::CanselOperation(){
    emit ToolCanseled();
}

void DialogSinglePoint::OkOperation(){
    point = QPointF(ui->doubleSpinBoxX->value()*PrintDPI/25.4,
                    ui->doubleSpinBoxY->value()*PrintDPI/25.4);
    name = ui->lineEditName->text();
    emit SinglePointCreated(ui->lineEditName->text(), point);
}

void DialogSinglePoint::closeEvent ( QCloseEvent * event ){
    emit ToolCanseled();
    event->accept();
}

void DialogSinglePoint::setData(const QString name, const QPointF point){
    this->name = name;
    this->point = point;
    isInitialized = true;
    ui->lineEditName->setText(name);
    ui->doubleSpinBoxX->setValue(point.x()/PrintDPI*25.4);
    ui->doubleSpinBoxY->setValue(point.y()/PrintDPI*25.4);
}

QString DialogSinglePoint::getName()const{
    return name;
}

QPointF DialogSinglePoint::getPoint()const{
    return point;
}

DialogSinglePoint::~DialogSinglePoint(){
    delete ui;
}

