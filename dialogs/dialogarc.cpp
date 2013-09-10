#include "dialogarc.h"
#include "ui_dialogarc.h"
#include "../container/calculator.h"

DialogArc::DialogArc(const VContainer *data, Draw::Mode mode, QWidget *parent) :
    DialogTool(data, mode, parent), ui(new Ui::DialogArc), flagRadius(false), flagF1(false), flagF2(false),
    timerRadius(0), timerF1(0), timerF2(0), center(0), radius(QString()), f1(QString()), f2(QString()){
    ui->setupUi(this);

    timerRadius = new QTimer(this);
    connect(timerRadius, &QTimer::timeout, this, &DialogArc::EvalRadius);

    timerF1 = new QTimer(this);
    connect(timerF1, &QTimer::timeout, this, &DialogArc::EvalF1);

    timerF2 = new QTimer(this);
    connect(timerF2, &QTimer::timeout, this, &DialogArc::EvalF2);

    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogArc::DialogAccepted);

    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogArc::DialogRejected);
    FillComboBoxPoints(ui->comboBoxBasePoint);

    CheckState();

    listWidget = ui->listWidget;
    labelDescription = ui->labelDescription;
    radioButtonSizeGrowth = ui->radioButtonSizeGrowth;
    radioButtonStandartTable = ui->radioButtonStandartTable;
    radioButtonIncrements = ui->radioButtonIncrements;
    radioButtonLengthLine = ui->radioButtonLengthLine;

    connect(ui->toolButtonPutHereRadius, &QPushButton::clicked, this, &DialogArc::PutRadius);
    connect(ui->toolButtonPutHereF1, &QPushButton::clicked, this, &DialogArc::PutF1);
    connect(ui->toolButtonPutHereF2, &QPushButton::clicked, this, &DialogArc::PutF2);
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogArc::ValChenged);

    ShowVariable(data->DataBase());
    connect(ui->radioButtonSizeGrowth, &QRadioButton::clicked, this, &DialogArc::SizeGrowth);
    connect(ui->radioButtonStandartTable, &QRadioButton::clicked, this, &DialogArc::StandartTable);
    connect(ui->radioButtonIncrements, &QRadioButton::clicked, this, &DialogArc::Increments);
    connect(ui->radioButtonLengthLine, &QRadioButton::clicked, this, &DialogArc::LengthLines);
    connect(ui->radioButtonLineAngles, &QRadioButton::clicked, this, &DialogArc::LineArcs);

    connect(ui->toolButtonEqualRadius, &QPushButton::clicked, this, &DialogArc::EvalRadius);
    connect(ui->toolButtonEqualF1, &QPushButton::clicked, this, &DialogArc::EvalF1);
    connect(ui->toolButtonEqualF2, &QPushButton::clicked, this, &DialogArc::EvalF2);

    connect(ui->lineEditRadius, &QLineEdit::textChanged, this, &DialogArc::RadiusChanged);
    connect(ui->lineEditF1, &QLineEdit::textChanged, this, &DialogArc::F1Changed);
    connect(ui->lineEditF2, &QLineEdit::textChanged, this, &DialogArc::F2Changed);
}

qint64 DialogArc::GetCenter() const{
    return center;
}

void DialogArc::SetCenter(const qint64 &value){
    center = value;
    ChangeCurrentData(ui->comboBoxBasePoint, center);
}

QString DialogArc::GetF2() const{
    return f2;
}

void DialogArc::SetF2(const QString &value){
    f2 = value;
    ui->lineEditF2->setText(f2);
}

QString DialogArc::GetF1() const{
    return f1;
}

void DialogArc::SetF1(const QString &value){
    f1 = value;
    ui->lineEditF1->setText(f1);
}

QString DialogArc::GetRadius() const{
    return radius;
}

void DialogArc::SetRadius(const QString &value){
    radius = value;
    ui->lineEditRadius->setText(radius);
}

DialogArc::~DialogArc(){
delete ui;
}

void DialogArc::ChoosedObject(qint64 id, Scene::Type type){
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

void DialogArc::DialogAccepted(){
    radius = ui->lineEditRadius->text();
    f1 = ui->lineEditF1->text();
    f2 = ui->lineEditF2->text();
    center = getCurrentPointId(ui->comboBoxBasePoint);
    emit DialogClosed(QDialog::Accepted);
}

void DialogArc::ValChenged(int row){
    if(ui->listWidget->count() == 0){
        return;
    }
    QListWidgetItem *item = ui->listWidget->item( row );
    if(ui->radioButtonLineAngles->isChecked()){
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetLineArc(item->text()))
                .arg("Значення кута лінії.");
        ui->labelDescription->setText(desc);
        return;
    }
    DialogTool::ValChenged(row);
}

void DialogArc::PutRadius(){
    PutValHere(ui->lineEditRadius, ui->listWidget);
}

void DialogArc::PutF1(){
    PutValHere(ui->lineEditF1, ui->listWidget);
}

void DialogArc::PutF2(){
    PutValHere(ui->lineEditF2, ui->listWidget);
}

void DialogArc::LineArcs(){
    ShowLineArcs();
}

void DialogArc::RadiusChanged(){
    ValFormulaChanged(flagRadius, ui->lineEditRadius, timerRadius);
}

void DialogArc::F1Changed(){
    ValFormulaChanged(flagF1, ui->lineEditF1, timerF1);
}

void DialogArc::F2Changed(){
    ValFormulaChanged(flagF2, ui->lineEditF2, timerF2);
}

void DialogArc::CheckState(){
    Q_CHECK_PTR(bOk);
    bOk->setEnabled(flagRadius & flagF1 & flagF2);
}

void DialogArc::EvalRadius(){
    Eval(ui->lineEditRadius, flagRadius, timerRadius, ui->labelResultRadius);
}

void DialogArc::EvalF1(){
    Eval(ui->lineEditF1, flagF1, timerF1, ui->labelResultF1);
}

void DialogArc::EvalF2(){
    Eval(ui->lineEditF2, flagF2, timerF2, ui->labelResultF2);
}

void DialogArc::ShowLineArcs(){
    disconnect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogArc::ValChenged);
    ui->listWidget->clear();
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogArc::ValChenged);
    const QMap<QString, qreal> *lineArcsTable = data->DataLineAngles();
    QMapIterator<QString, qreal> i(*lineArcsTable);
    while (i.hasNext()) {
        i.next();
        QListWidgetItem *item = new QListWidgetItem(i.key());

        item->setFont(QFont("Times", 12, QFont::Bold));
        ui->listWidget->addItem(item);
    }
    ui->listWidget->setCurrentRow (0);
}
