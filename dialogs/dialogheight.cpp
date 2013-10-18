#include "dialogheight.h"
#include "ui_dialogheight.h"

DialogHeight::DialogHeight(const VContainer *data, Draw::Draws mode, QWidget *parent) :
    DialogTool(data, mode, parent), ui(new Ui::DialogHeight), number(0), pointName(QString()),
    typeLine(QString()), basePointId(0), p1LineId(0), p2LineId(0){
    ui->setupUi(this);
    labelEditNamePoint = ui->labelEditNamePoint;
    flagFormula = true;
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogHeight::DialogAccepted);
    flagName = false;
    CheckState();
    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogHeight::DialogRejected);
    FillComboBoxPoints(ui->comboBoxBasePoint);
    FillComboBoxPoints(ui->comboBoxP1Line);
    FillComboBoxPoints(ui->comboBoxP2Line);
    FillComboBoxTypeLine(ui->comboBoxLineType);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogHeight::NamePointChanged);
}

DialogHeight::~DialogHeight(){
    delete ui;
}

QString DialogHeight::getPointName() const{
    return pointName;
}

void DialogHeight::setPointName(const QString &value){
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

QString DialogHeight::getTypeLine() const{
    return typeLine;
}

void DialogHeight::setTypeLine(const QString &value){
    typeLine = value;
    SetupTypeLine(ui->comboBoxLineType, value);
}

qint64 DialogHeight::getBasePointId() const{
    return basePointId;
}

void DialogHeight::setBasePointId(const qint64 &value, const qint64 &id){
    basePointId = value;
    setCurrentPointId(ui->comboBoxBasePoint, basePointId, value, id);
}

qint64 DialogHeight::getP1LineId() const{
    return p1LineId;
}

void DialogHeight::setP1LineId(const qint64 &value, const qint64 &id){
    p1LineId = value;
    setCurrentPointId(ui->comboBoxP1Line, p1LineId, value, id);
}

qint64 DialogHeight::getP2LineId() const{
    return p2LineId;
}

void DialogHeight::setP2LineId(const qint64 &value, const qint64 &id){
    p2LineId = value;
    setCurrentPointId(ui->comboBoxP2Line, p2LineId, value, id);
}

void DialogHeight::ChoosedObject(qint64 id, Scene::Scenes type){
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
        switch(number){
        case(0):
            ChangeCurrentText(ui->comboBoxBasePoint, point.name());
            number++;
            emit ToolTip(tr("Select first point of line"));
            break;
        case(1):
            ChangeCurrentText(ui->comboBoxP1Line, point.name());
            number++;
            emit ToolTip(tr("Select second point of line"));
            break;
        case(2):
            ChangeCurrentText(ui->comboBoxP2Line, point.name());
            number = 0;
            emit ToolTip(tr(""));
            if(!isInitialized){
                this->show();
            }
            break;
        }
    }
}

void DialogHeight::DialogAccepted(){
    pointName = ui->lineEditNamePoint->text();
    typeLine = GetTypeLine(ui->comboBoxLineType);
    basePointId = getCurrentPointId(ui->comboBoxBasePoint);
    p1LineId = getCurrentPointId(ui->comboBoxP1Line);
    p2LineId = getCurrentPointId(ui->comboBoxP2Line);
    emit DialogClosed(QDialog::Accepted);
}

