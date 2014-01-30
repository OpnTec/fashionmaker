/************************************************************************
 **
 **  @file   dialogarc.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#include "dialogarc.h"
#include "ui_dialogarc.h"

#include <QPushButton>
#include <QtWidgets>

DialogArc::DialogArc(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogArc), flagRadius(false), flagF1(false), flagF2(false),
    timerRadius(0), timerF1(0), timerF2(0), center(0), radius(QString()), f1(QString()), f2(QString())
{
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
    radioButtonStandardTable = ui->radioButtonStandardTable;
    radioButtonIncrements = ui->radioButtonIncrements;
    radioButtonLengthLine = ui->radioButtonLengthLine;
    radioButtonLengthArc = ui->radioButtonLengthArc;
    radioButtonLengthCurve = ui->radioButtonLengthSpline;

    connect(ui->toolButtonPutHereRadius, &QPushButton::clicked, this, &DialogArc::PutRadius);
    connect(ui->toolButtonPutHereF1, &QPushButton::clicked, this, &DialogArc::PutF1);
    connect(ui->toolButtonPutHereF2, &QPushButton::clicked, this, &DialogArc::PutF2);
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogArc::ValChenged);

    ShowVariable(data->DataBase());
    connect(ui->radioButtonSizeGrowth, &QRadioButton::clicked, this, &DialogArc::SizeGrowth);
    connect(ui->radioButtonStandardTable, &QRadioButton::clicked, this, &DialogArc::StandardTable);
    connect(ui->radioButtonIncrements, &QRadioButton::clicked, this, &DialogArc::Increments);
    connect(ui->radioButtonLengthLine, &QRadioButton::clicked, this, &DialogArc::LengthLines);
    connect(ui->radioButtonLineAngles, &QRadioButton::clicked, this, &DialogArc::LineAngles);
    connect(ui->radioButtonLengthArc, &QRadioButton::clicked, this, &DialogArc::LengthArcs);
    connect(ui->radioButtonLengthSpline, &QRadioButton::clicked, this, &DialogArc::LengthCurves);

    connect(ui->toolButtonEqualRadius, &QPushButton::clicked, this, &DialogArc::EvalRadius);
    connect(ui->toolButtonEqualF1, &QPushButton::clicked, this, &DialogArc::EvalF1);
    connect(ui->toolButtonEqualF2, &QPushButton::clicked, this, &DialogArc::EvalF2);

    connect(ui->lineEditRadius, &QLineEdit::textChanged, this, &DialogArc::RadiusChanged);
    connect(ui->lineEditF1, &QLineEdit::textChanged, this, &DialogArc::F1Changed);
    connect(ui->lineEditF2, &QLineEdit::textChanged, this, &DialogArc::F2Changed);
}

DialogArc::~DialogArc()
{
    delete ui;
}

void DialogArc::SetCenter(const qint64 &value)
{
    center = value;
    ChangeCurrentData(ui->comboBoxBasePoint, center);
}

void DialogArc::SetF2(const QString &value)
{
    f2 = value;
    ui->lineEditF2->setText(f2);
}

void DialogArc::SetF1(const QString &value)
{
    f1 = value;
    ui->lineEditF1->setText(f1);
}

void DialogArc::SetRadius(const QString &value)
{
    radius = value;
    ui->lineEditRadius->setText(radius);
}

void DialogArc::ChoosedObject(qint64 id, const Scene::Scenes &type)
{
    if (type == Scene::Point)
    {
        const VPointF *point = data->GeometricObject<const VPointF *>(id);

        ChangeCurrentText(ui->comboBoxBasePoint, point->name());
        emit ToolTip("");
        this->show();
    }
}

void DialogArc::DialogAccepted()
{
    radius = ui->lineEditRadius->text();
    f1 = ui->lineEditF1->text();
    f2 = ui->lineEditF2->text();
    center = getCurrentObjectId(ui->comboBoxBasePoint);
    emit DialogClosed(QDialog::Accepted);
}

void DialogArc::ValChenged(int row)
{
    if (ui->listWidget->count() == 0)
    {
        return;
    }
    QListWidgetItem *item = ui->listWidget->item( row );
    if (ui->radioButtonLineAngles->isChecked())
    {
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetLineAngle(item->text()))
                .arg(tr("Value of angle of line."));
        ui->labelDescription->setText(desc);
        return;
    }
    DialogTool::ValChenged(row);
}

void DialogArc::PutRadius()
{
    PutValHere(ui->lineEditRadius, ui->listWidget);
}

void DialogArc::PutF1()
{
    PutValHere(ui->lineEditF1, ui->listWidget);
}

void DialogArc::PutF2()
{
    PutValHere(ui->lineEditF2, ui->listWidget);
}

void DialogArc::LineAngles()
{
    ShowLineAngles();
}

void DialogArc::RadiusChanged()
{
    labelEditFormula = ui->labelEditRadius;
    ValFormulaChanged(flagRadius, ui->lineEditRadius, timerRadius);
}

void DialogArc::F1Changed()
{
    labelEditFormula = ui->labelEditF1;
    ValFormulaChanged(flagF1, ui->lineEditF1, timerF1);
}

void DialogArc::F2Changed()
{
    labelEditFormula = ui->labelEditF2;
    ValFormulaChanged(flagF2, ui->lineEditF2, timerF2);
}

void DialogArc::CheckState()
{
    Q_CHECK_PTR(bOk);
    bOk->setEnabled(flagRadius && flagF1 && flagF2);
}

void DialogArc::EvalRadius()
{
    labelEditFormula = ui->labelEditRadius;
    Eval(ui->lineEditRadius, flagRadius, timerRadius, ui->labelResultRadius);
}

void DialogArc::EvalF1()
{
    labelEditFormula = ui->labelEditF1;
    Eval(ui->lineEditF1, flagF1, timerF1, ui->labelResultF1);
}

void DialogArc::EvalF2()
{
    labelEditFormula = ui->labelEditF2;
    Eval(ui->lineEditF2, flagF2, timerF2, ui->labelResultF2);
}

void DialogArc::ShowLineAngles()
{
    disconnect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogArc::ValChenged);
    ui->listWidget->clear();
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogArc::ValChenged);
    const QHash<QString, qreal> *lineAnglesTable = data->DataLineAngles();
    Q_CHECK_PTR(lineAnglesTable);
    QHashIterator<QString, qreal> i(*lineAnglesTable);
    while (i.hasNext())
    {
        i.next();
        QListWidgetItem *item = new QListWidgetItem(i.key());

        item->setFont(QFont("Times", 12, QFont::Bold));
        ui->listWidget->addItem(item);
    }
    ui->listWidget->setCurrentRow (0);
}
