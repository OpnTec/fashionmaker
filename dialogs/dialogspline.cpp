#include "dialogspline.h"
#include "ui_dialogspline.h"

DialogSpline::DialogSpline(const VContainer *data, QWidget *parent) :
    DialogTool(data, parent), ui(new Ui::DialogSpline)
{
    ui->setupUi(this);
    number = 0;
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogSpline::DialogAccepted);

    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogSpline::DialogRejected);
    FillComboBoxPoints(ui->comboBoxP1);
    FillComboBoxPoints(ui->comboBoxP4);
}

DialogSpline::~DialogSpline()
{
    delete ui;
}

void DialogSpline::ChoosedObject(qint64 id, Scene::Type type){
    if(type == Scene::Point){
        VPointF point = data->GetPoint(id);
        if(number == 0){
            qint32 index = ui->comboBoxP1->findText(point.name());
            if ( index != -1 ) { // -1 for not found
                ui->comboBoxP1->setCurrentIndex(index);
                number++;
                return;
            }
        }
        if(number == 1){
            qint32 index = ui->comboBoxP4->findText(point.name());
            if ( index != -1 ) { // -1 for not found
                ui->comboBoxP4->setCurrentIndex(index);
                number = 0;
                index = ui->comboBoxP1->currentIndex();
                qint64 p1Id = qvariant_cast<qint64>(ui->comboBoxP1->itemData(index));
                QPointF p1 = data->GetPoint(p1Id);
                QPointF p4 = data->GetPoint(id);
                ui->spinBoxAngle1->setValue(QLineF(p1, p4).angle());
                ui->spinBoxAngle2->setValue(QLineF(p4, p1).angle());
            }
            if(!isInitialized){
                this->show();
            }
        }
    }
}

void DialogSpline::DialogAccepted(){
    qint32 index = ui->comboBoxP1->currentIndex();
    p1 = qvariant_cast<qint64>(ui->comboBoxP1->itemData(index));
    index = ui->comboBoxP4->currentIndex();
    p4 = qvariant_cast<qint64>(ui->comboBoxP4->itemData(index));
    angle1 = ui->spinBoxAngle1->value();
    angle2 = ui->spinBoxAngle2->value();
    kAsm1 = ui->doubleSpinBoxKasm1->value();
    kAsm2 = ui->doubleSpinBoxKasm2->value();
    kCurve = ui->doubleSpinBoxKcurve->value();
    emit DialogClosed(QDialog::Accepted);
}

qreal DialogSpline::getKCurve() const{
    return kCurve;
}

void DialogSpline::setKCurve(const qreal &value){
    kCurve = value;
    ui->doubleSpinBoxKcurve->setValue(value);
}

qreal DialogSpline::getKAsm2() const{
    return kAsm2;
}

void DialogSpline::setKAsm2(const qreal &value){
    kAsm2 = value;
    ui->doubleSpinBoxKasm2->setValue(value);
}

qreal DialogSpline::getKAsm1() const{
    return kAsm1;
}

void DialogSpline::setKAsm1(const qreal &value){
    kAsm1 = value;
    ui->doubleSpinBoxKasm1->setValue(value);
}

qreal DialogSpline::getAngle2() const{
    return angle2;
}

void DialogSpline::setAngle2(const qreal &value){
    angle2 = value;
    ui->spinBoxAngle2->setValue(value);
}

qreal DialogSpline::getAngle1() const{
    return angle1;
}

void DialogSpline::setAngle1(const qreal &value){
    angle1 = value;
    ui->spinBoxAngle1->setValue(value);
}

qint64 DialogSpline::getP4() const{
    return p4;
}

void DialogSpline::setP4(const qint64 &value){
    p4 = value;
    ChangeCurrentData(ui->comboBoxP4, value);
}

qint64 DialogSpline::getP1() const{
    return p1;
}

void DialogSpline::setP1(const qint64 &value){
    p1 = value;
    ChangeCurrentData(ui->comboBoxP1, value);
}
