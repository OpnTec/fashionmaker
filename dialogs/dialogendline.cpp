#include "dialogendline.h"
#include "ui_dialogendline.h"
#include <QCloseEvent>
#include <QString>

#include "../container/vpointf.h"
#include "../container/calculator.h"

DialogEndLine::DialogEndLine(const VContainer *data, QWidget *parent) :
    DialogTool(data, parent), ui(new Ui::DialogEndLine)
{
    ui->setupUi(this);
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
    connect(bOk, &QPushButton::clicked, this, &DialogEndLine::DialogAccepted);
    flagName = false;
    CheckState();
    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogEndLine::DialogRejected);
    FillComboBoxPoints(ui->comboBoxBasePoint);
    FillComboBoxTypeLine(ui->comboBoxLineType);

    connect(ui->toolButtonArrowDown, &QPushButton::clicked, this,
            &DialogEndLine::ArrowDown);
    connect(ui->toolButtonArrowUp, &QPushButton::clicked, this,
            &DialogEndLine::ArrowUp);
    connect(ui->toolButtonArrowLeft, &QPushButton::clicked, this,
            &DialogEndLine::ArrowLeft);
    connect(ui->toolButtonArrowRight, &QPushButton::clicked, this,
            &DialogEndLine::ArrowRight);
    connect(ui->toolButtonArrowLeftUp, &QPushButton::clicked, this,
            &DialogEndLine::ArrowLeftUp);
    connect(ui->toolButtonArrowLeftDown, &QPushButton::clicked, this,
            &DialogEndLine::ArrowLeftDown);
    connect(ui->toolButtonArrowRightUp, &QPushButton::clicked, this,
            &DialogEndLine::ArrowRightUp);
    connect(ui->toolButtonArrowRightDown, &QPushButton::clicked, this,
            &DialogEndLine::ArrowRightDown);
    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogEndLine::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogEndLine::PutVal);
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogEndLine::ValChenged);

    ShowVariable(data->DataBase());
    connect(ui->radioButtonSizeGrowth, &QRadioButton::clicked, this, &DialogEndLine::SizeGrowth);
    connect(ui->radioButtonStandartTable, &QRadioButton::clicked, this, &DialogEndLine::StandartTable);
    connect(ui->radioButtonIncrements, &QRadioButton::clicked, this, &DialogEndLine::Increments);
    connect(ui->radioButtonLengthLine, &QRadioButton::clicked, this, &DialogEndLine::LengthLines);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogEndLine::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogEndLine::NamePointChanged);
    connect(ui->lineEditFormula, &QLineEdit::textChanged, this, &DialogEndLine::FormulaChanged);
}

void DialogEndLine::ChoosedObject(qint64 id, Scene::Type type){
    if(type == Scene::Point){
        VPointF point = data->GetPoint(id);
        ChangeCurrentText(ui->comboBoxBasePoint, point.name());
        this->show();
    }
}

QString DialogEndLine::getPointName() const{
    return pointName;
}

void DialogEndLine::setPointName(const QString &value){
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

QString DialogEndLine::getTypeLine() const{
    return typeLine;
}

void DialogEndLine::setTypeLine(const QString &value){
    typeLine = value;
    SetupTypeLine(ui->comboBoxLineType, value);
}

QString DialogEndLine::getFormula() const{
    return formula;
}

void DialogEndLine::setFormula(const QString &value){
    formula = value;
    ui->lineEditFormula->setText(formula);
}

qint32 DialogEndLine::getAngle() const{
    return angle;
}

void DialogEndLine::setAngle(const qint32 &value){
    angle = value;
    ui->spinBoxAngle->setValue(angle);
}

qint64 DialogEndLine::getBasePointId() const{
    return basePointId;
}

void DialogEndLine::setBasePointId(const qint64 &value){
    basePointId = value;
    ChangeCurrentData(ui->comboBoxBasePoint, value);
}

void DialogEndLine::DialogAccepted(){
    pointName = ui->lineEditNamePoint->text();
    typeLine = GetTypeLine(ui->comboBoxLineType);
    formula = ui->lineEditFormula->text();
    angle = ui->spinBoxAngle->value();
    qint32 index = ui->comboBoxBasePoint->currentIndex();
    basePointId = qvariant_cast<qint64>(ui->comboBoxBasePoint->itemData(index));
    emit DialogClosed(QDialog::Accepted);
}

DialogEndLine::~DialogEndLine()
{
    delete ui;
}
