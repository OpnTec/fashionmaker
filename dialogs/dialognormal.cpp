#include "dialognormal.h"
#include "ui_dialognormal.h"
#include <QMenu>

DialogNormal::DialogNormal(const VContainer *data, QWidget *parent) :
    DialogTool(data, parent), ui(new Ui::DialogNormal)
{
    ui->setupUi(this);
    number = 0;
    spinBoxAngle = ui->spinBoxAngle;
    listWidget = ui->listWidget;
    labelResultCalculation = ui->labelResultCalculation;
    labelDescription = ui->labelDescription;
    radioButtonSizeGrowth = ui->radioButtonSizeGrowth;
    radioButtonStandartTable = ui->radioButtonStandartTable;
    radioButtonIncrements = ui->radioButtonIncrements;
    radioButtonLengthLine = ui->radioButtonLengthLine;
    lineEditFormula = ui->lineEditFormula;
    flagFormula = false;
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogNormal::DialogAccepted);
    flagName = false;
    CheckState();
    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogNormal::DialogRejected);
    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);
    FillComboBoxTypeLine(ui->comboBoxLineType);

    connect(ui->toolButtonArrowDown, &QPushButton::clicked, this,
            &DialogNormal::ArrowDown);
    connect(ui->toolButtonArrowUp, &QPushButton::clicked, this,
            &DialogNormal::ArrowUp);
    connect(ui->toolButtonArrowLeft, &QPushButton::clicked, this,
            &DialogNormal::ArrowLeft);
    connect(ui->toolButtonArrowRight, &QPushButton::clicked, this,
            &DialogNormal::ArrowRight);
    connect(ui->toolButtonArrowLeftUp, &QPushButton::clicked, this,
            &DialogNormal::ArrowLeftUp);
    connect(ui->toolButtonArrowLeftDown, &QPushButton::clicked, this,
            &DialogNormal::ArrowLeftDown);
    connect(ui->toolButtonArrowRightUp, &QPushButton::clicked, this,
            &DialogNormal::ArrowRightUp);
    connect(ui->toolButtonArrowRightDown, &QPushButton::clicked, this,
            &DialogNormal::ArrowRightDown);
    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogNormal::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogNormal::PutVal);
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogNormal::ValChenged);

    ShowVariable(data->DataBase());
    connect(ui->radioButtonSizeGrowth, &QRadioButton::clicked, this, &DialogNormal::SizeGrowth);
    connect(ui->radioButtonStandartTable, &QRadioButton::clicked, this, &DialogNormal::StandartTable);
    connect(ui->radioButtonIncrements, &QRadioButton::clicked, this, &DialogNormal::Increments);
    connect(ui->radioButtonLengthLine, &QRadioButton::clicked, this, &DialogNormal::LengthLines);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogNormal::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogNormal::NamePointChanged);
    connect(ui->lineEditFormula, &QLineEdit::textChanged, this, &DialogNormal::FormulaChanged);
}

DialogNormal::~DialogNormal()
{
    delete ui;
}

void DialogNormal::ChoosedObject(qint64 id, Scene::Type type){
    if(type == Scene::Point){
        VPointF point = data->GetPoint(id);
        if(number == 0){
            qint32 index = ui->comboBoxFirstPoint->findText(point.name());
            if ( index != -1 ) { // -1 for not found
                ui->comboBoxFirstPoint->setCurrentIndex(index);
                number++;
                return;
            }
        }
        if(number == 1){
            qint32 index = ui->comboBoxSecondPoint->findText(point.name());
            if ( index != -1 ) { // -1 for not found
                ui->comboBoxSecondPoint->setCurrentIndex(index);
                number = 0;
            }
            if(!isInitialized){
                this->show();
            }
        }
    }
}

void DialogNormal::DialogAccepted(){
    pointName = ui->lineEditNamePoint->text();
    typeLine = GetTypeLine(ui->comboBoxLineType);
    formula = ui->lineEditFormula->text();
    angle = ui->spinBoxAngle->value();
    firstPointId = getCurrentPointId(ui->comboBoxFirstPoint);
    secondPointId = getCurrentPointId(ui->comboBoxSecondPoint);
    emit DialogClosed(QDialog::Accepted);
}

qint64 DialogNormal::getSecondPointId() const{
    return secondPointId;
}

void DialogNormal::setSecondPointId(const qint64 &value, const qint64 &id){
    setCurrentPointId(ui->comboBoxSecondPoint, secondPointId, value, id);
}

qint64 DialogNormal::getFirstPointId() const{
    return firstPointId;
}

void DialogNormal::setFirstPointId(const qint64 &value, const qint64 &id){
    setCurrentPointId(ui->comboBoxFirstPoint, firstPointId, value, id);
}

qint32 DialogNormal::getAngle() const{
    return angle;
}

void DialogNormal::setAngle(const qint32 &value){
    angle = value;
    ui->spinBoxAngle->setValue(angle);
}

QString DialogNormal::getFormula() const{
    return formula;
}

void DialogNormal::setFormula(const QString &value){
    formula = value;
    ui->lineEditFormula->setText(formula);
}

QString DialogNormal::getTypeLine() const{
    return typeLine;
}

void DialogNormal::setTypeLine(const QString &value){
    typeLine = value;
    SetupTypeLine(ui->comboBoxLineType, value);
}

QString DialogNormal::getPointName() const{
    return pointName;
}

void DialogNormal::setPointName(const QString &value){
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}
