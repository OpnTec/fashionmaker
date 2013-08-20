#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include "dialogsinglepoint.h"
#include "ui_dialogsinglepoint.h"
#include <QShowEvent>
#include <QPushButton>
#include <QDebug>
#pragma GCC diagnostic pop
#include "../options.h"

DialogSinglePoint::DialogSinglePoint(const VContainer *data, QWidget *parent) :
    DialogTool(data, parent), ui(new Ui::DialogSinglePoint), name(QString()), point(QPointF()){
    ui->setupUi(this);
    ui->doubleSpinBoxX->setRange(0,PaperSize/PrintDPI*25.4);
    ui->doubleSpinBoxY->setRange(0,PaperSize/PrintDPI*25.4);
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    flagName = false;
    CheckState();
    connect(bOk, &QPushButton::clicked, this, &DialogSinglePoint::DialogAccepted);
    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogSinglePoint::DialogRejected);
    connect(ui->lineEditName,&QLineEdit::textChanged, this, &DialogSinglePoint::NamePointChanged);
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

void DialogSinglePoint::DialogAccepted(){
    point = QPointF(ui->doubleSpinBoxX->value()*PrintDPI/25.4,
                    ui->doubleSpinBoxY->value()*PrintDPI/25.4);
    name = ui->lineEditName->text();
    emit DialogClosed(QDialog::Accepted);
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

