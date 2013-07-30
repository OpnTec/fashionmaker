#include "dialogbisector.h"
#include "ui_dialogbisector.h"

DialogBisector::DialogBisector(const VContainer *data, QWidget *parent) :
    DialogTool(data, parent), ui(new Ui::DialogBisector)
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
    connect(bOk, &QPushButton::clicked, this, &DialogBisector::DialogAccepted);
    flagName = false;
    CheckState();
    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogBisector::DialogRejected);
    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);
    FillComboBoxPoints(ui->comboBoxThirdPoint);
    FillComboBoxTypeLine(ui->comboBoxLineType);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogBisector::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogBisector::PutVal);
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogBisector::ValChenged);

    ShowBase();
    connect(ui->radioButtonSizeGrowth, &QRadioButton::clicked, this, &DialogBisector::SizeGrowth);
    connect(ui->radioButtonStandartTable, &QRadioButton::clicked, this, &DialogBisector::StandartTable);
    connect(ui->radioButtonIncrements, &QRadioButton::clicked, this, &DialogBisector::Increments);
    connect(ui->radioButtonLengthLine, &QRadioButton::clicked, this, &DialogBisector::LengthLines);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogBisector::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogBisector::NamePointChanged);
    connect(ui->lineEditFormula, &QLineEdit::textChanged, this, &DialogBisector::FormulaChanged);
}

DialogBisector::~DialogBisector(){
    delete ui;
}

void DialogBisector::ChoosedPoint(qint64 id, Scene::Type type){
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
                number++;
                return;
            }
        }
        if(number == 2){
            qint32 index = ui->comboBoxThirdPoint->findText(point.name());
            if ( index != -1 ) { // -1 for not found
                ui->comboBoxThirdPoint->setCurrentIndex(index);
                number = 0;
            }
            if(!isInitialized){
                this->show();
            }
        }
    }
}

QString DialogBisector::getPointName() const{
    return pointName;
}

void DialogBisector::setPointName(const QString &value){
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

QString DialogBisector::getTypeLine() const{
    return typeLine;
}

void DialogBisector::setTypeLine(const QString &value){
    typeLine = value;
    SetupTypeLine(ui->comboBoxLineType, value);
}

QString DialogBisector::getFormula() const{
    return formula;
}

void DialogBisector::setFormula(const QString &value){
    formula = value;
    ui->lineEditFormula->setText(formula);
}

qint64 DialogBisector::getFirstPointId() const{
    return firstPointId;
}

void DialogBisector::setFirstPointId(const qint64 &value){
    firstPointId = value;
    ChangeCurrentData(ui->comboBoxFirstPoint, value);
}

qint64 DialogBisector::getSecondPointId() const{
    return secondPointId;
}

void DialogBisector::setSecondPointId(const qint64 &value){
    secondPointId = value;
    ChangeCurrentData(ui->comboBoxSecondPoint, value);
}

qint64 DialogBisector::getThirdPointId() const{
    return thirdPointId;
}

void DialogBisector::setThirdPointId(const qint64 &value){
    thirdPointId = value;
    ChangeCurrentData(ui->comboBoxThirdPoint, value);
}

void DialogBisector::DialogAccepted(){
    pointName = ui->lineEditNamePoint->text();
    typeLine = GetTypeLine(ui->comboBoxLineType);
    formula = ui->lineEditFormula->text();
    qint32 index = ui->comboBoxFirstPoint->currentIndex();
    firstPointId = qvariant_cast<qint64>(ui->comboBoxFirstPoint->itemData(index));
    index = ui->comboBoxSecondPoint->currentIndex();
    secondPointId = qvariant_cast<qint64>(ui->comboBoxSecondPoint->itemData(index));
    index = ui->comboBoxThirdPoint->currentIndex();
    thirdPointId = qvariant_cast<qint64>(ui->comboBoxThirdPoint->itemData(index));
    emit DialogClosed(QDialog::Accepted);
}
