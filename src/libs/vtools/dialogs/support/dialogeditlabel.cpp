#include "dialogeditlabel.h"
#include "ui_dialogeditlabel.h"

DialogEditLabel::DialogEditLabel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditLabel)
{
    ui->setupUi(this);
}

DialogEditLabel::~DialogEditLabel()
{
    delete ui;
}
