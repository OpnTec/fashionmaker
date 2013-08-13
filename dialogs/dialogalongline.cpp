#include "dialogalongline.h"
#include "ui_dialogalongline.h"

DialogAlongLine::DialogAlongLine(const VContainer *data, QWidget *parent) :
    DialogTool(data, parent), ui(new Ui::DialogAlongLine)
{
    ui->setupUi(this);
    number = 0;
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
    connect(bOk, &QPushButton::clicked, this, &DialogAlongLine::DialogAccepted);
    flagName = false;
    CheckState();
    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogAlongLine::DialogRejected);
    FillComboBoxTypeLine(ui->comboBoxLineType);
    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogAlongLine::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogAlongLine::PutVal);
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogAlongLine::ValChenged);

    ShowVariable(data->DataBase());
    connect(ui->radioButtonSizeGrowth, &QRadioButton::clicked, this, &DialogAlongLine::SizeGrowth);
    connect(ui->radioButtonStandartTable, &QRadioButton::clicked, this, &DialogAlongLine::StandartTable);
    connect(ui->radioButtonIncrements, &QRadioButton::clicked, this, &DialogAlongLine::Increments);
    connect(ui->radioButtonLengthLine, &QRadioButton::clicked, this, &DialogAlongLine::LengthLines);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogAlongLine::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogAlongLine::NamePointChanged);
    connect(ui->lineEditFormula, &QLineEdit::textChanged, this, &DialogAlongLine::FormulaChanged);
}

DialogAlongLine::~DialogAlongLine()
{
    delete ui;
}

void DialogAlongLine::ChoosedObject(qint64 id, Scene::Type type){
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

void DialogAlongLine::DialogAccepted(){
    pointName = ui->lineEditNamePoint->text();
    typeLine = GetTypeLine(ui->comboBoxLineType);
    formula = ui->lineEditFormula->text();
    firstPointId = getCurrentPointId(ui->comboBoxFirstPoint);
    secondPointId = getCurrentPointId(ui->comboBoxSecondPoint);
    emit DialogClosed(QDialog::Accepted);
}

qint64 DialogAlongLine::getSecondPointId() const{
    return secondPointId;
}

void DialogAlongLine::setSecondPointId(const qint64 &value, const qint64 &id){
    setCurrentPointId(ui->comboBoxSecondPoint, secondPointId, value, id);
}

qint64 DialogAlongLine::getFirstPointId() const{
    return firstPointId;
}

void DialogAlongLine::setFirstPointId(const qint64 &value, const qint64 &id){
    setCurrentPointId(ui->comboBoxFirstPoint, firstPointId, value, id);
}

QString DialogAlongLine::getFormula() const{
    return formula;
}

void DialogAlongLine::setFormula(const QString &value){
    formula = value;
    ui->lineEditFormula->setText(formula);
}

QString DialogAlongLine::getTypeLine() const{
    return typeLine;
}

void DialogAlongLine::setTypeLine(const QString &value){
    typeLine = value;
    SetupTypeLine(ui->comboBoxLineType, value);
}

QString DialogAlongLine::getPointName() const{
    return pointName;
}

void DialogAlongLine::setPointName(const QString &value){
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}
