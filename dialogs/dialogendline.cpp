#include "dialogendline.h"
#include "ui_dialogendline.h"
#include <QCloseEvent>
#include <QString>
#include "container/vpointf.h"
#include "container/calculator.h"

DialogEndLine::DialogEndLine(const VContainer *data, Draw::Mode mode, QWidget *parent) :
    DialogTool(data, mode, parent), ui(new Ui::DialogEndLine), pointName(QString()), typeLine(QString()),
    formula(QString()), angle(0), basePointId(0){
    ui->setupUi(this);
    spinBoxAngle = ui->doubleSpinBoxAngle;
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
    if(idDetail == 0 && mode == Draw::Modeling){
        if(type == Scene::Detail){
            idDetail = id;
            return;
        }
    }
    if(mode == Draw::Modeling){
        if(!CheckObject(id)){
            return;
        }
    }
    if(type == Scene::Point){
        VPointF point;
        if(mode == Draw::Calculation){
            point = data->GetPoint(id);
        } else {
            point = data->GetModelingPoint(id);
        }
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

qreal DialogEndLine::getAngle() const{
    return angle;
}

void DialogEndLine::setAngle(const qreal &value){
    angle = value;
    ui->doubleSpinBoxAngle->setValue(angle);
}

qint64 DialogEndLine::getBasePointId() const{
    return basePointId;
}

void DialogEndLine::setBasePointId(const qint64 &value, const qint64 &id){
    setCurrentPointId(ui->comboBoxBasePoint, basePointId, value, id);
}

void DialogEndLine::DialogAccepted(){
    pointName = ui->lineEditNamePoint->text();
    typeLine = GetTypeLine(ui->comboBoxLineType);
    formula = ui->lineEditFormula->text();
    angle = ui->doubleSpinBoxAngle->value();
    basePointId = getCurrentPointId(ui->comboBoxBasePoint);
    emit DialogClosed(QDialog::Accepted);
}

DialogEndLine::~DialogEndLine()
{
    delete ui;
}
