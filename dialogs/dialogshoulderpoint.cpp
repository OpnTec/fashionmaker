#include "dialogshoulderpoint.h"
#include "ui_dialogshoulderpoint.h"

DialogShoulderPoint::DialogShoulderPoint(const VContainer *data, QWidget *parent) :
    DialogTool(data, parent), ui(new Ui::DialogShoulderPoint)
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
    connect(bOk, &QPushButton::clicked, this, &DialogShoulderPoint::DialogAccepted);
    flagName = false;
    CheckState();
    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogShoulderPoint::DialogRejected);
    FillComboBoxTypeLine(ui->comboBoxLineType);
    FillComboBoxPoints(ui->comboBoxP1Line);
    FillComboBoxPoints(ui->comboBoxP2Line);
    FillComboBoxPoints(ui->comboBoxPShoulder);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogShoulderPoint::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogShoulderPoint::PutVal);
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogShoulderPoint::ValChenged);

    ShowBase();
    connect(ui->radioButtonSizeGrowth, &QRadioButton::clicked, this, &DialogShoulderPoint::SizeGrowth);
    connect(ui->radioButtonStandartTable, &QRadioButton::clicked, this, &DialogShoulderPoint::StandartTable);
    connect(ui->radioButtonIncrements, &QRadioButton::clicked, this, &DialogShoulderPoint::Increments);
    connect(ui->radioButtonLengthLine, &QRadioButton::clicked, this, &DialogShoulderPoint::LengthLines);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogShoulderPoint::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogShoulderPoint::NamePointChanged);
    connect(ui->lineEditFormula, &QLineEdit::textChanged, this, &DialogShoulderPoint::FormulaChanged);
}

DialogShoulderPoint::~DialogShoulderPoint()
{
    delete ui;
}

void DialogShoulderPoint::ChoosedObject(qint64 id, Scene::Type type){
    if(type == Scene::Point){
        VPointF point = data->GetPoint(id);
        if(number == 0){
            qint32 index = ui->comboBoxP1Line->findText(point.name());
            if ( index != -1 ) { // -1 for not found
                ui->comboBoxP1Line->setCurrentIndex(index);
                number++;
                return;
            }
        }
        if(number == 1){
            qint32 index = ui->comboBoxP2Line->findText(point.name());
            if ( index != -1 ) { // -1 for not found
                ui->comboBoxP2Line->setCurrentIndex(index);
                number++;
                return;
            }
        }
        if(number == 2){
            qint32 index = ui->comboBoxPShoulder->findText(point.name());
            if ( index != -1 ) { // -1 for not found
                ui->comboBoxPShoulder->setCurrentIndex(index);
                number = 0;
            }
            if(!isInitialized){
                this->show();
            }
        }
    }
}

void DialogShoulderPoint::DialogAccepted(){
    pointName = ui->lineEditNamePoint->text();
    typeLine = GetTypeLine(ui->comboBoxLineType);
    formula = ui->lineEditFormula->text();
    qint32 index = ui->comboBoxP1Line->currentIndex();
    p1Line = qvariant_cast<qint64>(ui->comboBoxP1Line->itemData(index));
    index = ui->comboBoxP2Line->currentIndex();
    p2Line = qvariant_cast<qint64>(ui->comboBoxP2Line->itemData(index));
    index = ui->comboBoxPShoulder->currentIndex();
    pShoulder = qvariant_cast<qint64>(ui->comboBoxPShoulder->itemData(index));
    emit DialogClosed(QDialog::Accepted);
}

qint64 DialogShoulderPoint::getPShoulder() const{
    return pShoulder;
}

void DialogShoulderPoint::setPShoulder(const qint64 &value){
    pShoulder = value;
    ChangeCurrentData(ui->comboBoxPShoulder, value);
}

qint64 DialogShoulderPoint::getP2Line() const{
    return p2Line;
}

void DialogShoulderPoint::setP2Line(const qint64 &value){
    p2Line = value;
    ChangeCurrentData(ui->comboBoxP2Line, value);
}

qint64 DialogShoulderPoint::getP1Line() const{
    return p1Line;
}

void DialogShoulderPoint::setP1Line(const qint64 &value)
{
    p1Line = value;
    ChangeCurrentData(ui->comboBoxP1Line, value);
}

QString DialogShoulderPoint::getFormula() const{
    return formula;
}

void DialogShoulderPoint::setFormula(const QString &value){
    formula = value;
    ui->lineEditFormula->setText(formula);
}

QString DialogShoulderPoint::getTypeLine() const{
    return typeLine;
}

void DialogShoulderPoint::setTypeLine(const QString &value){
    typeLine = value;
    SetupTypeLine(ui->comboBoxLineType, value);
}

QString DialogShoulderPoint::getPointName() const{
    return pointName;
}

void DialogShoulderPoint::setPointName(const QString &value){
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}
