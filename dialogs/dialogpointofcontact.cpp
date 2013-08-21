#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include "dialogpointofcontact.h"
#pragma GCC diagnostic pop

DialogPointOfContact::DialogPointOfContact(const VContainer *data, QWidget *parent) :
    DialogTool(data, parent){
    ui.setupUi(this);
    number = 0;
    listWidget = ui.listWidget;
    labelResultCalculation = ui.labelResultCalculation;
    labelDescription = ui.labelDescription;
    radioButtonSizeGrowth = ui.radioButtonSizeGrowth;
    radioButtonStandartTable = ui.radioButtonStandartTable;
    radioButtonIncrements = ui.radioButtonIncrements;
    radioButtonLengthLine = ui.radioButtonLengthLine;
    lineEditFormula = ui.lineEditFormula;
    flagFormula = false;
    bOk = ui.buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogPointOfContact::DialogAccepted);
    flagName = false;
    CheckState();
    QPushButton *bCansel = ui.buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogPointOfContact::DialogRejected);
    FillComboBoxPoints(ui.comboBoxCenter);
    FillComboBoxPoints(ui.comboBoxFirstPoint);
    FillComboBoxPoints(ui.comboBoxSecondPoint);

    connect(ui.toolButtonPutHere, &QPushButton::clicked, this, &DialogPointOfContact::PutHere);
    connect(ui.listWidget, &QListWidget::itemDoubleClicked, this, &DialogPointOfContact::PutVal);
    connect(ui.listWidget, &QListWidget::currentRowChanged, this, &DialogPointOfContact::ValChenged);

    ShowVariable(data->DataBase());
    connect(ui.radioButtonSizeGrowth, &QRadioButton::clicked, this, &DialogPointOfContact::SizeGrowth);
    connect(ui.radioButtonStandartTable, &QRadioButton::clicked, this, &DialogPointOfContact::StandartTable);
    connect(ui.radioButtonIncrements, &QRadioButton::clicked, this, &DialogPointOfContact::Increments);
    connect(ui.radioButtonLengthLine, &QRadioButton::clicked, this, &DialogPointOfContact::LengthLines);
    connect(ui.toolButtonEqual, &QPushButton::clicked, this, &DialogPointOfContact::EvalFormula);
    connect(ui.lineEditNamePoint, &QLineEdit::textChanged, this, &DialogPointOfContact::NamePointChanged);
    connect(ui.lineEditFormula, &QLineEdit::textChanged, this, &DialogPointOfContact::FormulaChanged);
}

void DialogPointOfContact::ChoosedObject(qint64 id, Scene::Type type){
    if(type == Scene::Point){
        VPointF point = data->GetPoint(id);
        if(number == 0){
            qint32 index = ui.comboBoxFirstPoint->findText(point.name());
            if ( index != -1 ) { // -1 for not found
                ui.comboBoxFirstPoint->setCurrentIndex(index);
                number++;
                return;
            }
        }
        if(number == 1){
            qint32 index = ui.comboBoxSecondPoint->findText(point.name());
            if ( index != -1 ) { // -1 for not found
                ui.comboBoxSecondPoint->setCurrentIndex(index);
                number++;
                return;
            }
        }
        if(number == 2){
            qint32 index = ui.comboBoxCenter->findText(point.name());
            if ( index != -1 ) { // -1 for not found
                ui.comboBoxCenter->setCurrentIndex(index);
                number = 0;
            }
            if(!isInitialized){
                this->show();
            }
        }
    }
}


void DialogPointOfContact::DialogAccepted(){
    pointName = ui.lineEditNamePoint->text();
    radius = ui.lineEditFormula->text();
    center = getCurrentPointId(ui.comboBoxCenter);
    firstPoint = getCurrentPointId(ui.comboBoxFirstPoint);
    secondPoint = getCurrentPointId(ui.comboBoxSecondPoint);
    emit DialogClosed(QDialog::Accepted);
}

qint64 DialogPointOfContact::getSecondPoint() const{
    return secondPoint;
}

void DialogPointOfContact::setSecondPoint(const qint64 &value, const qint64 &id){
    setCurrentPointId(ui.comboBoxSecondPoint, secondPoint, value, id);
}

qint64 DialogPointOfContact::getFirstPoint() const{
    return firstPoint;
}

void DialogPointOfContact::setFirstPoint(const qint64 &value, const qint64 &id){
    setCurrentPointId(ui.comboBoxFirstPoint, firstPoint, value, id);
}

qint64 DialogPointOfContact::getCenter() const{
    return center;
}

void DialogPointOfContact::setCenter(const qint64 &value, const qint64 &id){
    setCurrentPointId(ui.comboBoxCenter, center, value, id);
    center = value;
}

QString DialogPointOfContact::getRadius() const{
    return radius;
}

void DialogPointOfContact::setRadius(const QString &value){
    radius = value;
    ui.lineEditFormula->setText(radius);
}

QString DialogPointOfContact::getPointName() const{
    return pointName;
}

void DialogPointOfContact::setPointName(const QString &value){
    pointName = value;
    ui.lineEditNamePoint->setText(pointName);
}
