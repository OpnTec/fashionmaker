#include "dialogsinglepoint.h"
#include "ui_dialogsinglepoint.h"
#include <QShowEvent>
#include <QPushButton>

#include "../options.h"

DialogSinglePoint::DialogSinglePoint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSinglePoint)
{
    ui->setupUi(this);
    isInitialized = false;
    ui->spinBoxX->setRange(0,(qint32)(PaperSize*PrintDPI/25.4));
    ui->spinBoxY->setRange(0,(qint32)(PaperSize*PrintDPI/25.4));
    QPushButton* pOkButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    pOkButton->setEnabled(false);
    connect(ui->lineEditName, &QLineEdit::textChanged, this, &DialogSinglePoint::NameChanged);
}

void DialogSinglePoint::mousePress(QPointF scenePos){
    if(isInitialized == false){
        ui->spinBoxX->setValue((qint32)(scenePos.x()*PrintDPI/25.4));
        ui->spinBoxY->setValue((qint32)(scenePos.y()*PrintDPI/25.4));
        this->show();
    } else {
        ui->spinBoxX->setValue((qint32)(scenePos.x()*PrintDPI/25.4));
        ui->spinBoxY->setValue((qint32)(scenePos.y()*PrintDPI/25.4));
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

DialogSinglePoint::~DialogSinglePoint()
{
    delete ui;
}
